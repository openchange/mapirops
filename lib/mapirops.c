/*
   OpenChange MAPI implementation.

   Copyright (C) Julien Kerihuel 2012.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
   \file mapirops.c
   \author Julien Kerihuel <j.kerihuel@openchange.org>
   \version 0.1
   \brief Packing/Unpacking MAPI rops buffer and primitive types
 */

#include "config.h"

#include "libmapirops.h"
#include "mapirops_uuid.h"

/** \def MAPIROPS_CHUNK_SIZE
    Default chunk size when expanding size of mapirops buffer 
*/
#define	MAPIROPS_CHUNK_SIZE	1024

/** \def MAPIROPS_SYSLOG_NAME
    Defines the syslog identifier for logging messages
*/
#define	MAPIROPS_SYSLOG_NAME	"mapirops"

/** \cond */

#define	MAPIROPS_PUSH_NEED_BYTES(mapirops, n) \
	MAPIROPS_CHECK(mapirops_push_expand(mapirops, mapirops->data.length+(n)))

#define	MAPIROPS_PULL_NEED_BYTES(mapirops, n) do {					\
	if (unlikely((n)) > mapirops->data.length ||					\
	    mapirops->offset + (n) > mapirops->data.length) {				\
		return mapirops_error(MAPIROPS_ERR_BUFSIZE, LOG_ERR,			\
				   "Pull bytes %u (%s)", (unsigned)n, __location__);	\
	}										\
} while (0)

/** \endcond */

/**
   \details Log MAPIROPS error into syslog

   \param errnum mapistore_err_code to return
   \param priority syslog priority code
   \param format format string to use for logging
   \param ... format string arguments

   \return Provided errnum code
 */
enum mapirops_err_code mapirops_error(enum mapirops_err_code errnum, 
				      int priority,
				      const char *format, ...)
{
	va_list	ap;

	va_start(ap, format);
	va_end(ap);

	vsyslog(priority, format, ap);

	return errnum;
}

/**
   \details Expand the available space in the buffer to size

   \param push Pointer to the mapirops_push structure
   \param extra_size The extra size to add to current buffer

   \return MAPIROPS_ERR_SUCCESS on success, MAPIROPS_ERR_BUFSIZE if an
   overflow is detected, or MAPIROPS_ERR_ALLOC if realloc failed.
 */
enum mapirops_err_code mapirops_push_expand(struct mapirops_push *push, 
					    uint32_t extra_size)
{
	uint32_t	size = extra_size + push->offset;

	if (size < push->offset) {
		return mapirops_error(MAPIROPS_ERR_BUFSIZE, LOG_ERR,
				      "Overflow in push_expand to %u", size);
	}

	if (talloc_get_size(push->data.data) >= size) {
		return MAPIROPS_ERR_SUCCESS;
	}

	push->data.length += MAPIROPS_CHUNK_SIZE;
	if ((size + 1) > push->data.length) {
		push->data.length = size + 1;
	}

	push->data.data = talloc_realloc(push, push->data.data, uint8_t, 
					 push->data.length);

	if (push->data.data == NULL) {
		return mapirops_error(MAPIROPS_ERR_ALLOC, LOG_ERR,
				      "Failed to push_expand to %u", push->data.length);
	}

	return MAPIROPS_ERR_SUCCESS;
}

/**
   \details Destructor for mapirops_push context

   \param data Generic pointer to data with mapirops_push information

   \return MAPIROPS_ERR_SUCCESS on success, otherwise MAPIROPS error
 */
static enum mapirops_err_code mapirops_push_destructor(void *data)
{
	struct mapirops_push	*push = (struct mapirops_push *)data;
	int			ret;

	if (push->utf8to16) {
		ret = iconv_close(push->utf8to16);
		if (ret == -1) {
			return MAPIROPS_ERR_ICONV;
		}
	}

	if (push->utf8toascii) {
		ret = iconv_close(push->utf8toascii);
		if (ret == -1) {
			return MAPIROPS_ERR_ICONV;
		}
	}

