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
   \file libmapirops.h
   \author Julien Kerihuel <j.kerihuel@openchange.org>
   \version 0.1
   \brief libmapirops data structures, enum and function definitions
 */

#ifndef	__LIBMAPIROPS_H__
#define	__LIBMAPIROPS_H__

#include <sys/types.h>
#include <sys/param.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <syslog.h>
#include <string.h>
#include <iconv.h>
#include <ctype.h>

#include <talloc.h>

#include <mapirops_uuid.h>
#include <mapistatus.h>
struct mapirops_push;
struct mapirops_pull;
#include <oxcstor.h>

/**
   \enum RopId
   \brief Define the different MAPI rops identifiers
 */
enum RopId {
	RopRelease					=	(int)(0x01),
	RopOpenFolder					=	(int)(0x02),
	RopOpenMessage					=	(int)(0x03),
	RopGetHierarchyTable				=	(int)(0x04),
	RopGetContentsTable				=	(int)(0x05),
	RopCreateMessage				=	(int)(0x06),
	RopGetPropertiesSpecific			=	(int)(0x07),
	RopGetPropertiesAll				=	(int)(0x08),
	RopGetPropertiesList				=	(int)(0x09),
	RopSetProperties				=	(int)(0x0A),
	RopDeleteProperties				=	(int)(0x0B),
	RopSaveChangesMessage				=	(int)(0x0C),
	RopRemoveAllRecipients				=	(int)(0x0D),
	RopModifyRecipients				=	(int)(0x0E),
	RopReadRecipients				=	(int)(0x0F),
	RopReloadCachedInformation			=	(int)(0x10),
	RopSetMessageReadFlag				=	(int)(0x11),
	RopSetColumns					=	(int)(0x12),
	RopSortTable					=	(int)(0x13),
	RopRestrict					=	(int)(0x14),
	RopQueryRows					=	(int)(0x15),
	RopGetStatus					=	(int)(0x16),
	RopQueryPosition				=	(int)(0x17),
	RopSeekRow					=	(int)(0x18),
	RopSeekRowBookmark				=	(int)(0x19),
	RopSeekRowFractional				=	(int)(0x1A),
	RopCreateBookmark				=	(int)(0x1B),
	RopCreateFolder					=	(int)(0x1C),
	RopDeleteFolder					=	(int)(0x1D),
	RopDeleteMessages				=	(int)(0x1E),
	RopGetMessageStatus				=	(int)(0x1F),
	RopSetMessageStatus				=	(int)(0x20),
	RopGetAttachmentTable				=	(int)(0x21),
	RopOpenAttachment				=	(int)(0x22),
	RopCreateAttachment				=	(int)(0x23),
	RopDeleteAttachment				=	(int)(0x24),
	RopSaveChangesAttachment			=	(int)(0x25),
	RopSetReceiveFolder				=	(int)(0x26),
	RopGetReceiveFolder				=	(int)(0x27),
	/* Reserved					=	(int)(0x28) */
	RopRegisterNotification				=	(int)(0x29),
	RopNotify					=	(int)(0x2A),
	RopOpenStream					=	(int)(0x2B),
	RopReadStream					=	(int)(0x2C),
	RopWriteStream					=	(int)(0x2D),
	RopSeekStream					=	(int)(0x2E),
	RopSetStreamSize				=	(int)(0x2F),
	RopSetSearchCriteria				=	(int)(0x30),
	RopGetSearchCriteria				=	(int)(0x31),
	RopSubmitMessage				=	(int)(0x32),
	RopMoveCopyMessages				=	(int)(0x33),
	RopAbortSubmit					=	(int)(0x34),
	RopMoveFolder					=	(int)(0x35),
	RopCopyFolder					=	(int)(0x36),
	RopQueryColumnsAll				=	(int)(0x37),
	RopAbort					=	(int)(0x38),
	RopCopyTo					=	(int)(0x39),
	RopCopyToStream					=	(int)(0x3A),
	RopCLoneStream					=	(int)(0x3B),
	/* Reserved					=	(int)(0x3C) - (0x3D) */
	RopGetPermissionsTable				=	(int)(0x3E),
	RopGetRulesTable				=	(int)(0x3F),
	RopModifyPermissions				=	(int)(0x40),
	RopModifyRules					=	(int)(0x41),
	RopGetOwningServers				=	(int)(0x42),
	RopLongTermIdFromId				=	(int)(0x43),
	RopIdFromLongTermId				=	(int)(0x44),
	RopPublicFolderIsGhosted			=	(int)(0x45),
	RopOpenEmbeddedMessage				=	(int)(0x46),
	RopSetSpooler					=	(int)(0x47),
	RopSpoolerLockMessage				=	(int)(0x48),
	RopGetAddressTypes				=	(int)(0x49),
	RopTransportSend				=	(int)(0x4A),
	RopFastTransferSourceCopyMessages		=	(int)(0x4B),
	RopFastTransferSourceCopyFolder			=	(int)(0x4C),
	RopFastTransferSourceCopyTo			=	(int)(0x4D),
	RopFastTransferSourceGetBuffer			=	(int)(0x4E),
	RopFindRow					=	(int)(0x4F),
	RopProgress					=	(int)(0x50),
	RopTransportNewMail				=	(int)(0x51),
	RopGetValidAttachments				=	(int)(0x52),
	RopFastTransferDestinationConfigure		=	(int)(0x53),
	RopFastTransferDestinationPutBuffer		=	(int)(0x54),
	RopGetNamesFromPropertyIds			=	(int)(0x55),
	RopGetPropertyIdsFromNames			=	(int)(0x56),
	RopUpdateDeferredActionMessages			=	(int)(0x57),
	RopEmptyFolder					=	(int)(0x58),
	RopExpandRow					=	(int)(0x59),
	RopCollapseRow					=	(int)(0x5A),
	RopLockRegionStream				=	(int)(0x5B),
	RopUnlockRegionStream				=	(int)(0x5C),
	RopCommitStream					=	(int)(0x5D),
	RopGetStreamSize				=	(int)(0x5E),
	RopQueryNamedProperties				=	(int)(0x5F),
	RopGetPerUserLongTermIds			=	(int)(0x60),
	RopGetPerUserGuid				=	(int)(0x61),
	/* Reserved					=	(int)(0x62) */
	RopReadPerUserInformation			=	(int)(0x63),
	RopWritePerUserInformation			=	(int)(0x64),
	/* Reserved					=	(int)(0x65) */
	RopSetReadFlags					=	(int)(0x66),
	RopCopyProperties				=	(int)(0x67),
	RopGetReceiveFolderTable			=	(int)(0x68),
	RopFastTransferSourceCopyProperties		=	(int)(0x69),
	/* Reserved					=	(int)(0x6A) */
	RopGetCollapseState				=	(int)(0x6B),
	RopSetCollapseState				=	(int)(0x6C),
	RopGetTransportFolder				=	(int)(0x6D),
	RopPending					=	(int)(0x6E),
	RopOptionsData					=	(int)(0x6F),
	RopSynchronizationConfigure			=	(int)(0x70),
	/* Reserved					=	(int)(0x71) */
	RopSynchronizationImportMessageChange		=	(int)(0x72),
	RopSynchronizationImportHierarchyChange		=	(int)(0x73),
	RopSynchronizationImportDeletes			=	(int)(0x74),
	RopSynchronizationUploadStateStreamBegin	=	(int)(0x75),
	RopSynchronizationUploadStateStreamContinue	=	(int)(0x76),
	RopSynchronizationUploadStateStreamEnd		=	(int)(0x77),
	RopSynchronizationImportMessageMove		=	(int)(0x78),
	RopSetPropertiesNoReplicate			=	(int)(0x79),
	RopDeletePropertiesNoReplicate			=	(int)(0x7A),
	RopGetStoreState				=	(int)(0x7B),
	/* Reserved					=	(int)(0x7C) - (0x7D) */
	RopSynchronizationOpenCollector			=	(int)(0x7E),
	RopGetLocalReplicaIds				=	(int)(0x7F),
	RopSynchronizationImportReadStateChanges	=	(int)(0x80),
	RopResetTable					=	(int)(0x81),
	RopSynchronizationGetTransferState		=	(int)(0x82),
	/* Reserved					=	(int)(0x83) - (0x85) */
	RopTellVersion					=	(int)(0x86),
	/* Reserved					=	(int)(0x87) - (0x88) */
	RopFreeBookmark					=	(int)(0x89),
	/* Reserved					=	(int)(0x8A) - (0x8F) */
	RopWriteAndCommitStream				=	(int)(0x90),
	RopHardDeleteMessages				=	(int)(0x91),
	RopHardDeleteMessagesAndSubFolders		=	(int)(0x92),
	RopSetLocalReplicaMidsetDeleted			=	(int)(0x93),
	/* Reserved					=	(int)(0x94) - (0xF8) */
	RopBackoff					=	(int)(0xF9),
	/* Reserved					=	(int)(0xFA) - (0xFD) */
	RopLogon					=	(int)(0xFE),		/*!< [MS-OXCSTOR] (10/08/12 - 11.0) section 2.2.1.1 */
	RopBufferTooSmall				=	(int)(0xFF)
};

