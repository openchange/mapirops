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
   \file libmapirops_private.h
   \author Julien Kerihuel <j.kerihuel@openchange.org>
   \version 0.1
   \brief libmapirops private data structures, enum and function
   definitions.
 */

#ifndef	__LIBMAPIROPS_PRIVATE_H__
#define	__LIBMAPIROPS_PRIVATE_H__

#include <sys/types.h>

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
/** \endcond */

__BEGIN_DECLS

/* The following definitions come from util.c */
size_t	mapirops_ascii_len_n(const char *, size_t);
size_t	mapirops_utf16_len(const void *);
size_t	mapirops_utf16_len_n(const void *, size_t);

__END_DECLS

#endif /* !__LIBMAPIROPS_PRIVATE_H__ */