	return MAPIROPS_ERR_SUCCESS;
}

/**
   \details Initialize mapirops_push data structure
   \param mem_ctx Pointer to the TALLOC memory context to use
   \return Allocated mapirops_push structure on success, otherwise
   NULL.
 */
struct mapirops_push *mapirops_push_init(TALLOC_CTX *mem_ctx)
{
	struct mapirops_push *push = talloc_zero(mem_ctx, struct mapirops_push);

	openlog(MAPIROPS_SYSLOG_NAME, LOG_PERROR|LOG_PID, LOG_USER);

	if (push == NULL)
		return NULL;

	push->utf8to16 = iconv_open("UTF-16LE", "UTF-8");
	if (push->utf8to16 == (iconv_t)-1) {
		talloc_free(push);
		mapirops_error(MAPIROPS_ERR_ICONV, LOG_ERR,
			       "Failed to open push conversion descriptor from UTF8 to UTF16");
		return NULL;
	}

	push->utf8toascii = iconv_open("ASCII", "UTF-8//IGNORE");
	if (push->utf8toascii == (iconv_t)-1) {
		talloc_free(push);
		mapirops_error(MAPIROPS_ERR_ICONV, LOG_ERR,
			       "Failed to open push conversion descriptor from UTF8 to ASCII");
		return NULL;
	}

	talloc_set_destructor((void *)push, (int (*)(void *))mapirops_push_destructor);

	return push;
}

/**
   \details Destructor for mapirops_pull context
   
   \param data Generic pointer to data with mapirops_pull information

   \return MAPIROPS_ERR_SUCCESS on success, otherwise MAPIROPS error
 */
static enum mapirops_err_code mapirops_pull_destructor(void *data)
{
	struct mapirops_pull	*pull = (struct mapirops_pull *)data;
	int			ret;

	if (pull->utf16to8) {
		ret = iconv_close(pull->utf16to8);
		if (ret == -1) {
			return MAPIROPS_ERR_ICONV;
		}
	}
	return MAPIROPS_ERR_SUCCESS;
}

/**
   \details Initialize mapirops_pull data structure
   \param mem_ctx Pointer to the TALLOC memory context to use
   \return Allocated mapirops_pull structure on success, otherwise
   NULL.
 */
struct mapirops_pull *mapirops_pull_init(TALLOC_CTX *mem_ctx)
{
	struct mapirops_pull *pull = talloc_zero(mem_ctx, struct mapirops_pull);

	openlog(MAPIROPS_SYSLOG_NAME, LOG_PERROR|LOG_PID, LOG_USER);

	if (pull == NULL)
		return NULL;

	pull->utf16to8 = iconv_open("UTF-8", "UTF-16LE");
	if (pull->utf16to8 == (iconv_t)-1) {
		talloc_free(pull);
		mapirops_error(MAPIROPS_ERR_ICONV, LOG_ERR,
			       "Failed to open pull conversion descriptor from UTF16 to UTF8");
		return NULL;
	}

	talloc_set_destructor((void *)pull, (int (*)(void *))mapirops_pull_destructor);

	return pull;
}


/**
   \details Push a set of bytes

   \param push Pointer to the mapirops_push structure
   \param data Pointer to the array of bytes to push
   \param n Size of the array of bytes

   \return MAPIROPS_ERR_SUCCESS on success, otherwise MAPIROPS error
 */
enum mapirops_err_code mapirops_push_bytes(struct mapirops_push *push, const uint8_t *data, uint32_t n)
{
	MAPIROPS_PUSH_NEED_BYTES(push, n);
	memcpy(push->data.data + push->offset, data, n);
	push->offset += n;
	return MAPIROPS_ERR_SUCCESS;
}


/**
   \details Pull a set of bytes

   \param pull Pointer to the mapirops_pull structure
   \param data Pointer to the array of bytes to return
   \param n Size of the array of bytes to return

   \note Calling function is responsible of data memory allocation

   \return MAPIROPS_ERR_SUCCESS on success, otherwise MAPIROPS error
 */