/**
   \enum mapirops_flow
   \brief Define the two different flows (push/pull) used in libmapirops
 */
enum mapirops_flow {
	MAPIROPS_PUSH	= 0x1,
	MAPIROPS_PULL
};

/**
   \struct mapibuf
   \brief Hold arbitrary MAPI blob of data
 */
struct mapibuf {
	uint8_t	*data;	/*!< Pointer to the blob of data */
	size_t	length; /*!< Length of the blob of data */
};

/**
   \enum mapirops_err_code
   \brief Define the different error codes used in libmapirops
 */
enum mapirops_err_code {
	MAPIROPS_ERR_SUCCESS = 0,	/*!< Success error code */
	MAPIROPS_ERR_BUFFER_TOO_SMALL,	/*!< Buffer is too small */
	MAPIROPS_ERR_BUFSIZE,		/*!< Invalid buffer size */
	MAPIROPS_ERR_NO_MEMORY,		/*!< No more memory left */
	MAPIROPS_ERR_ALLOC,		/*!< Memory allocation error */
	MAPIROPS_ERR_ICONV,		/*!< Iconv error */
	MAPIROPS_ERR_INVALID_FLAGS,	/*!< Invalid flag or combination of flags */
	MAPIROPS_ERR_INVALID_VAL,	/*!< Invalid value */
	MAPIROPS_ERR_INVALID_EC,	/*!< Invalid MAPI error code supplied for the call */
	MAPIROPS_GENERIC_ERR		/*!< Generic error code */
};

