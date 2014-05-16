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

START_TEST (test_RopLogon_LogonFlags)
{
	TALLOC_CTX		*mem_ctx;
	enum mapirops_err_code	errval;
	struct mapirops_push	*push;
	struct mapirops_pull	*pull;
	enum LogonFlags		r;
	

	COMMON_TEST_START(RopLogon_LogonFlags);

	/* Test correct combination of flags */
	errval = mapirops_push_enum_LogonFlags(push, 0); 
	fail_if(!MAPIROPS_ERR_CODE_IS_SUCCESS(errval));
	errval = mapirops_push_enum_LogonFlags(push, LogonFlags_LogonPrivate); 
	fail_if(!MAPIROPS_ERR_CODE_IS_SUCCESS(errval));
	errval = mapirops_push_enum_LogonFlags(push, LogonFlags_UnderCover); 
	fail_if(!MAPIROPS_ERR_CODE_IS_SUCCESS(errval));
	errval = mapirops_push_enum_LogonFlags(push, LogonFlags_Ghosted); 
	fail_if(!MAPIROPS_ERR_CODE_IS_SUCCESS(errval));
	errval = mapirops_push_enum_LogonFlags(push, LogonFlags_LogonPrivate|LogonFlags_UnderCover); 
	fail_if(!MAPIROPS_ERR_CODE_IS_SUCCESS(errval));
	errval = mapirops_push_enum_LogonFlags(push, LogonFlags_Ghosted|LogonFlags_UnderCover); 
	fail_if(!MAPIROPS_ERR_CODE_IS_SUCCESS(errval));
	
	pull->data = push->data;

	/* Test combination of flags for pull operation */
	errval = mapirops_pull_enum_LogonFlags(pull, &r);
	fail_if(!MAPIROPS_ERR_CODE_IS_SUCCESS(errval) || r != 0);
	errval = mapirops_pull_enum_LogonFlags(pull, &r);
	fail_if(!MAPIROPS_ERR_CODE_IS_SUCCESS(errval) || r != LogonFlags_LogonPrivate);
	errval = mapirops_pull_enum_LogonFlags(pull, &r);
	fail_if(!MAPIROPS_ERR_CODE_IS_SUCCESS(errval) || r != LogonFlags_UnderCover);
	errval = mapirops_pull_enum_LogonFlags(pull, &r);
	fail_if(!MAPIROPS_ERR_CODE_IS_SUCCESS(errval) || r != LogonFlags_Ghosted);
	errval = mapirops_pull_enum_LogonFlags(pull, &r);
	fail_if(!MAPIROPS_ERR_CODE_IS_SUCCESS(errval) || r != (LogonFlags_LogonPrivate|LogonFlags_UnderCover));
	errval = mapirops_pull_enum_LogonFlags(pull, &r);
	fail_if(!MAPIROPS_ERR_CODE_IS_SUCCESS(errval) || r != (LogonFlags_Ghosted|LogonFlags_UnderCover));
	fail_if(pull->offset != push->offset);

}
END_TEST