enum mapirops_err_code mapirops_pull_bytes(struct mapirops_pull *pull, uint8_t *data, uint32_t n)
{
	MAPIROPS_PULL_NEED_BYTES(pull, n);
	memcpy(data, pull->data.data + pull->offset, n);
	pull->offset += n;
	return MAPIROPS_ERR_SUCCESS;
}


/**
   \details Push a int8_t

   int8_t is a signed char which value goes from -127 to 127.

   \param push Pointer to mapirops_push structure
   \param v int8_t value to push

   \return MAPIROPS_ERR_SUCCESS on success, otherwise MAPIROPS error
 */
enum mapirops_err_code mapirops_push_int8(struct mapirops_push *push, int8_t v)
{
	MAPIROPS_PUSH_NEED_BYTES(push, 1);
	SCVAL(push->data.data, push->offset, (uint8_t)v);
	push->offset += 1;
	return MAPIROPS_ERR_SUCCESS;
}


/**
   \details Parse and pull a int8_t

   int8_t is a signed char which value goes from -127 to 127.

   \param pull Pointer to mapirops_pull structure
   \param v Pointer to the int8_t value to return

   \return MAPIROPS_ERR_SUCCESS on success, otherwise MAPIROPS error
 */
enum mapirops_err_code mapirops_pull_int8(struct mapirops_pull *pull, int8_t *v)
{
	MAPIROPS_PULL_NEED_BYTES(pull, 1);
	*v = (int8_t)CVAL(pull->data.data, pull->offset);
	pull->offset += 1;
	return MAPIROPS_ERR_SUCCESS;
}

/**
   \details Push a uint8_t

   uint8_t is an unsigned char which value goes from 0 to 255.

   \param push Pointer to the mapirops_push structure
   \param v uint8_t value to push

   \return MAPIROPS_ERR_SUCCESS on success, otherwise MAPIROPS error
 */
enum mapirops_err_code mapirops_push_uint8(struct mapirops_push *push, uint8_t v)
{
	MAPIROPS_PUSH_NEED_BYTES(push, 1);
	SCVAL(push->data.data, push->offset, v);
	push->offset += 1;
	return MAPIROPS_ERR_SUCCESS;
}


/**
   \details Pull a uint8_t

   uint8_t is an unsigned char which value goes from 0 to 255.
   
   \param pull Pointer to the mapirops_pull structure
   \param v Pointer to the uint8_t value to return

   \return MAPIROPS_ERR_SUCCESS on success, otherwise MAPIROPS error
 */
enum mapirops_err_code mapirops_pull_uint8(struct mapirops_pull *pull, uint8_t *v)
{
	MAPIROPS_PULL_NEED_BYTES(pull, 1);
	*v = CVAL(pull->data.data, pull->offset);
	pull->offset += 1;
	return MAPIROPS_ERR_SUCCESS;
}


/**
   \details Push a int16_t

   int16_t is a signed char which values goes from -32767 to 32767

   \param push Pointer to the mapirops_push structure
   \param v int16_t value to push

   \return MAPIROPS_ERR_SUCCESS on success, otherwise MAPIROPS error
*/
enum mapirops_err_code mapirops_push_int16(struct mapirops_push *push, int16_t v)
{
	MAPIROPS_PUSH_NEED_BYTES(push, 2);
	SSVAL(push->data.data, push->offset, v);
	push->offset += 2;
	return MAPIROPS_ERR_SUCCESS;
}

/**
   \details Pull a int16_t

   int16_t is a signed char which values goes from -32767 to 32767

   \param pull Pointer to the mapirops_pull structure
   \param v Pointer to the int16_t value to return

   \return MAPIROPS_ERR_SUCCESS on success, otherwise MAPIROPS error
 */
enum mapirops_err_code mapirops_pull_int16(struct mapirops_pull *pull, int16_t *v)
{
	MAPIROPS_PULL_NEED_BYTES(pull, 2);
	*v = (uint16_t)SVAL(pull->data.data, pull->offset);
	pull->offset += 2;
	return MAPIROPS_ERR_SUCCESS;
}


