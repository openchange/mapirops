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

#include "testsuite.h"

START_TEST (test_push_init)
{
	TALLOC_CTX		*mem_ctx;
	struct mapirops_push	*push;

	mem_ctx = talloc_named(NULL, 0, "test_push_init");
	fail_if(mem_ctx == NULL);

	push = mapirops_push_init(mem_ctx);
	fail_if(push == NULL);

	talloc_free(mem_ctx);
}
END_TEST

START_TEST (test_pull_init)
{
	TALLOC_CTX		*mem_ctx;
	struct mapirops_pull	*pull;

	mem_ctx = talloc_named(NULL, 0, "test_pull_init");
	fail_if(mem_ctx == NULL);

	pull = mapirops_pull_init(mem_ctx);
	fail_if(pull == NULL);

	talloc_free(mem_ctx);
}
END_TEST

START_TEST (test_int8)
{
	COMMON_TEST_DECLARE(int8);
	COMMON_TEST_START(int8);
	in = 127;
	COMMON_TEST_CHECK(int8);
	fail_if(push->data.data[0] != 0x7F);
	COMMON_TEST_END();
}
END_TEST

START_TEST (test_uint8)
{
	COMMON_TEST_DECLARE(uint8);
	COMMON_TEST_START(uint8);
	in = 255;
	COMMON_TEST_CHECK(uint8);
	fail_if(push->data.data[0] != 0xFF);
	COMMON_TEST_END();
}
END_TEST

START_TEST (test_int16)
{
	COMMON_TEST_DECLARE(int16);
	COMMON_TEST_START(int16);
	in = 0xFFFE; /* FE FF on wire */
	COMMON_TEST_CHECK(int16);
	fail_if(push->data.data[0] != 0xFE);
	fail_if(push->data.data[1] != 0xFF);
	COMMON_TEST_END();
}
END_TEST

START_TEST (test_uint16)
{
	COMMON_TEST_DECLARE(uint16);
	COMMON_TEST_START(uint16);
	in = 0xFFFE;  /* FE FF on wire */
	COMMON_TEST_CHECK(uint16);
	fail_if(push->data.data[0] != 0xFE);
	fail_if(push->data.data[1] != 0xFF);
	COMMON_TEST_END();
}
END_TEST

START_TEST (test_int32)
{
	COMMON_TEST_DECLARE(int32);
	COMMON_TEST_START(int32);
	in = 0xAABBCCDD; /* DD CC BB AA on wire */
	COMMON_TEST_CHECK(int32);
	fail_if(push->data.data[0] != 0xDD);
	fail_if(push->data.data[1] != 0xCC);
	fail_if(push->data.data[2] != 0xBB);
	fail_if(push->data.data[3] != 0xAA);
	COMMON_TEST_END();
}
END_TEST

START_TEST (test_uint32)
{
	COMMON_TEST_DECLARE(uint32);
	COMMON_TEST_START(uint32);
	in = 0xCCDDEEFF; /* FF EE DD CC on wire */
	COMMON_TEST_CHECK(uint32);
	fail_if(push->data.data[0] != 0xFF);
	fail_if(push->data.data[1] != 0xEE);
	fail_if(push->data.data[2] != 0xDD);
	fail_if(push->data.data[3] != 0xCC);
	COMMON_TEST_END();
}
END_TEST

START_TEST (test_int64)
{
	COMMON_TEST_DECLARE(int64);
	COMMON_TEST_START(int64);
	in = 0x8899AABBCCDDEEFF; /* FF EE DD CC BB AA 99 88 on wire */
	COMMON_TEST_CHECK(int64);
	fail_if(push->data.data[0] != 0xFF);
	fail_if(push->data.data[1] != 0xEE);
	fail_if(push->data.data[2] != 0xDD);
	fail_if(push->data.data[3] != 0xCC);
	fail_if(push->data.data[4] != 0xBB);
	fail_if(push->data.data[5] != 0xAA);
	fail_if(push->data.data[6] != 0x99);
	fail_if(push->data.data[7] != 0x88);
	COMMON_TEST_END();
}
END_TEST