START_TEST (test_RopLogon_OpenFlags)
{
	TALLOC_CTX		*mem_ctx;
	enum mapirops_err_code	errval;
	struct mapirops_push	*push;
	struct mapirops_pull	*pull;
	uint32_t		r;

	COMMON_TEST_START(RopLogon_OpenFlags);

	/* Test correct combination of flags */
	errval = mapirops_push_enum_OpenFlags(push, OpenFlags_USE_ADMIN_PRIVILEGE);
	fail_if(!MAPIROPS_ERR_CODE_IS_SUCCESS(errval));
	errval = mapirops_push_enum_OpenFlags(push, OpenFlags_PUBLIC);
	fail_if(!MAPIROPS_ERR_CODE_IS_SUCCESS(errval));
	errval = mapirops_push_enum_OpenFlags(push, OpenFlags_HOME_LOGON|OpenFlags_TAKE_OWNERSHIP);
	fail_if(!MAPIROPS_ERR_CODE_IS_SUCCESS(errval));
	errval = mapirops_push_enum_OpenFlags(push, OpenFlags_ALTERNATE_SERVER);
	fail_if(!MAPIROPS_ERR_CODE_IS_SUCCESS(errval));
	errval = mapirops_push_enum_OpenFlags(push, OpenFlags_IGNORE_HOME_MDB);
	fail_if(!MAPIROPS_ERR_CODE_IS_SUCCESS(errval));
	errval = mapirops_push_enum_OpenFlags(push, OpenFlags_NO_MAIL);
	fail_if(!MAPIROPS_ERR_CODE_IS_SUCCESS(errval));
	errval = mapirops_push_enum_OpenFlags(push, OpenFlags_USE_PER_MDB_REPLID_MAPPING);
	fail_if(!MAPIROPS_ERR_CODE_IS_SUCCESS(errval));
	errval = mapirops_push_enum_OpenFlags(push, OpenFlags_SUPPORT_PROGRESS);
	fail_if(!MAPIROPS_ERR_CODE_IS_SUCCESS(errval));

	/* Push invalid flags */
	errval = mapirops_push_enum_OpenFlags(push, 0xdeadbeef);
	fail_if(MAPIROPS_ERR_CODE_IS_SUCCESS(errval));

	pull->data = push->data;

	/* Test combination of flags for pull operation */
	errval = mapirops_pull_enum_OpenFlags (pull, &r);
	fail_if(!MAPIROPS_ERR_CODE_IS_SUCCESS(errval) || r != OpenFlags_USE_ADMIN_PRIVILEGE);
	errval = mapirops_pull_enum_OpenFlags (pull, &r);
	fail_if(!MAPIROPS_ERR_CODE_IS_SUCCESS(errval) || r != OpenFlags_PUBLIC);
	errval = mapirops_pull_enum_OpenFlags (pull, &r);
	fail_if(!MAPIROPS_ERR_CODE_IS_SUCCESS(errval) || r != (OpenFlags_HOME_LOGON|OpenFlags_TAKE_OWNERSHIP));
	errval = mapirops_pull_enum_OpenFlags (pull, &r);
	fail_if(!MAPIROPS_ERR_CODE_IS_SUCCESS(errval) || r != OpenFlags_ALTERNATE_SERVER);
	errval = mapirops_pull_enum_OpenFlags (pull, &r);
	fail_if(!MAPIROPS_ERR_CODE_IS_SUCCESS(errval) || r != OpenFlags_IGNORE_HOME_MDB);
	errval = mapirops_pull_enum_OpenFlags (pull, &r);
	fail_if(!MAPIROPS_ERR_CODE_IS_SUCCESS(errval) || r != OpenFlags_NO_MAIL);
	errval = mapirops_pull_enum_OpenFlags (pull, &r);
	fail_if(!MAPIROPS_ERR_CODE_IS_SUCCESS(errval) || r != OpenFlags_USE_PER_MDB_REPLID_MAPPING);
	errval = mapirops_pull_enum_OpenFlags (pull, &r);
	fail_if(!MAPIROPS_ERR_CODE_IS_SUCCESS(errval) || r != OpenFlags_SUPPORT_PROGRESS);
}
END_TEST

START_TEST (test_RopLogon_request)
{
	TALLOC_CTX		*mem_ctx;
	enum mapirops_err_code	errval;
	struct mapirops_push	*push;
	struct mapirops_pull	*pull;
	struct RopLogon_request	request;
	struct RopLogon_request	orequest;

#define	MAILBOX_STR	"/o=First Organization/ou=First Administrative Group/cn=Recipients/cn=test"

	request.RopId = RopLogon;
	request.LogonId = 0x1;
	request.OutputHandleIndex = 0x0;
	request.LogonFlags = LogonFlags_LogonPrivate|LogonFlags_UnderCover;
	request.OpenFlags = OpenFlags_USE_PER_MDB_REPLID_MAPPING;
	request.StoreState = 0x00000000;
	request.EssDnSize = strlen(MAILBOX_STR) + 1;
	request.EssDn = MAILBOX_STR;

	/* Test common RopLogon request */
	{
		COMMON_TEST_START(RopLogon_request);

		errval = mapirops_push_struct_RopLogon_request(push, &request);
		fail_if(errval != MAPIROPS_ERR_SUCCESS);

		pull->data = push->data;
		
		errval = mapirops_pull_struct_RopLogon_request(pull, &orequest);
		fail_if(errval != MAPIROPS_ERR_SUCCESS);
		fail_if(request.LogonId != orequest.LogonId);
		fail_if(request.OutputHandleIndex != orequest.OutputHandleIndex);
		fail_if(request.LogonFlags != orequest.LogonFlags);
		fail_if(request.OpenFlags != orequest.OpenFlags);
		fail_if(request.StoreState != orequest.StoreState);
		fail_if(request.EssDnSize != orequest.EssDnSize);
		fail_if(strcmp(request.EssDn, orequest.EssDn));

		talloc_free(mem_ctx);
	}

	/* Test with EssDnSize == 0x0 */
	{
		COMMON_TEST_START(RopLogon_request);

		request.EssDnSize = 0x0;
		request.EssDn = NULL;

		errval = mapirops_push_struct_RopLogon_request(push, &request);
		fail_if(errval != MAPIROPS_ERR_SUCCESS);

		pull->data = push->data;
		errval = mapirops_pull_struct_RopLogon_request(pull, &orequest);
		fail_if(errval != MAPIROPS_ERR_SUCCESS);
		fail_if(request.LogonId != orequest.LogonId);
		fail_if(request.OutputHandleIndex != orequest.OutputHandleIndex);
		fail_if(request.LogonFlags != orequest.LogonFlags);
		fail_if(request.OpenFlags != orequest.OpenFlags);
		fail_if(request.StoreState != orequest.StoreState);
		fail_if(request.EssDnSize != orequest.EssDnSize);

		talloc_free(mem_ctx);
	}


	/* Test with EssDn == "" */
	{
		COMMON_TEST_START(RopLogon_request);

		request.EssDn = "";
		request.EssDnSize = strlen(request.EssDn) + 1;

		errval = mapirops_push_struct_RopLogon_request(push, &request);
		fail_if(errval != MAPIROPS_ERR_SUCCESS);

		pull->data = push->data;
		errval = mapirops_pull_struct_RopLogon_request(pull, &orequest);
		fail_if(errval != MAPIROPS_ERR_SUCCESS);
		fail_if(request.LogonId != orequest.LogonId);
		fail_if(request.OutputHandleIndex != orequest.OutputHandleIndex);
		fail_if(request.LogonFlags != orequest.LogonFlags);
		fail_if(request.OpenFlags != orequest.OpenFlags);
		fail_if(request.StoreState != orequest.StoreState);
		fail_if(request.EssDnSize != orequest.EssDnSize);

		talloc_free(mem_ctx);
	}
}
END_TEST