/**
   \details Push a uint16_t

   uint16_t is an unsigned char which values goes from 0 to 65535

   \param push Pointer to the mapirops_push structure
   \param v the uint16_t value to push

   \return MAPIROPS_ERR_SUCCESS on success, otherwise MAPIROPS error
 */
enum mapirops_err_code mapirops_push_uint16(struct mapirops_push *push, uint16_t v)
{
	MAPIROPS_PUSH_NEED_BYTES(push, 2);
	SSVAL(push->data.data, push->offset, v);
	push->offset += 2;
	return MAPIROPS_ERR_SUCCESS;
}


/**
   \details Pull a uint16_t

   uint16_t is an unsigned char which values goes from 0 to 65535

   \param pull Pointer to the mapirops_pull structure
   \param v Pointer to the uint16_t value to return

   \return MAPIROPS_ERR_SUCCESS on success, otherwise MAPIROPS error
 */
enum mapirops_err_code mapirops_pull_uint16(struct mapirops_pull *pull, uint16_t *v)
{
	MAPIROPS_PULL_NEED_BYTES(pull, 2);
	*v = SVAL(pull->data.data, pull->offset);
	pull->offset += 2;
	return MAPIROPS_ERR_SUCCESS;
}


/**
   \details Push a int32_t

   int32_t is a signed long which values goes from -2147483647 to 2147483647

   \param push Pointer to the mapirops_push structure
   \param v int32_t value to push

   \return MAPIROPS_ERR_SUCCESS on success, otherwise MAPIROPS error
 */
enum mapirops_err_code mapirops_push_int32(struct mapirops_push *push, int32_t v)
{
	MAPIROPS_PUSH_NEED_BYTES(push, 4);
	SIVALS(push->data.data, push->offset, v);
	push->offset += 4;
	return MAPIROPS_ERR_SUCCESS;
}


/**
   \details Pull a int32_t

   int32_t is a signed long which values goes from -2147483647 to 2147483647

   \param pull Pointer to the mapirops_pull structure
   \param v Pointer to the int32_t value to return

   \return MAPIROPS_ERR_SUCCESS on success, otherwise MAPIROPS error
 */
enum mapirops_err_code mapirops_pull_int32(struct mapirops_pull *pull, int32_t *v)
{
	MAPIROPS_PULL_NEED_BYTES(pull, 4);
	*v = IVALS(pull->data.data, pull->offset);
	pull->offset += 4;
	return MAPIROPS_ERR_SUCCESS;
}

/**
   \details Push a uint32_t

   uint32_t is an unsigned long which values goes from 0 to 4294967295

   \param push Pointer to the mapirops_push structure
   \param v uint32_t value to push

   \return MAPIROPS_ERR_SUCCESS on success, otherwise MAPIROPS error
 */
enum mapirops_err_code mapirops_push_uint32(struct mapirops_push *push, uint32_t v)
{
	MAPIROPS_PUSH_NEED_BYTES(push, 4);
	SIVAL(push->data.data, push->offset, v);
	push->offset += 4;
	return MAPIROPS_ERR_SUCCESS;
}


/**
   \details Pull a uint32_t

   uint32_t is an unsigned long which values goes from 0 to 4294967295

   \param pull Pointer to the mapirops_pull structure
   \param v Pointer to the uint32_t value to return

   \return MAPIROPS_ERR_SUCCESS on success, otherwise MAPIROPS error
 */
enum mapirops_err_code mapirops_pull_uint32(struct mapirops_pull *pull, uint32_t *v)
{
	MAPIROPS_PULL_NEED_BYTES(pull, 4);
	*v = IVAL(pull->data.data, pull->offset);
	pull->offset += 4;
	return MAPIROPS_ERR_SUCCESS;
}