START_TEST (test_uint64)
{
	COMMON_TEST_DECLARE(uint64);
	COMMON_TEST_START(uint64);
	in = 0xFFFFAABBCCDDEEFF; /* FF EE DD CC BB AA FF FF on wire */
	COMMON_TEST_CHECK(uint64);
	fail_if(push->data.data[0] != 0xFF);
	fail_if(push->data.data[1] != 0xEE);
	fail_if(push->data.data[2] != 0xDD);
	fail_if(push->data.data[3] != 0xCC);
	fail_if(push->data.data[4] != 0xBB);
	fail_if(push->data.data[5] != 0xAA);
	fail_if(push->data.data[6] != 0xFF);
	fail_if(push->data.data[7] != 0xFF);
	COMMON_TEST_END();
}
END_TEST

#define double_t	double
START_TEST (test_double)
{
	COMMON_TEST_DECLARE(double);
	COMMON_TEST_START(double);
	in = 3.1415926;
	COMMON_TEST_CHECK(double);
	fail_if(push->data.data[0] != 0x4A);
	fail_if(push->data.data[1] != 0xD8);
	fail_if(push->data.data[2] != 0x12);
	fail_if(push->data.data[3] != 0x4D);
	fail_if(push->data.data[4] != 0xFB);
	fail_if(push->data.data[5] != 0x21);
	fail_if(push->data.data[6] != 0x09);
	fail_if(push->data.data[7] != 0x40);
	COMMON_TEST_END();
}
END_TEST

START_TEST (test_ascii)
{
	TALLOC_CTX		*mem_ctx;
	enum mapirops_err_code	errval;
	struct mapirops_push	*push;
	struct mapirops_pull	*pull;
	char			*in = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char			*out = NULL;

	COMMON_TEST_START(ascii);

	errval = mapirops_push_ascii_string(push, 0, in);
	fail_if(errval != MAPIROPS_ERR_SUCCESS);
	fail_if(push->offset != (strlen(in) + 1));
	
	pull->data = push->data;
	errval = mapirops_pull_ascii_string(pull, mem_ctx, 0, &out, strlen(in));
	fail_if(errval != MAPIROPS_ERR_SUCCESS);
	fail_if(strcmp(in, out));

	COMMON_TEST_END();
}
END_TEST

START_TEST (test_ascii_noterm)
{
	TALLOC_CTX		*mem_ctx;
	enum mapirops_err_code	errval;
	struct mapirops_push	*push;
	struct mapirops_pull	*pull;
	char			*in = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char			*out = NULL;

	COMMON_TEST_START(ascii_noterm);
		
	errval = mapirops_push_ascii_string(push, MAPIROPS_STR_NOTERM, in);
	fail_if(errval != MAPIROPS_ERR_SUCCESS);
	fail_if(push->offset != strlen(in));

	pull->data = push->data;
	errval = mapirops_pull_ascii_string(pull, mem_ctx, MAPIROPS_STR_NOTERM, 
					    &out, strlen(in));
	fail_if(errval != MAPIROPS_ERR_SUCCESS);
	fail_if(strcmp(in, out));

	COMMON_TEST_END();
}
END_TEST

START_TEST (test_utf16)
{
	TALLOC_CTX		*mem_ctx;
	enum mapirops_err_code	errval;
	struct mapirops_push	*push;
	struct mapirops_pull	*pull;
	char			*in = "ABCDE";
	char			*out = NULL;
	size_t			utf16len = 0;

	COMMON_TEST_START(utf16);

	utf16len = strlen(in) * 2;

	errval = mapirops_push_utf16_string(push, 0, in);
	fail_if(errval != MAPIROPS_ERR_SUCCESS);
	fail_if(push->offset != (utf16len + 2));

	pull->data = push->data;
	errval = mapirops_pull_utf16_string(pull, mem_ctx, 0, 
					    &out, strlen(in) * 2);
	fail_if(errval != MAPIROPS_ERR_SUCCESS);
	fail_if(strcmp(in, out));

	COMMON_TEST_END();
}
END_TEST

