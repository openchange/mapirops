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

#ifndef	__TESTSUITE_H__
#define	__TESTSUITE_H__

#include "libmapirops.h"

#include <popt.h>
#include <check.h>

/** \cond */

#define	COMMON_TEST_DECLARE(t)			\
	TALLOC_CTX		*mem_ctx;	\
	enum mapirops_err_code	errval;		\
	struct mapirops_push	*push;		\
	struct mapirops_pull	*pull;		\
	t##_t			in = 0;		\
	t##_t			out = 0;

#define	COMMON_TEST_START(t)				\
	mem_ctx = talloc_named(NULL, 0, "test_##t");	\
	fail_if(mem_ctx == NULL);			\
	push = mapirops_push_init(mem_ctx);		\
	fail_if(push == NULL);				\
	pull = mapirops_pull_init(mem_ctx);		\
	fail_if(pull == NULL);

#define	COMMON_TEST_CHECK(t)				\
	errval = mapirops_push_##t (push, in);		\
	fail_if(errval != MAPIROPS_ERR_SUCCESS);	\
	fail_if(push->offset != sizeof(t##_t));		\
	pull->data = push->data;			\
	errval = mapirops_pull_##t (pull, &out);	\
	fail_if(errval != MAPIROPS_ERR_SUCCESS);	\
	fail_if(in != out);

#define	COMMON_TEST_END()				\
	talloc_free(mem_ctx);

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
Suite *oxcstor_suite(void);
void oxcstor_suite_references(void);
__END_DECLS

#endif /*! __TESTSUITE_H__ */