#define	MAPIROPS_STR_NOSIZE	(1<<0)	/*!< No prefixing size was retrieved from the wire. */
#define	MAPIROPS_STR_NOTERM	(1<<1)	/*!< The string has no termination char */

/**
   \struct mapirops_pull
   \brief Structure passed to routines that unpack MAPI rops
 */
struct mapirops_pull {
	struct mapibuf	data;		/*!< MAPI buffer from where data are pulled */
	uint32_t	offset;		/*!< Current MAPI buffer offset */
	TALLOC_CTX	*mem_ctx;	/*!< Pointer to the memory context */
	iconv_t		utf16to8;	/*!< Pointer to utf16 to utf8 iconv descriptor */
};

/**
   \struct mapirops_push
   \brief Structure passed to routines that pack MAPI rops
 */
struct mapirops_push {
	struct mapibuf	data;		/*!< MAPI buffer where data are pushed */
	uint32_t	offset;		/*!< MAPI buffer offset */
	iconv_t		utf8to16;	/*!< Pointer to utf8 to utf16 iconv descriptor */
	iconv_t		utf8toascii;	/*!< Pointer to utf8 to ascii iconv descriptor */
};

/** \cond */

#define	CAREFUL_ALIGNMENT	1
#ifndef	CVAL
#define	CVAL(buf,pos)(((unsigned char *)(buf))[pos])
#endif
#ifndef	PVAL
#define PVAL(buf,pos)((unsigned)CVAL(buf,pos))
#endif
#ifndef	SCVAL
#define SCVAL(buf,pos,val)(CVAL(buf,pos) = (val))
#endif
#ifndef	SVAL
#define SVAL(buf,pos)(PVAL(buf,pos)|PVAL(buf,(pos)+1)<<8)
#endif
#ifndef	IVAL
#define IVAL(buf,pos)(SVAL(buf,pos)|SVAL(buf,(pos)+2)<<16)
#endif
#ifndef	SSVALX
#define	SSVALX(buf,pos,val)(CVAL(buf,pos)=(val)&0xFF,CVAL(buf,pos+1)=(val)>>8)
#endif
#ifndef	SIVALX
#define SIVALX(buf,pos,val)(SSVALX(buf,pos,val&0xFFFF),SSVALX(buf,pos+2,val>>16))
#endif
#ifndef	IVALS
#define IVALS(buf,pos)((int32_t)IVAL(buf,pos))
#endif
#ifndef	SSVAL
#define	SSVAL(buf,pos,val) SSVALX((buf),(pos),((uint16_t)(val)))
#endif
#ifndef	SIVAL
#define	SIVAL(buf,pos,val) SIVALX((buf),(pos),((uint32_t)(val)))
#endif
#ifndef	SIVALS
#define	SIVALS(buf,pos,val) SIVALX((buf),(pos),((int32_t)(val)))
#endif