/**
   \details Push a int64_t

   int64_t is a signed long long int 

   \param push Pointer to the mapirops_push structure
   \param v the int64_t value to push

   \return MAPIROPS_ERR_SUCCESS on success, otherwise MAPIROPS error
 */
enum mapirops_err_code mapirops_push_int64(struct mapirops_push *push, int64_t v)
{
	MAPIROPS_PUSH_NEED_BYTES(push, 8);
	SIVAL(push->data.data, push->offset, (v & 0xFFFFFFFF));
	SIVAL(push->data.data, push->offset + 4, (v >> 32));
	push->offset += 8;
	return MAPIROPS_ERR_SUCCESS;
}


/**
   \details Pull a int64_t

   int64_t is a signed long long int

   \param pull Pointer to the mapirops_pull structure
   \param v Pointer to the int64_t value to return

   \return MAPIROPS_ERR_SUCCESS on success, otherwise MAPIROPS error
 */
enum mapirops_err_code mapirops_pull_int64(struct mapirops_pull *pull, int64_t *v)
{
	MAPIROPS_PULL_NEED_BYTES(pull, 8);
	*v = IVAL(pull->data.data, pull->offset);
	*v |= (int64_t)(IVAL(pull->data.data, pull->offset + 4)) << 32;
	pull->offset += 8;
	return MAPIROPS_ERR_SUCCESS;
}


/**
   \details Push a uint64_t

   uint64_t is an unsigned long long int

   \param push Pointer to the mapirops_push structure
   \param v uint64_t value to push

   \return MAPIROPS_ERR_SUCCESS on success, otherwise MAPIROPS error
 */
enum mapirops_err_code mapirops_push_uint64(struct mapirops_push *push, uint64_t v)
{
	MAPIROPS_PUSH_NEED_BYTES(push, 8);
	SIVAL(push->data.data, push->offset, (v & 0xFFFFFFFF));
	SIVAL(push->data.data, push->offset + 4, (v >> 32));
	push->offset += 8;
	return MAPIROPS_ERR_SUCCESS;
}

/**
   \details Pull a uint64_t

   uint64_t is an unsigned long long int

   \param pull Pointer to the mapirops_pull structure
   \param v Pointer to the uint64_t value to return

   \return MAPIROPS_ERR_SUCCESS on success, otherwise MAPIROPS error
 */
enum mapirops_err_code mapirops_pull_uint64(struct mapirops_pull *pull, uint64_t *v)
{
	MAPIROPS_PULL_NEED_BYTES(pull, 8);
	*v = IVAL(pull->data.data, pull->offset);
	*v |= (uint64_t)(IVAL(pull->data.data, pull->offset + 4)) << 32;
	pull->offset += 8;
	return MAPIROPS_ERR_SUCCESS;
}

/**
   \details Push a double

   double is a double-precision floating-point type

   \param push Pointer to the mapirops_push structure
   \param v double value to push

   \return MAPIROPS_ERR_SUCCESS on success, otherwise MAPIROPS error
 */
enum mapirops_err_code mapirops_push_double(struct mapirops_push *push, double v)
{
	MAPIROPS_PUSH_NEED_BYTES(push, 8);
	memcpy(push->data.data + push->offset, &v, 8);
	push->offset += 8;
	return MAPIROPS_ERR_SUCCESS;
}

/**
   \details Pull a double

   double is a double-precision floating-point type

   \param pull Pointer to the mapirops_pull structure
   \param v Pointer to the double value to return

   \return MAPIROPS_ERR_SUCCESS on success, otherwise MAPIROPS error
 */
enum mapirops_err_code mapirops_pull_double(struct mapirops_pull *pull, double *v)
{
	MAPIROPS_PULL_NEED_BYTES(pull, 8);
	memcpy(v, pull->data.data + pull->offset, 8);
	pull->offset += 8;
	return MAPIROPS_ERR_SUCCESS;
}

/**
   \details Push an ASCII string

   \param push Pointer to the mapirops_push structure
   \param flags Flags controlling how the string should be pushed
   \param str Pointer to the UTF-8 string to convert and push

   \return MAPIROPS_ERR_SUCCESS on success, otherwise MAPIROPS error
 */
