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
   \file mapirops_uuid.h
   \author Julien Kerihuel <j.kerihuel@openchange.org>
   \version 0.1
   \brief DCE UUID Microsoft implementation of GUID
 */

#ifndef	__MAPIROPS_UUID_H__
#define	__MAPIROPS_UUID_H__

#include <sys/types.h>

#include <talloc.h>

/**
   \struct _GUID
   \sa GUID
*/
/**
   \var GUID
   \brief DCE UUID Microsoft implementation of GUID
 */
typedef struct _GUID {
	uint32_t	Data1;		/*!< First 8 hexadecimal digits of the GUID */
	uint16_t	Data2;		/*!< First group of 4 hexadecimal digits */
	uint16_t	Data3;		/*!< Second group of 4 hexadecimal digits */ 
	uint8_t		Data4[8];	/*!< Array of 8 bytes. The first 2 bytes contain the 
					  third group of 4 hexadecimal digits. The remaining 
					  6 bytes contain the final 12 hexadecimal digits. */
} GUID;

/**
   \struct _MAPIGUID
   \sa MAPIGUID
 */
/**
   \var MAPIGUID
   \brief Flat GUID for MAPI service providers
 */
typedef struct _MAPIGUID {
	uint8_t		ab[16];		/*!< Array of 16 bytes containing flat GUID */
} MAPIGUID;

/** \cond */
#ifndef	__BEGIN_DECLS
#ifdef	__cplusplus
#define	__BEGIN_DECLS	extern "C" {
#define	__END_DECLS	}
#else
#define	__BEGIN_DECLS
#define	__END_DECLS
#endif
#endif

struct mapirops_push;
struct mapirops_pull;

/** \endcond */

__BEGIN_DECLS

/* The following definitions come from mapirops.c */
enum mapirops_err_code	mapirops_push_GUID(struct mapirops_push *, const GUID *);
enum mapirops_err_code	mapirops_pull_GUID(struct mapirops_pull *, GUID *);

/* The following definitions come from uuid.c */
enum mapirops_err_code mapirops_GUID_from_string(const char *, GUID *);
char *mapirops_GUID_to_string(TALLOC_CTX *, GUID *);

__END_DECLS

#endif /* ! __MAPIROPS_UUID_H__ */