#if (__GNUC__ >= 3)
#ifndef	likely
#define	likely(x) __builtin_expect(!!(x), 1)
#endif
#ifndef	unlikely
#define	unlikely(x) __builtin_expect(!!(x), 0)
#endif
#else
#ifndef	likely
#define	likely(x) (x)
#endif
#ifndef	unlikely
#define	unlikely(x) (x)
#endif
#endif

#define	MAPIROPS_ERR_CODE_IS_SUCCESS(x) (x == MAPIROPS_ERR_SUCCESS)

#define	MAPIROPS_CHECK(call) do {				\
	enum mapirops_err_code	_status;			\
	_status = call;						\
	if (unlikely(!MAPIROPS_ERR_CODE_IS_SUCCESS(_status))) {	\
		return _status;					\
	}							\
} while (0)

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

/* The following definitions come from mapirops.c */
struct mapirops_push	*mapirops_push_init(TALLOC_CTX *);
struct mapirops_pull	*mapirops_pull_init(TALLOC_CTX *);
enum mapirops_err_code	mapirops_push_bytes(struct mapirops_push *, const uint8_t *, uint32_t);
enum mapirops_err_code	mapirops_pull_bytes(struct mapirops_pull *, uint8_t *, uint32_t);
enum mapirops_err_code	mapirops_push_int8(struct mapirops_push *, int8_t);
enum mapirops_err_code	mapirops_pull_int8(struct mapirops_pull *, int8_t *);
enum mapirops_err_code	mapirops_push_uint8(struct mapirops_push *, uint8_t);
enum mapirops_err_code	mapirops_pull_uint8(struct mapirops_pull *, uint8_t *);
enum mapirops_err_code	mapirops_push_int16(struct mapirops_push *, int16_t);
enum mapirops_err_code	mapirops_pull_int16(struct mapirops_pull *, int16_t *);
enum mapirops_err_code	mapirops_push_uint16(struct mapirops_push *, uint16_t);
enum mapirops_err_code	mapirops_pull_uint16(struct mapirops_pull *, uint16_t *);
enum mapirops_err_code	mapirops_push_int32(struct mapirops_push *, int32_t);
enum mapirops_err_code	mapirops_pull_int32(struct mapirops_pull *, int32_t *);
enum mapirops_err_code	mapirops_push_uint32(struct mapirops_push *, uint32_t);
enum mapirops_err_code	mapirops_pull_uint32(struct mapirops_pull *, uint32_t *);
enum mapirops_err_code	mapirops_push_int64(struct mapirops_push *, int64_t);
enum mapirops_err_code	mapirops_pull_int64(struct mapirops_pull *, int64_t *);
enum mapirops_err_code	mapirops_push_uint64(struct mapirops_push *, uint64_t);
enum mapirops_err_code	mapirops_pull_uint64(struct mapirops_pull *, uint64_t *);
enum mapirops_err_code	mapirops_push_double(struct mapirops_push *, double);
enum mapirops_err_code	mapirops_pull_double(struct mapirops_pull *, double *);
enum mapirops_err_code	mapirops_push_ascii_string(struct mapirops_push *, int, char *);
enum mapirops_err_code	mapirops_pull_ascii_string(struct mapirops_pull *, TALLOC_CTX *, int, char **, size_t);
enum mapirops_err_code	mapirops_push_utf16_string(struct mapirops_push *, int, char *);
enum mapirops_err_code	mapirops_pull_utf16_string(struct mapirops_pull *, TALLOC_CTX *, int, char **, size_t);
enum mapirops_err_code	mapirops_push_enum_MAPISTATUS(struct mapirops_push *, enum MAPISTATUS);
enum mapirops_err_code	mapirops_pull_enum_MAPISTATUS(struct mapirops_pull *, enum MAPISTATUS *);

/* The following definitions come from mapirops_print.c */
void mapirops_hexdump(const uint8_t *, int);

__END_DECLS

#endif /* ! __LIBMAPIROPS_H__ */