enum mapirops_err_code mapirops_push_ascii_string(struct mapirops_push *push, int flags, char *str)
{
	TALLOC_CTX		*mem_ctx;
	size_t			ret;
	size_t			slen;
	size_t			inlen;
	size_t			outlen;
	enum mapirops_err_code	errcode;
	char			*ascii_str = NULL;
	char			*start = NULL;

	slen = str ? strlen(str) : 0;

	if (flags & MAPIROPS_STR_NOTERM) {
		flags &= ~MAPIROPS_STR_NOTERM;
	} else {
		slen += 1;
	}

	if (slen == 0) {
		str = NULL;
		return MAPIROPS_ERR_SUCCESS;
	}
        mem_ctx = talloc_named(NULL, 0, "mapirops_push_ascii");

        if ((str == NULL) || (strlen(str) == 0)) {
                start = talloc_strdup(mem_ctx, "\0");
        } else {
                inlen = slen;
                outlen = slen;

                ascii_str = (char *) talloc_array(mem_ctx, uint8_t, outlen);
                start = ascii_str;
                ret = iconv(push->utf8toascii, &str, &inlen, &ascii_str, &outlen);
                if (ret == (size_t)-1) {
                        talloc_free(mem_ctx);
                        return MAPIROPS_ERR_ICONV;
                }
        }
	errcode = mapirops_push_bytes(push, (const uint8_t *)start, slen);

	talloc_free(mem_ctx);

	return errcode;
}


/**
   \details Pull an ASCII string

   \param pull Pointer to the mapirops_pull structure
   \param mem_ctx Pointer to the memory context to use for string
   memory allocation
   \param flags Flags controlling how the string should be pulled
   \param str Pointer on pointer to the UTF-8 string to return
   \param slen Size of the UTF-8 string to pull

   \note Calling function is responsible for freeing the str allocated
   string returned

   \return MAPIROPS_ERR_SUCCESS on success, otherwise MAPIROPS error
 */
enum mapirops_err_code mapirops_pull_ascii_string(struct mapirops_pull *pull, TALLOC_CTX *mem_ctx, 
						  int flags, char **str, size_t slen)
{
	size_t			src_len = slen;

	/* If no prefixing size is available, calculate the size */
	if (flags & MAPIROPS_STR_NOSIZE) {
		/* We can't have STR_SIZE and STR_NOTERM flags set at the same time */
		if (flags & MAPIROPS_STR_NOTERM) {
			return MAPIROPS_ERR_INVALID_FLAGS;
		}
		/* TODO: Implement parsing routine looking for \0 */
		flags &= ~MAPIROPS_STR_NOSIZE;
		src_len = slen;
	}

	if (flags & MAPIROPS_STR_NOTERM) {
		flags &= ~MAPIROPS_STR_NOTERM;
	} else {
		/* Add termination character */
		src_len += 1;
	}

	if (flags) {
		return MAPIROPS_ERR_INVALID_FLAGS;
	}

	/* Empty string, nothing to pull */
	if (src_len == 0) {
		*str = NULL;
		return MAPIROPS_ERR_SUCCESS;
	}

	/* Ensure src_len is <= remaining buffer size */
	if (src_len >= (pull->data.length - pull->offset)) {
		return mapirops_error(MAPIROPS_ERR_BUFSIZE, LOG_ERR, 
				      "Overflow in pull_ascii to %u", src_len);
	}

	/* No iconv conversion required here: ASCII is a subset of UTF-8 */
        if (src_len > 0) {
            *str = talloc_strndup(mem_ctx, (const char *)pull->data.data + pull->offset, src_len);
            if (*str == NULL) {
                    return mapirops_error(MAPIROPS_ERR_ALLOC, LOG_ERR,
                                        "Failed to pull_ascii to %u", src_len);
            }
            pull->offset += src_len;
        } else {
            *str = NULL;
        }

	return MAPIROPS_ERR_SUCCESS;
}