START_TEST (test_utf16_noterm)
{
	TALLOC_CTX		*mem_ctx;
	enum mapirops_err_code	errval;
	struct mapirops_push	*push;
	struct mapirops_pull	*pull;
	char			*in = "ABCDE";
	char			*out = NULL;

	COMMON_TEST_START(utf16_noterm);

	errval = mapirops_push_utf16_string(push, MAPIROPS_STR_NOTERM, in);
	fail_if(errval != MAPIROPS_ERR_SUCCESS);
	fail_if(push->offset != (strlen(in) * 2));

	pull->data = push->data;
	errval = mapirops_pull_utf16_string(pull, mem_ctx, MAPIROPS_STR_NOTERM, 
					    &out, strlen(in) * 2);
	fail_if(errval != MAPIROPS_ERR_SUCCESS);
	fail_if(strcmp(in, out));

	COMMON_TEST_END();
}
END_TEST

START_TEST (test_bytes)
{
	TALLOC_CTX		*mem_ctx;
	enum mapirops_err_code	errval;
	struct mapirops_push	*push;
	struct mapirops_pull	*pull;
	uint8_t			*in;
	uint8_t			*out = NULL;
	uint32_t		u32out = 0;

	COMMON_TEST_START(test_bytes);

#define	BYTE_TEST_STR	"OpenChange Project"

	in = talloc_array(mem_ctx, uint8_t, strlen(BYTE_TEST_STR));
	fail_if(in == NULL);
	memcpy(in, BYTE_TEST_STR, strlen(BYTE_TEST_STR));

	errval = mapirops_push_bytes(push, in, strlen(BYTE_TEST_STR));
	fail_if(errval != MAPIROPS_ERR_SUCCESS);
	fail_if(push->offset != strlen(BYTE_TEST_STR));

	pull->data = push->data;
	out = talloc_array(mem_ctx, uint8_t, strlen(BYTE_TEST_STR));
	errval = mapirops_pull_bytes(pull, out, strlen(BYTE_TEST_STR));
	fail_if(errval != MAPIROPS_ERR_SUCCESS);
	fail_if(memcmp(in, out, strlen(BYTE_TEST_STR)));
	fail_if(memcmp(out, BYTE_TEST_STR, strlen(BYTE_TEST_STR)));

	COMMON_TEST_END();
}
END_TEST

START_TEST (test_GUID)
{
	TALLOC_CTX		*mem_ctx;
	enum mapirops_err_code	errval;
	struct mapirops_push	*push;
	struct mapirops_pull	*pull;
	GUID			in;
	GUID			out;
	char			*string;

#define TEST_PS_MAPI_36	"00020328-0000-0000-c000-000000000046"
#define TEST_PS_MAPI_38	"{00020328-0000-0000-c000-000000000046}"

	/* GUID 36 */
	{
		COMMON_TEST_START(test_guid);

		errval = mapirops_GUID_from_string(TEST_PS_MAPI_36, &in);
		fail_if(errval != MAPIROPS_ERR_SUCCESS);

		errval = mapirops_push_GUID(push, &in);
		fail_if(errval != MAPIROPS_ERR_SUCCESS);
		fail_if(push->offset != sizeof(GUID));

		pull->data = push->data;
		errval = mapirops_pull_GUID(pull, &out);
		fail_if(errval != MAPIROPS_ERR_SUCCESS);
		fail_if(pull->offset != sizeof(GUID));

		fail_if(in.Data1 != out.Data1);
		fail_if(in.Data2 != out.Data2);
		fail_if(in.Data3 != out.Data3);
		fail_if(memcmp(in.Data4, out.Data4, 8));
	
		COMMON_TEST_END();
	}

	/* GUID 38 */
	{
		COMMON_TEST_START(test_guid);

		errval = mapirops_GUID_from_string(TEST_PS_MAPI_38, &in);
		fail_if(errval != MAPIROPS_ERR_SUCCESS);

		errval = mapirops_push_GUID(push, &in);
		fail_if(errval != MAPIROPS_ERR_SUCCESS);
		fail_if(push->offset != sizeof(GUID));

		pull->data = push->data;
		errval = mapirops_pull_GUID(pull, &out);
		fail_if(errval != MAPIROPS_ERR_SUCCESS);
		fail_if(pull->offset != sizeof(GUID));

		fail_if(in.Data1 != out.Data1);
		fail_if(in.Data2 != out.Data2);
		fail_if(in.Data3 != out.Data3);
		fail_if(memcmp(in.Data4, out.Data4, 8));
	
		COMMON_TEST_END();

	}
}
END_TEST