START_TEST (test_RopLogon_LogonTime)
{
	TALLOC_CTX		*mem_ctx;
	enum mapirops_err_code	errval;
	struct mapirops_push	*push;
	struct mapirops_pull	*pull;
	struct LogonTime	LogonTime;
	struct LogonTime	oLogonTime;

	LogonTime.Seconds = 15;
	LogonTime.Minutes = 31;
	LogonTime.Hours = 20;
	LogonTime.DayOfWeek = DayOfWeek_Tuesday;
	LogonTime.Day = 28;
	LogonTime.CurrentMonth = CurrentMonth_August;
	LogonTime.Year = 2012;

	/* Test valid LogonTime */
	{
		COMMON_TEST_START(RopLogon_LogonTime);
		errval = mapirops_push_struct_LogonTime(push, &LogonTime);
		fail_if(errval != MAPIROPS_ERR_SUCCESS);

		pull->data = push->data;

		errval = mapirops_pull_struct_LogonTime(pull, &oLogonTime);
		fail_if(errval != MAPIROPS_ERR_SUCCESS);

		fail_if(LogonTime.Seconds != oLogonTime.Seconds);
		fail_if(LogonTime.Minutes != oLogonTime.Minutes);
		fail_if(LogonTime.Hours != oLogonTime.Hours);
		fail_if(LogonTime.DayOfWeek != oLogonTime.DayOfWeek);
		fail_if(LogonTime.Day != oLogonTime.Day);
		fail_if(LogonTime.CurrentMonth != oLogonTime.CurrentMonth);
		fail_if(LogonTime.Year != oLogonTime.Year);
		talloc_free(mem_ctx);
	}

	/* Test invalid LogonTime DayOfWeek */
	{
		COMMON_TEST_START(RopLogon_LogonTime);
		LogonTime.DayOfWeek = 9;
		errval = mapirops_push_struct_LogonTime(push, &LogonTime);
		fail_if(errval != MAPIROPS_ERR_INVALID_VAL);
		talloc_free(mem_ctx);
	}

	/* Test invalid LogonTime Month */
	{
		COMMON_TEST_START(RopLogon_LogonTime);
		LogonTime.DayOfWeek = DayOfWeek_Tuesday;
		LogonTime.CurrentMonth = 14;
		errval = mapirops_push_struct_LogonTime(push, &LogonTime);
		fail_if(errval != MAPIROPS_ERR_INVALID_VAL);
		talloc_free(mem_ctx);
	}

}
END_TEST

START_TEST (test_RopLogon_response_OK)
{
	TALLOC_CTX			*mem_ctx;
	enum mapirops_err_code		errval;
	struct mapirops_push		*push;
	struct mapirops_pull		*pull;
	struct RopLogon_response	response;
	struct RopLogon_response	oresponse;

	/* Mailbox response */
	{
		COMMON_TEST_START(RopLogon_response);
	}

	/* Public folders response */
}
END_TEST

START_TEST (test_RopLogon_response_Failure)
{

}
END_TEST

START_TEST (test_RopLogon_response_Redirect)
{

}
END_TEST

Suite *oxcstor_suite(void)
{
	Suite	*s;
	TCase	*TRopLogon;

	s = suite_create("[MS-OXCSTOR] ROPS");
	TRopLogon = tcase_create("[MS-OXCSTOR] RopLogon");

	suite_add_tcase(s, TRopLogon);
	tcase_add_test(TRopLogon, test_RopLogon_LogonFlags);
	tcase_add_test(TRopLogon, test_RopLogon_OpenFlags);
	tcase_add_test(TRopLogon, test_RopLogon_request);
	tcase_add_test(TRopLogon, test_RopLogon_LogonTime);
	tcase_add_test(TRopLogon, test_RopLogon_request);
	tcase_add_test(TRopLogon, test_RopLogon_response_OK);
	tcase_add_test(TRopLogon, test_RopLogon_response_Failure);
	tcase_add_test(TRopLogon, test_RopLogon_response_Redirect);
	return s;
}

void oxcstor_suite_references(void)
{
	printf("\t* [MS-OXCSTOR] %10s, %s, %s (%s)\n",
	       OXCSTOR_REVISION, OXCSTOR_VERSION, 
	       OXCSTOR_DATE, OXCSTOR_DESCRIPTION);
}