/**
   \details Push an UTF-16 string

   \param push Pointer to the mapirops_push structure
   \param flags Flags controlling how the string should be pushed
   \param utf8_str Pointer to the UTF-8 string to convert and push

   \return MAPIROPS_ERR_SUCCESS on success, otherwise MAPIROPS error
 */
enum mapirops_err_code mapirops_push_utf16_string(struct mapirops_push *push, int flags, char *utf8_str)
{
	TALLOC_CTX		*mem_ctx;
	enum mapirops_err_code	errcode;
	size_t			ret;
	size_t			slen;
	size_t			inlen;
	size_t			outlen;
	size_t			dlen;
	char			*start = NULL;
	char			*utf16_str = NULL;

	slen = utf8_str ? strlen(utf8_str) : 0;
	if (flags & MAPIROPS_STR_NOTERM) {
		flags &= ~MAPIROPS_STR_NOTERM;
		dlen = slen * 2;
	} else {
		slen += 1;
		dlen = slen * 2;
	}

	inlen = slen;
	outlen = dlen;

	mem_ctx = talloc_named(NULL, 0, "mapirops_push_utf16");

	utf16_str = (char *) talloc_array(mem_ctx, uint8_t, outlen);
	start = utf16_str;

	ret = iconv(push->utf8to16, &utf8_str, &inlen, &utf16_str, &dlen);
	if (ret == (size_t)-1) {
		talloc_free(mem_ctx);
		return MAPIROPS_ERR_ICONV;
	}

	errcode = mapirops_push_bytes(push, (const uint8_t *)start, outlen);

	talloc_free(mem_ctx);

	return errcode;
}


/**
   \details Pull an UTF-16 string 

   \param pull Pointer to the mapirops_pull structure
   \param mem_ctx Pointer to the memory context to use for string memory allocation
   \param flags Flags controlling how the string should be pulled
   \param str Pointer on pointer to the UTF-8 converted string to return
   \param slen Size of the UTF-16 string to pull

   \note Calling function is responsible for freeing the str allocated string returned

   \return MAPIROPS_ERR_SUCCESS on success, otherwise MAPIROPS error
 */
enum mapirops_err_code mapirops_pull_utf16_string(struct mapirops_pull *pull, TALLOC_CTX *mem_ctx,
						  int flags, char **str, size_t slen)
{
	enum mapirops_err_code	errcode;
	size_t			ret;
	size_t			utf16_len = 0;
	char			*utf16_str = NULL;
	char			*utf8_str = NULL;
	size_t			utf8_len = 0;
	size_t			dlen = 0;
	char			*start = NULL;
	char			*start_utf16 = NULL;

	/* If no prefixing size is available, calculate the size */
	if (flags & MAPIROPS_STR_NOSIZE) {
		/* We can't have STR_SIZE and STR_NOTERM flags set at the same time */
		if (flags & MAPIROPS_STR_NOTERM) {
			return MAPIROPS_ERR_INVALID_FLAGS;
		}
		/* TODO: Implement parsing routine looking for \0\0 */
		flags &= ~ MAPIROPS_STR_NOSIZE;
	}

	if (flags & MAPIROPS_STR_NOTERM) {
		flags &= ~MAPIROPS_STR_NOTERM;
		utf16_len = slen;
	} else {
		utf16_len = slen + 2;
	}

	dlen = utf16_len / 2;

	/* Ensure utf16_len is <= remaining buffer size */
	if (utf16_len >= (pull->data.length - pull->offset)) {
		return mapirops_error(MAPIROPS_ERR_BUFSIZE, LOG_ERR,
				      "Overflow in pull_utf16 to %u", utf16_len);
	}

	/* Retrieve UTF-16 string from buffer */
	utf16_str = (char *) talloc_array(mem_ctx, uint8_t, utf16_len);
	start_utf16 = utf16_str;
	if (utf16_str == NULL) {
		return mapirops_error(MAPIROPS_ERR_ALLOC, LOG_ERR,
				      "Failed to pull_utf16 to %u", utf16_len);
	}
	errcode = mapirops_pull_bytes(pull, (uint8_t *)utf16_str, utf16_len);
	if (errcode != MAPIROPS_ERR_SUCCESS) {
		talloc_free(utf16_str);
		return errcode;
	}

	/* Allocate memory for str string to be returned */
	utf8_str = (char *) talloc_array(mem_ctx, uint8_t, dlen);
	start = utf8_str;
	utf8_len = dlen;
	if (utf8_str == NULL) {
		talloc_free(utf16_str);
		return mapirops_error(MAPIROPS_ERR_ALLOC, LOG_ERR, 
				      "Failed to pull_utf16 to %u", dlen);
	}

	/* iconv conversion required */
	ret = iconv(pull->utf16to8, &utf16_str, &utf16_len, &utf8_str, &utf8_len);
	if (ret == (size_t)-1) {
		talloc_free(utf8_str);
		talloc_free(utf16_str);
		return MAPIROPS_ERR_ICONV;
	}

	*str = talloc_strndup(mem_ctx, start, dlen);

	talloc_free(start);
	talloc_free(start_utf16);

	return MAPIROPS_ERR_SUCCESS;
}

