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
   \file uuid.c
   \brief UUID utility functions
 */

#include "config.h"
#include "libmapirops.h"
#include  "mapirops_uuid.h"

/**
   \details Build a GUID structure from a string

   The initial code for this function is taken from Samba
   librpc/ndr/uuid.c. We are only interested in GUID string length
   equal to 36 ( XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX ) or 38 (
   {XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX})

   \param s Pointer to the string to convert
   \param guid Pointer to the GUID structure to return

   \return MAPIROPS_ERR_SUCCESS on success, otherwise MAPIROPS error
 */
enum mapirops_err_code mapirops_GUID_from_string(const char *s, GUID *guid)
{
	TALLOC_CTX		*mem_ctx;
	enum mapirops_err_code	retval = MAPIROPS_GENERIC_ERR;
	const char		*string;
	size_t			len = 0;

	if (s == NULL) {
		return MAPIROPS_ERR_BUFSIZE;
	}

	len = strlen(s);

	mem_ctx = talloc_new(NULL);
	if (mem_ctx == NULL) {
		return MAPIROPS_ERR_NO_MEMORY;
	}

	string = talloc_strndup(mem_ctx, s, len);
	if (string == NULL) {
		return MAPIROPS_ERR_NO_MEMORY;
	}

	switch (len) {
	case 36:
		if (11 == sscanf(string,
				 "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
				 &guid->Data1, (unsigned int *)&guid->Data2, 
				 (unsigned int *)&guid->Data3,
				 (unsigned int *)&guid->Data4[0], 
				 (unsigned int *)&guid->Data4[1],
				 (unsigned int *)&guid->Data4[2], 
				 (unsigned int *)&guid->Data4[3],
				 (unsigned int *)&guid->Data4[4], 
				 (unsigned int *)&guid->Data4[5],
				 (unsigned int *)&guid->Data4[6], 
				 (unsigned int *)&guid->Data4[7])) {
			retval = MAPIROPS_ERR_SUCCESS;
		}
		break;
	case 38:
		if (11 == sscanf(string,
				 "{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
				 &guid->Data1, (unsigned int *)&guid->Data2, 
				 (unsigned int *)&guid->Data3, 
				 (unsigned int *)&guid->Data4[0], 
				 (unsigned int *)&guid->Data4[1],
				 (unsigned int *)&guid->Data4[2], 
				 (unsigned int *)&guid->Data4[3], 
				 (unsigned int *)&guid->Data4[4], 
				 (unsigned int *)&guid->Data4[5], 
				 (unsigned int *)&guid->Data4[6], 
				 (unsigned int *)&guid->Data4[7])) {
			retval = MAPIROPS_ERR_SUCCESS;
		}
		break;
	}
	talloc_free(mem_ctx);
	return retval;
}


/**
   \details Convert a GUID structure into a string

   \param mem_ctx Pointer to the memory context
   \param guid Pointer to the GUID structure to convert

   \return Allocated GUID string on success, otherwise NULL
 */
char *mapirops_GUID_to_string(TALLOC_CTX *mem_ctx, GUID *guid)
{
	return talloc_asprintf(mem_ctx, 
			       "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
			       guid->Data1, guid->Data2, guid->Data3,
			       guid->Data4[0], guid->Data4[1],
			       guid->Data4[2], guid->Data4[3],
			       guid->Data4[4], guid->Data4[5],
			       guid->Data4[6], guid->Data4[7]);
}