START_TEST (test_MAPISTATUS)
{
	TALLOC_CTX		*mem_ctx;
	enum mapirops_err_code	errval;
	struct mapirops_push	*push;
	struct mapirops_pull	*pull;
	enum MAPISTATUS		out;
	
	COMMON_TEST_START(test_MAPISTATUS);
	errval = mapirops_push_enum_MAPISTATUS(push, MAPI_E_NO_ACCESS);
	fail_if(errval != MAPIROPS_ERR_SUCCESS);
	
	pull->data = push->data;
	errval = mapirops_pull_enum_MAPISTATUS(pull, &out);
	fail_if(errval != MAPIROPS_ERR_SUCCESS);
	fail_if(pull->offset != sizeof(enum MAPISTATUS));
	fail_if(out != MAPI_E_NO_ACCESS);

        COMMON_TEST_END();
}
END_TEST

static Suite *primitives_suite(void)
{
	Suite	*s;
	TCase	*tc;
	
	s = suite_create("MAPIROPS buffer stack");
	tc = tcase_create("[OC-TYPE]");
	suite_add_tcase(s, tc);
	tcase_add_test(tc, test_push_init);
	tcase_add_test(tc, test_pull_init);
	tcase_add_test(tc, test_int8);
	tcase_add_test(tc, test_uint8);
	tcase_add_test(tc, test_int16);
	tcase_add_test(tc, test_uint16);
	tcase_add_test(tc, test_int32);
	tcase_add_test(tc, test_uint32);
	tcase_add_test(tc, test_int64);
	tcase_add_test(tc, test_uint64);
	tcase_add_test(tc, test_double);
	tcase_add_test(tc, test_ascii);
	tcase_add_test(tc, test_ascii_noterm);
	tcase_add_test(tc, test_utf16);
	tcase_add_test(tc, test_utf16_noterm);
	tcase_add_test(tc, test_bytes);
	tcase_add_test(tc, test_GUID);
	tcase_add_test(tc, test_MAPISTATUS);

	return s;
}

static void testsuite_print_references(void)
{
	printf("Microsoft Open Specifications:\n");
	oxcstor_suite_references();
}

int main(int argc, const char *argv[])
{
	poptContext	pc;
	int		opt;
	int		nf;
	Suite		*s;
	Suite		*oxcstor;
	SRunner		*sr;

	enum { OPT_REFS=1000 };
	struct poptOption	long_options[] = {
		POPT_AUTOHELP
		{"version", 0, POPT_ARG_NONE, NULL, OPT_REFS, "List of Microsoft Open Specification used", NULL},
		{ NULL, 0, 0, NULL, 0, NULL, NULL }
	};

	pc = poptGetContext("mapirops_testsuite", argc, argv, long_options, 0);
	while ((opt = poptGetNextOpt(pc)) != -1) {
		switch (opt) {
		case OPT_REFS:
			testsuite_print_references();
                        poptFreeContext(pc);
			exit(0);
			break;
		default:
			break;
		}
	}

	poptFreeContext(pc);

	s = primitives_suite();
	sr = srunner_create(s);

	oxcstor = oxcstor_suite();
	srunner_add_suite(sr, oxcstor);

	srunner_run_all(sr, CK_NORMAL);
	nf = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