/**
   \details Push a GUID structure

   \param push Pointer to mapirops_push structure
   \param guid Pointer to the GUID to push

   \return MAPIROPS_ERR_SUCCESS on success, otherwise MAPIROPS error
 */
enum mapirops_err_code mapirops_push_GUID(struct mapirops_push *push, const GUID *guid)
{
	MAPIROPS_CHECK(mapirops_push_uint32(push, guid->Data1));
	MAPIROPS_CHECK(mapirops_push_uint16(push, guid->Data2));
	MAPIROPS_CHECK(mapirops_push_uint16(push, guid->Data3));
	MAPIROPS_CHECK(mapirops_push_bytes(push, guid->Data4, 8));

	return MAPIROPS_ERR_SUCCESS;
}


/**
   \details Pull a GUID structure

   \param pull Pointer to the mapirops_pull structure
   \param guid Pointer to the GUID structure to return
   
   \return MAPIROPS_ERR_SUCCESS on success, otherwise MAPIROPS error
 */
enum mapirops_err_code mapirops_pull_GUID(struct mapirops_pull *pull, GUID *guid)
{
	MAPIROPS_CHECK(mapirops_pull_uint32(pull, &guid->Data1));
	MAPIROPS_CHECK(mapirops_pull_uint16(pull, &guid->Data2));
	MAPIROPS_CHECK(mapirops_pull_uint16(pull, &guid->Data3));
	MAPIROPS_CHECK(mapirops_pull_bytes(pull, (uint8_t *)guid->Data4, 8));

	return MAPIROPS_ERR_SUCCESS;
}


/**
   \details Push MAPISTATUS enumeration value
   
   \param push Pointer to the mapirops_push structure
   \param v MAPISTATUS enum value to push

   \return MAPIROPS_ERR_SUCCESS on success, otherwise MAPIROPS error
 */
enum mapirops_err_code mapirops_push_enum_MAPISTATUS(struct mapirops_push *push, enum MAPISTATUS r)
{
	MAPIROPS_CHECK(mapirops_push_uint32(push, r));
	return MAPIROPS_ERR_SUCCESS;		       
}


/**
   \details Pull MAPISTATUS enumeration value

   \parma pull Pointer to the mapirops_pull structure
   \param v Pointer to the MAPISTATUS enum value to return

   \return MAPIROPS_ERR_SUCCESS on success, otherwise MAPIROPS error
 */
enum mapirops_err_code mapirops_pull_enum_MAPISTATUS(struct mapirops_pull *pull, enum MAPISTATUS *r)
{
	MAPIROPS_CHECK(mapirops_pull_uint32(pull, r));
	return MAPIROPS_ERR_SUCCESS;
}
