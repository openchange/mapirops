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
   \file util.c
   \author Julien Kerihuel <j.kerihuel@openchange.org>
   \version 0.1
   \brief libmapirops utility functions
 */

#include "config.h"
#include "libmapirops.h"
#include "mapirops_uuid.h"

/**
   \details Return the number of bytes occupied by a buffer in ASCII
   format. The result includes the null termination limited by 'n'
   bytes.

   \param buf the ascii string
   \param n maximum size

   \note This function initially named ascii_len_n is copied from Samba
   lib/util/util.c file.

   \return Size of the ascii string
 */
size_t mapirops_ascii_len_n(const char *buf, size_t n)
{
	size_t	len;

	len = strnlen(buf, n);
	if ((len + 1) <= n) {
		len += 1;
	}
	return len;
}

/**

   \details Return the number of bytes occupied by a buffer in
   CH_UTF16 format.  The result includes the null termination

   \param buf the utf16 buffer

   \note This function initially named utf16_len is copied from Samba
   lib/util/util.c file.

   \return Size of the utf16 buffer
 */

size_t mapirops_utf16_len(const void *buf)
{
	size_t	len;

	for (len = 0; SVAL(buf, len); len += 2);
	return len + 2;
}


/**
   \details Return the number of bytes occupied by a buffer in
   CH_UTF16 format. The result includes the null termination limited
   by 'n' bytes.

   \param buf the utf16 buffer
   \param n maximum size

   \note This function initially named utf16_len_n is copied from
   Samba lib/util/util.c file.

   \return Size of the utf16 buffer
 */
size_t mapirops_utf16_len_n(const void *buf, size_t n)
{
	size_t	len;

	for (len = 0; ((len + 2) < n) && SVAL(buf, len); len += 2);
	if ((len + 2) <= n) {
		len += 2;
	}
	return len;
}
