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
   \file mapistatus.h
   \author Julien Kerihuel <j.kerihuel@openchange.org>
   \version 0.1
   \brief MAPI error codes and alternate names as described in
   [MS-OXCDATA] - v20121003
 */

#ifndef	__MAPISTATUS_H__
#define	__MAPISTATUS_H__

/**
   \enum MAPISTATUS
   \brief Enumeration of all available MAPI error and warning codes
 */
enum MAPISTATUS {
	/* Error codes */
	MAPI_E_SUCCESS			=	0x00000000,	/*!< Success: The operation successed */
	MAPI_E_CALL_FAILED		=	0x80004005,	/*!< GeneralFailure: The operation failed for an unspecified reason */
	MAPI_E_NOT_ENOUGH_MEMORY	=	0x8007000E,	/*!< OutOfMemory: Not enough memory was available to complete the operation */
	MAPI_E_INVALID_PARAMETER	=	0x80070057,	/*!< InvalidParameter: Invalid parameter passed to a RPC call */
	MAPI_E_INTERFACE_NOT_SUPPORTED	=	0x80004002,	/*!< NoInterface: The requested interface is not supported */
	MAPI_E_NO_ACCESS		=	0x80070005,	/*!< AccessDenied: The caller does not have sufficient access rights */
	STG_E_INVALIDFUNCTION		=	0x80030001,	/*!< StorageInvalidFunction: The server was unable to perform the requested operation */
	STG_E_ACCESSDENIED		=	0x80030005,	/*!< StorageAccessDenied: The caller does not have sufficient access rights  */
	STG_E_INSUFFICIENTMEMORY	=	0x80030008,	/*!< StorageInsufficientMemory: Insufficient memory available */
	STG_E_INVALIDPOINTER		=	0x80030009,	/*!< StorageInvalidPointer: An invalid pointer was passed to RPC call */
	STG_E_READFAULT			=	0x8003001E,	/*!< StorageReadFault: Disk error occurred during read operation */
	STG_E_LOCKVIOLATION		=	0x80030021,	/*!< StorageLockViolation: Lock violation occurred */
	STG_E_INVALIDPARAMETER		=	0x80030057,	/*!< StorageInvalidParameter: Invalid parameter passed to a RPC call */
	STG_E_INVALIDFLAG		=	0x800300FF,	/*!< StorageInvalidFlag: Invalid flag passed to a RPC call */
	STG_E_CANTSAVE			=	0x80030103,	/*!< StorageCantSave: Stream could not be saved */
	MAPI_E_NO_SUPPORT		=	0x80040102,	/*!< NotSupported: The server does not support this method call */
	MAPI_E_BAD_CHARWIDTH		=	0x80040103,	/*!< InvalidCharacterWidth: Unicode chars requested when only 8-bit are supported or vice versa */
	MAPI_E_STRING_TOO_LONG		=	0x80040105,	/*!< StringTooLong: String exceeds maximum permitted length for this call */
	MAPI_E_UNKNOWN_FLAGS		=	0x80040106,	/*!< InvalidFlag: Unrecognized flag bit passed to method call */
	MAPI_E_INVALID_ENTRYID		=	0x80040107,	/*!< InvalidEntryID: Incorrectly formatted EntryID passed to method call */
	MAPI_E_INVALID_OBJECT		=	0x80040108,	/*!< InvalidObject: method call made with ref to an object destroyed or not in a viable state */
	MAPI_E_OBJECT_CHANGED		=	0x80040109,	/*!< ObjectChanged: Attempt to commit changes failed because object was changed separately */
	MAPI_E_OBJECT_DELETED		=	0x8004010A,	/*!< ObjectDeleted: Operation failed because object was changed separately */
	MAPI_E_BUSY			=	0x8004010B,	/*!< ServerBusy: Table operation failed because separate op was in progress at the same time */
	MAPI_E_NOT_ENOUGH_DISK		=	0x8004010D,	/*!< OutOfDisk: Not enough disk space available to complete operation */
	MAPI_E_NOT_ENOUGH_RESOURCES	=	0x8004010E,	/*!< OutOfResources: Not enough of unspecified resource to complete operation */
	MAPI_E_NOT_FOUND		=	0x8004010F,	/*!< NotFound: The requested object could not be found on the server */
	MAPI_E_VERSION			=	0x80040110,	/*!< VersionMismatch: Client and server versions are incompatible*/
	MAPI_E_LOGON_FAILED		=	0x80040111,	/*!< LogonFailed: Client was unable to log on the server */
	MAPI_E_SESSION_LIMIT		=	0x80040112,	/*!< TooManySessions: Server or service is unable to create any more sessions */
	MAPI_E_USER_CANCEL		=	0x80040113,	/*!< UserCanceled: An operation failed because a user cancelled it */
	MAPI_E_UNABLE_TO_ABORT		=	0x80040114,	/*!< AbortFailed: RopAbort or RopAbortSubmit ROP was unsuccessful */
	MAPI_E_NETWORK_ERROR		=	0x80040115,	/*!< NetworkError: Unsuccessful because of a problem with network operations or services */
	MAPI_E_DISK_ERROR		=	0x80040116,	/*!< DiskError: Problem writing or reading to disk */
	MAPI_E_TOO_COMPLEX		=	0x80040117,	/*!< TooComplex: Operation too complex for the server to handle - generally related to restrictions */
	MAPI_E_BAD_COLUMN		=	0x80040118,	/*!< InvalidColumn: Requested column is not allowed in this type of table */
	MAPI_E_COMPUTED			=	0x8004011A,	/*!< ComputedValue: Property can't be updated because it is read-only, computed by server */
	MAPI_E_CORRUPT_DATA		=	0x8004011B,	/*!< CorruptData: Internal inconsistency in database or in complex property value */
	MAPI_E_UNKNOWN_CPID		=	0x8004011E,	/*!< InvalidCodepage: Server not configured to support requested code page */
	MAPI_E_UNKNOWN_LCID		=	0x8004011F,	/*!< InvalidLocale: Server not configured to support requested locale */
	MAPI_E_INVALID_ACCESS_TIME	=	0x80040123,	/*!< TimeSkew: Clock skew between servers */
	MAPI_E_END_OF_SESSION		=	0x80040200,	/*!< EndOfSession: Server session has been destroyed, possibly by a server restart */
	MAPI_E_UNKNOWN_ENTRYID		=	0x80040201,	/*!< UnknownEntryId: EntryID passed to OpenEntry was created by a different MAPI provider */
	MAPI_E_UNABLE_TO_COMPLETE	=	0x80040400,	/*!< NotCompleted: A complex operation such as building a table row set could not be completed */
	MAPI_E_TIMEOUT			=	0x80040401,	/*!< Timeout: Asynchronous operation did not succeeded within specified time-out */
	MAPI_E_TABLE_EMPTY		=	0x80040402,	/*!< EmptyTable: A table essential to the operation is empty */
	MAPI_E_TABLE_TOO_BIG		=	0x80040403,	/*!< TableTooBig: The table is too big for requested operation to complete */
	MAPI_E_INVALID_BOOKMARK		=	0x80040405,	/*!< InvalidBookmark: The bookmark passed to the table op was not created on the same table */
	MAPI_E_WAIT			=	0x80040500,	/*!< ErrorWait: Wait time-out expired */
	MAPI_E_CANCEL			=	0x80040501,	/*!< ErrorCancel: Operation had to be cancelled */
	MAPI_E_NO_SUPPRESS		=	0x80040602,	/*!< NoSuppress: Server does not support suppress of read receipts */
	MAPI_E_COLLISION		=	0x80040604,	/*!< CollidingNames: Folder or item cannot be created because one with the same name or other criteria exists  */
	MAPI_E_NOT_INITIALIZED		=	0x80040605,	/*!< NotInitialized: The subsystem is not ready */
	MAPI_E_NO_RECIPIENTS		=	0x80040607,	/*!< NoRecipients: Message cannot be sent because it has no recipients */
	MAPI_E_SUBMITTED		=	0x80040608,	/*!< AlreadySent: Message cannot be opened for modification because it has already been sent */
	MAPI_E_HAS_FOLDERS		=	0x80040609,	/*!< HasFolders: Folder cannot be deleted because it still contains subfolders */
	MAPI_E_HAS_MESSAGES		=	0x8004060A,	/*!< HasMessages: Folder cannot be deleted because it still contains messages */
	MAPI_E_FOLDER_CYCLE		=	0x8004060B,	/*!< FolderCycle: A folder move or copy operation would create a cycle (e.g. parent to subfolder copy) */
	MAPI_E_LOCKID_LIMIT		=	0x8004060D,	/*!< TooManyLocks: Too many locks have been requested */
	MAPI_E_AMBIGUOUS_RECIP		=	0x80040700,	/*!< AmbiguousRecipient: An unresolved recipient matches more than one entry in the directory */
	SYNC_E_OBJECT_DELETED		=	0x80040800,	/*!< SyncObjectDeleted: The requested objected was previously deleted */
	SYNC_E_IGNORE			=	0x80040801,	/*!< IgnoreFailure: Error occurred, but it is safe to ignore the error */
	SYNC_E_CONFLICT			=	0x80040802,	/*!< SyncConflict: Conflicting changes to an object have been detected */
	SYNC_E_NO_PARENT		=	0x80040803,	/*!< NoParentFolder: Parent folder could not be found */
	SYNC_E_CYCLE_DELETED		=	0x80040804,	/*!< CycleDeleted: Operation would create a cycle */
	SYNC_E_UNSYNCHRONIZED		=	0x80040805,	/*!< NotSynchronized: Sync operation did not take place, possibly due to conflicting change */
	MAPI_E_NAMED_PROP_QUOTA_EXCEEDED=	0x80040900,	/*!< NamedPropertyQuota: Store object cannot store any more named property */
	MAPI_E_NOT_IMPLEMENTED		=	0x80040FFF,	/*!< NotImplemented: Server does not implement this method call */
	
	/* Additional error codes */
	ecProfileNotConfigured		=	0x0000011C,	/*!< ProfileNotConfigured: Profile is not configured */
	ecJetError			=	0x000003E4,	/*!< IsamError: Unspecified database error */
	ecUnknownUser			=	0x000003EB,	/*!< UnknownUser: Unable to identify a Store object for this user */
	ecExiting			=	0x000003ED,	/*!< Exiting: The server is in the process of stopping */
	ecBadConfig			=	0x000003EE,	/*!< BadConfig: Protocol settings for this user are incorrect */
	ecUnknownCodePage		=	0x000003EF,	/*!< UnknownCodePage: Specified code page is not installed on server */
	ecMemory			=	0x000003F0,	/*!< ServerMemory: Server is out of memory */
	ecLoginPerm			=	0x000003F2,	/*!< LoginPermission: This user does not have access rights to the mailbox */
	ecDatabaseRolledBack		=	0x000003F3,	/*!< DatabaseRolledBack: Database has been restored and needs fix-up but cannot be fixed up */
	ecDatabaseCopiedError		=	0x000003F4,	/*!< DatabaseCopiedError: Database has been copied to another server */
	ecAuditNotAllowed		=	0x000003F5,	/*!< AuditNotAllowed: Auditing of security operations not allowed */
	ecZombieUser			=	0x000003F6,	/*!< ZombieUser: User has no security identifier */
	ecUnconvertableACL		=	0x000003F7,	/*!< UnconvertableACL: ACL cannot be converted to NTFS format */
	ecNoFreeJses			=	0x0000044C,	/*!< NoFreeJetSessions: No Jet sesion is avialable */
	ecDifferentJses			=	0x0000044D,	/*!< DifferentJetSession: Warning: a Jet session different than the one requested was returned */
	ecFileRemove			=	0x0000044F,	/*!< FileRemove: Error occurred while attempting to remove database file */
	ecParameterOverflow		=	0x00000450,	/*!< ParameterOverflow: Parameter value overflow */
	ecBadVersion			=	0x00000451,	/*!< BadVersion: Bad message store database version number */
	ecTooManyCols			=	0x00000452,	/*!< TooManyColumns: Too many columns requested in SetColumns */
	ecHaveMore			=	0x00000453,	/*!< HaveMore: ROP has more data to return */
	ecDatabaseError			=	0x00000454,	/*!< DatabaseError: General database problem */
	ecIndexNameTooBig		=	0x00000455,	/*!< IndexNameTooBig: An index name is larger than what Jet allows */
	ecUnsupportedProp		=	0x00000456,	/*!< UnsupportedProperty: The property data type is not supported */
	ecMsgNotSaved			=	0x00000457,	/*!< MessageNotSaved: During AbortSubmit, message was not saved */
	ecUnpubNotif			=	0x00000459,	/*!< UnpublishedNotification: Notification could not be published at this time */
	ecDifferentRoot			=	0x0000045B,	/*!< DifferentRoot: Moving or copying folders to a different top-level hierarchy is not supported */
	ecBadFolderName			=	0x0000045C,	/*!< BadFolderName: Invalid folder name */
	ecAttachOpen			=	0x0000045D,	/*!< AttachmentOpen: The attachment is opened */
	ecInvClpsState			=	0x0000045E,	/*!< InvalidCollapseState: Collapse state given to SetCollapseState is invalid */
	ecSkipMyChildren		=	0x0000045F,	/*!< SkipMyChildren: While walking a folder tree, do not consider children of a tree */
	ecSearchFolder			=	0x00000460,	/*!< SearchFolder: Operation not supported on a search folder */
	ecNotSearchFolder		=	0x00000461,	/*!< NotSearchFolder: Operation only valid on search folder */
	ecFolderSetReceive		=	0x00000462,	/*!< FolderSetReceive: This is the receive folder and cannot be deleted */
	ecNoDelSubmitMsg		=	0x00000465,	/*!< DeletedSubmitMessage: Deleting a submitted message for sending is not permitted */
	ecInvalidRecips			=	0x00000467,	/*!< InvalidRecipients: Impossible to deliver to this recipient */
	ecNoReplicaHere			=	0x00000468,	/*!< NoReplicaHere: No replica of the public folder in this Store object */
	ecNoReplicaAvailable		=	0x00000469,	/*!< NoReplicaAvailable: No available Store objec thas a replica of this public folder */
	ecPublicMDB			=	0x0000046A,	/*!< PublicDatabase: The operation is invalid on a public Store object */
	ecNotPublicMDB			=	0x0000046B,	/*!< NotPublicDatabase: The operation is valid only on a public Store object */
	ecRecordNotFound		=	0x0000046C,	/*!< RecordNotFound: The record was not found */
	ecReplConflict			=	0x0000046D,	/*!< ReplicationConflict: Replication conflict was detected */
	ecFXBufferOverrun		=	0x00000470,	/*!< FXBufferOverrun: Prevented an overrun while reading a fast transfer buffer */
	ecFXBufferEmpty			=	0x00000471,	/*!< FXBufferEmpty: No more in a fast transfer buffer */
	ecFxPartialValue		=	0x00000472,	/*!< FXPartialValue: Partial Long value in a fast transfer buffer */
	ecFxNoRoom			=	0x00000473,	/*!< FXNoRoom: No room for an atomic value in a fast transfer buffer */
	ecMaxTimeExpired		=	0x00000474,	/*!< TimeExpired: HouseKeeping functions have exceeded their time window */
	ecDstError			=	0x00000475,	/*!< DestinationError: Error occurred on destination folder during a copy operation */
	ecMDBNotInit			=	0x00000476,	/*!< DatabaseNotInitialized: Store object was not properly initialized */
	ecWrongServer			=	0x00000478,	/*!< WrongServer: This server does not host the user's mailbox database */
	ecBufferTooSmall		=	0x0000047D,	/*!< BufferTooSmall: A buffer passed to this function is not big enough */
	ecRequiresRefResolve		=	0x0000047E,	/*!< AttachmentResolutionRequired: Linked attachments could not be resolved to actual files */
	ecServerPaused			=	0x0000047F,	/*!< ServerPaused: The server is in a paused state */
	ecServerBusy			=	0x00000480,	/*!< ServerBusy: The server is too busy to complete the operation */
	ecNoSuchLogon			=	0x00000481,	/*!< NoSuchLogon: No such logon exists in the Store object's logon list */
	ecLoadLibFailed			=	0x00000482,	/*!< LoadLibraryFailed: Internal error: the service cannot load a required library */
	ecObjAlreadyConfig		=	0x00000483,	/*!< AlreadyConfigured: A synchronization object has already been configured */
	ecObjNotConfig			=	0x00000484,	/*!< NotConfigured: A synchronization has not yet been configured */
	ecDataLoss			=	0x00000485,	/*!< DataLoss: a code page conversion incurred data loss */
	ecMaxSendThreadExceeded		=	0x00000488,	/*!< MaximumSendThreadExceeded: maximum number of send threads has been exceeded */
	ecFxErrorMarker			=	0x00000489,	/*!< FxErrorMarker: A fast transfer error marker was found, and recovery is necessary */
	ecNoFreeJtabs			=	0x0000048A,	/*!< NoFreeJtabs: no more free Jet tables */
	ecNotPrivateMDB			=	0x0000048B,	/*!< NotPrivateDatabase: Operation is valid only on a private mailbox database */
	ecIsintegMDB			=	0x0000048C,	/*!< IsIntegMDB: Store object locked by ISINTEG utility */
	ecRecoveryMDBMismatch		=	0x0000048D,	/*!< RecoveryMismatch: recovery storage group op attempted on a non-RSG Store object, or vice versa */
	ecTableMayNotBeDeleted		=	0x0000048E,	/*!< TableMayNotBeDeleted: Attempt to delete a critical table, such as the msg or attach table */
	ecSearchFolderScopeViolation	=	0x00000490,	/*!< SearchFolderScopeViolation: Attempt to perform a recursive search on a search folder */
	ecRpcRegisterIf			=	0x000004B1,	/*!< RpcRegisterIf: Error in registering RPC interfaces */
	ecRpcListen			=	0x000004B2,	/*!< RpcListen: Error in starting the RPC listener */
	ecRpcFormat			=	0x000004B6,	/*!< RpcFormat: badly formatted RPC buffer was detected */
	ecNoCopyTo			=	0x000004B7,	/*!< NoCopyTo: Single instance storage cannot be used in this case */
	ecNullObject			=	0x000004B9,	/*!< NullObject: An object handle referenced in the RPC buffer could not be resolved */
	ecRpcAuthentication		=	0x000004BC,	/*!< RpcAuthentication: Server requests client to use authentication */
	ecRpcBadAuthenticationLevel	=	0x000004BD,	/*!< RpcBadAuthenticationLevel: Server doesn't recognize a client's authentication level */
	ecNullCommentRestriction	=	0x000004BE,	/*!< NullCommentRestriction: The subrestriction of a comment restriction is empty */
	ecRulesLoadError		=	0x000004CC,	/*!< RulesLoadError: Rule data was unavailable for this folder */
	ecRulesDeliverErr		=	0x000004CD,	/*!< RulesDeliverErr: Delivery-time failure in rule execution */
	ecRulesParsingErr		=	0x000004CE,	/*!< RulesParsingErr: Invalid syntax in stored rule condition or action */
	ecRulesCreateDaeErr		=	0x000004CF,	/*!< RulesCreateDae: Failure creating a deferred rule action error message */
	ecRulesCreateDamErr		=	0x000004D0,	/*!< RulesCreateDAM: Failure creating a deferred rule action message */ 
	ecRulesNoMoveCopyFolder		=	0x000004D1,	/*!< RulesNoMoveCopyFolder: a move or copy rule action could not be performed due to problem with target folder */
	ecRulesNoFolderRights		=	0x000004D2,	/*!< RuleNoFolderRights: a move or copy rule action could not be performed due to permissions problem with the target folder */
	ecMessageTooBig			=	0x000004D4,	/*!< MessageTooBig: Message could not be delivered because it exceeds the maximum size */
	ecFormNotValid			=	0x000004D5,	/*!< FormNotValid: Problem with the form mapped to the message's message class */
	ecNotAuthorized			=	0x000004D6,	/*!< NotAuthorized: Delivery to the desired folder was not authorized */
	ecDeleteMessage			=	0x000004D7,	/*!< DeleteMessage: The message was deleted by a rule action */
	ecBounceMessage			=	0x000004D8,	/*!< BounceMessage: Delivery of the message was denied by a rule action */
	ecQuotaExceeded			=	0x000004D9,	/*!< QuotaExceeded: Operation failed because it would have exceeded a resource quota */
	ecMaxSubmissionExceeded		=	0x000004DA,	/*!< MaxSubmissionExceeded: Message could not be submitted because it exceeds maximum defined size */
	ecMaxAttachmentExceeded		=	0x000004DB,	/*!< MaxAttachmentExceeded: The maximum number of message attachment has been exceeded */
	ecSendAsDenied			=	0x000004DC,	/*!< SendAsDenied: User account does not have permissions to send email as the owner of the mailbox */
	ecShutoffQuotaExceeded		=	0x000004DD,	/*!< ShutoffQuotaExceeded: Operation failed because it would have exceeded mailbox's shutoff quota */
	ecMaxObjsExceeded		=	0x000004DE,	/*!< TooManyOpenObjects: A client has opened too many objects of a specific type */
	ecClientVersionDisallowed	=	0x000004DF,	/*!< ClientVersionBlocked: The server is configured to block clients of this version */
	ecRpcHttpDisallowed		=	0x000004E0,	/*!< RpcHttpDisallowed: The server is configured to block RPC connections over HTTP */
	ecCachedModeRequired		=	0x000004E1,	/*!< CachedModeRequired: The server is configured to block online mode connections */
	ecFolderCleanedUp		=	0x000004E3,	/*!< FolderCleanedUp: The foloder has been deleted but not yet cleaned up */
	ecFmtError			=	0x000004ED,	/*!< FormatError: Part of the ROP buffer was incorrectly formatted */
	ecNotExpanded			=	0x000004F7,	/*!< NotExpanded: Error in expanding or collapsing rows in a categorized view */
	ecNotCollapsed			=	0x000004F8,	/*!< NotCollapsed: Error in expanding or collapsing rows in a categorized view */
	ecLeaf				=	0x000004F9,	/*!< NoExpandedLeafRow: Leaf rows cannot be expanded. */
	ecUnregisteredNamedProp		=	0x000004FA,	/*!< UnregisteredNamedProp: Operation attempted on a named property ID for which no name has been registered */
	ecFolderDisabled		=	0x000004FB,	/*!< FolderDisabled: Access to the folder is disabled */
	ecDomainError			=	0x000004FC,	/*!< DomainError: Inconsistency in the Store object's association with its server */
	ecNoCreateRight			=	0x000004FF,	/*!< NoCreateRight: Operation requires create access rights the user does not have */
	ecPublicRoot			=	0x00000500,	/*!< PublicRoot: Operation requires create access rights at a public folder root */
	ecNoReadRight			=	0x00000501,	/*!< NoReadRight: Operation requires read access rights the user does not have */
	ecNoCreateSubfolderRight	=	0x00000502,	/*!< NoCreateSubfolderRight: Operation requires create subfolder right the user does not have */
	ecDstNullObject			=	0x00000503,	/*!< NullDestinationObject: RPC buffer contains a destination object handle that could not be resolved to a server object */
	ecMsgCycle			=	0x00000504,	/*!< MessageCycle: The source message contains the destination message and cannot be attached to it */
	ecTooManyRecips			=	0x00000505,	/*!< ToomanyRecips: A hard limit on the number of recipients per message was exceeded */
	ecVirusScanInProgress		=	0x0000050A,	/*!< VirusScanInProgress: Operation failed because the target message is being scanned for viruses */
	ecVirusDetected			=	0x0000050B,	/*!< VirusDetected: Operation failed because the target message is infected with a virus */
	ecMailboxInTransit		=	0x0000050C,	/*!< MailboxInTransit: The mailbox is in transit and is not accepting mail */
	ecBackupInProgress		=	0x0000050D,	/*!< BackupInProgress: Operation failed because the Store object is being backed up */
	ecVirusMessageDeleted		=	0x0000050E,	/*!< VirusMessageDeleted: Operation failed because remote target was infected with a virus and got deleted */
	ecInvalidBackupSequence		=	0x0000050F,	/*!< InvalidBackupSequence: Backup steps were performed out of sequence */
	ecInvalidBackupType		=	0x00000510,	/*!< InvalidBackupType: The requested backup type was not recognized */
	ecTooManyBackupsInProgress	=	0x00000511,	/*!< TooManyBackups: Too many backups are already in progress */
	ecRestoreInProgress		=	0x00000512,	/*!< RestoreInProgress: A restore is already in progress */
	ecDuplicateObject		=	0x00000579,	/*!< DuplicateObject: The object already exists */
	ecObjectNotFound		=	0x0000057A,	/*!< ObjectNotFound: An internal database object could not be found */
	ecFixupReplyRule		=	0x0000057B,	/*!< FixupReplyRule: The template Message ID in a reply rule object is missing or incorrect */
	ecTemplateNotFound		=	0x0000057C,	/*!< TemplateNotFound: The reply template could not be found for a message that triggered an auto-reply rule */
	ecRuleExecution			=	0x0000057D,	/*!< RuleExecution: An error occurred while executing a rule action */
	ecDSNoSuchObject		=	0x0000057E,	/*!< DSNoSuchObject: A server object could not be found in the directory */
	ecMessageAlreadyTombstoned	=	0x0000057F,	/*!< AlreadyTombstoned: An attempt to tombstone a message already in the message tombstone list failed */
	ecRequiresRWTransaction		=	0x00000596,	/*!< ReadOnlyTransaction: A write operation was attempted in a read-only transaction */
	ecPaused			=	0x0000060E,	/*!< Paused: Attempt to pause a server that is already paused */
	ecNotPaused			=	0x0000060F,	/*!< NotPaused: Attempt to unpause a server that is not paused */
	ecWrongMailbox			=	0x00000648,	/*!< WrongMailbox: Operation attempted on the wrong mailbox */
	ecChgPassword			=	0x0000064C,	/*!< ChangePassword: The account password needs to be changed */
	ecPwExpired			=	0x0000064D,	/*!< PasswordExpired: The account password has expired */
	ecInvWkstn			=	0x0000064E,	/*!< InvalidWorkstation: Account has logged on from the wrong workstation */
	ecInvLogonHrs			=	0x0000064F,	/*!< InvalidLogonHours: Account has logged on at the wrong time of day */
	ecAcctDisabled			=	0x00000650,	/*!< AccountDisabled: The account is disabled */
	ecRuleVersion			=	0x000006A4,	/*!< RuleVersion: The rule data contains an invalid rule version */
	ecRuleFormat			=	0x000006A5,	/*!< RuleFormat: The fule condition or action was incorrectly formatted */
	ecRuleSendAsDenied		=	0x000006A6,	/*!< RuleSendAsDenied: The rule is not authorized to send from this mailbox */
	ecNoServerSupport		=	0x000006B9,	/*!< NoServerSupport: A newer client requires functionality that an older server does not support */
	ecLockTimedOut			=	0x000006BA,	/*!< LockTimedOut: Attempt to unlock a message failed because the lock has already timed out */
	ecObjectLocked			=	0x000006BB,	/*!< ObjectLocked: The operation failed because the target object is locked */
	ecInvalidLockNamespace		=	0x000006BD,	/*!< InvalidLockNamespace: Attempt to lock a nonexistent object */
	ecMessageDeleted		=	0x000007D6,	/*!< MessageDeleted: Operation failed because the message has been deleted */
	ecProtocolDisabled		=	0x000007D8,	/*!< ProtocolDisabled: The requested protocol is disabled in the server configuration */
	ecCleartextLogonDisabled	=	0x000007D9,	/*!< CleartextLogonDisabled: Clear test logons are disabled */
	ecRejected			=	0x000007EE,	/*!< Rejected: Operation was rejected, perhaps because it is not supported */
	ecAmbiguousAlias		=	0x0000089A,	/*!< AmbiguousAlias: User account information did not uniquely identify a user */
	ecUnknownMailbox		=	0x0000089B,	/*!< UnknownMailbox: No mailbox object for this logon exists in the address book */
	ecExpReserved			=	0x000008FC,	/*!< ExpressionReserved: Internal error in evaluating an expression */
	ecExpParseDepth			=	0x000008FD,	/*!< ExpressionParseDepth: The expression tree exceeds a defined depth limit */
	ecExpFuncArgType		=	0x000008FE,	/*!< ExpressionArgumentType: An argument to a function has the wrong type */
	ecExpSyntax			=	0x000008FF,	/*!< ExpressionSyntax: Syntax error in expression */
	ecExpBadStrToken		=	0x00000900,	/*!< ExpressionBadStringToken: Invalid string token in expression */
	ecExpBadColToken		=	0x00000901,	/*!< ExpressionBadColToken: Invalid column name in expression */
	ecExpTypeMismatch		=	0x00000902,	/*!< ExpressionTypeMismatch: Property types - e.g. in a comparision expression - are incompatible */
	ecExpOpNotSupported		=	0x00000903,	/*!< ExpressionOperatorNotSupported: The requested operator is not supported */
	ecExpDivByZero			=	0x00000904,	/*!< ExpressionDivideByZero: Divide by zero doesn't work */
	ecExpUnaryArgType		=	0x00000905,	/*!< ExpressionUnaryArgument: The argument to an unary expression is of incorrect type */
	ecNotLocked			=	0x00000960,	/*!< NotLocked: An attempt to lock a resource failed */
	ecClientEvent			=	0x00000961,	/*!< ClientEvent: a client-supplied event has fired */
	ecCorruptEvent			=	0x00000965,	/*!< CorruptEvent: Data in the event table is bad */
	ecCorruptWatermark		=	0x00000966,	/*!< CorruptWatermark: A watermark in the event table is bad */
	ecEventError			=	0x00000967,	/*!< EventError: General event processing error */
	ecWatermarkError		=	0x00000968,	/*!< WatermarkError: An event watermark is out of range or otherwise invalid */
	ecNonCanonicalACL		=	0x00000969,	/*!< NonCanonicalACL: A modification to an ACL failed because existing ACL is not in cacnonical format */
	ecMailboxDisabled		=	0x0000096C,	/*!< MailboxDisabled: Logon was unsuccessful because the mailbox is disabled */
	ecRulesFolderOverQuota		=	0x0000096D,	/*!< RulesFolderOverQuota: A move or copy rule action failed because dest folder is over quota */
	ecADUnavailable			=	0x0000096E,	/*!< AddressBookUnavailable: The address book server could not be reached */
	ecADError			=	0x0000096F,	/*!< AddressBookError: Unspecified error from the address book server */
	ecADNotFound			=	0x00000971,	/*!< AddressBookObjectNotFound: An object was not found in the address book */
	ecADPropertyError		=	0x00000972,	/*!< AddressBookPropertyError: A property was not found in the address book */
	ecNotEncrypted			=	0x00000970,	/*!< NotEncrypted: The server is configured to force encrypted connections */
	ecRpcServerTooBusy		=	0x00000973,	/*!< RpcServerTooBusy: External RPC failed because the server was too busy */
	ecRpcOutOfMemory		=	0x00000974,	/*!< RpcOutOfMemory: External RPC failed because local server was out of memory */
	ecRpcServerOutOfMemory		=	0x00000975,	/*!< RpcServerOutOfMemory: External RPC failed because remote server was out of memory */
	ecRpcOutOfResources		=	0x00000976,	/*!< RpcOutOfResources: External RPC failed because remote server was out of an unspecified resource */
	ecRpcServerUnavailable		=	0x00000977,	/*!< RpcServerUnavailable: External RPC failed because remote server was unavailable */
	ecSecureSubmitError		=	0x0000097A,	/*!< SecureSubmitError: Failure occurred while setting the secure submission state of a message */
	ecEventsDeleted			=	0x0000097C,	/*!< EventsDeleted: Requested events were already deleted from the queue */
	ecSubsystemStopping		=	0x0000097D,	/*!< SubsystemStopping: A component service is in the process of shutting down */
	ecSAUnavailable			=	0x0000097E,	/*!< AttendantUnavailable: The system attendant service is unavailable */
	ecCiStopping			=	0x00000A28,	/*!< CIStopping: The content indexer server is stopping */
	ecFxInvalidState		=	0x00000A29,	/*!< FxInvalidState: An internal fast transfer object has invalid state */
	ecFxUnexpectedMarker		=	0x00000A2A,	/*!< FxUnexpectedMarker: Fast transfer parsing has hit an invalid marker */
	ecDuplicateDelivery		=	0x00000A2B,	/*!< DuplicateDelivery: A copy of this message has already been delivered */
	ecConditionViolation		=	0x00000A2C,	/*!< ConditionViolation: The condition was not met for a conditional operation */
	ecMaxPoolExceeded		=	0x00000A2D,	/*!< MaximumConnectionPoolsExceeded: RPC client has exceeded the defined limit of RPC connection pools */
	ecRpcInvalidHandle		=	0x00000A2E,	/*!< InvalidRpcHandle: The RPC connection is no longer valid */
	ecEventNotFound			=	0x00000A2F,	/*!< EventNotFound: There are no events in the event table, or requested event was not found */
	ecPropNotPromoted		=	0x00000A30,	/*!< PropertyNotPromoted: A property was not copied from the mesage table to the message header table */
	ecLowMdbSpace			=	0x00000A31,	/*!< LowFreeSpaceForDatabase: The drive hosting database files has little or no free space */
	ecLowMdbLogSpace		=	0x00000A32,	/*!< LowFreeSpaceForLogs: The drive hosting log files for the database has little or no free space */
	ecMailboxQuarantined		=	0x00000A33,	/*!< MailboxIsQuarantined: The mailbox has been placed under quarantine by an administrator */
	ecMountInProgress		=	0x00000A34,	/*!< DatabaseMountInProgress: The mailbox database is being mounted */
	ecDismountInProgress		=	0x00000A35,	/*!< DatabaseDismountInProgress: The mailbox database is being dismounted */
	ecMaxConnectionsExceeded	=	0x00000A36,	/*!< ConnectionsOverBudget: The number of RPC connections in use exceeds the amount budgeted for this client */
	ecNotFoundInContainer		=	0x00000A37,	/*!< NotFoundInContainer: The mailbox was not found in the mailbox metadata cache */
	ecCannotRemove			=	0x00000A38,	/*!< CannotRemove: An item cannot be removed from an internal list */
	ecInvalidPool			=	0x00000A39,	/*!< InvalidConnectionPool: RPC client has attempted connection using a connection pool unknown to the server */
	ecVirusScannerError		=	0x00000A3A	/*!< VirusScanGeneralFailure: A nonspecified failure occurred while scanning an item */
};

#define	ecNone				MAPI_E_SUCCESS			/*!< Alternate name for MAPI_E_SUCCESS */
#define	S_OK				MAPI_E_SUCCESS			/*!< Alternate name for MAPI_E_SUCCESS */
#define	E_FAIL				MAPI_E_CALL_FAILED		/*!< Alternate name for MAPI_E_CALL_FAILED */
#define	ecError				MAPI_E_CALL_FAILED		/*!< Alternate name for MAPI_E_CALL_FAILED */
#define	SYNC_E_ERROR			MAPI_E_CALL_FAILED		/*!< Alternate name for MAPI_E_CALL_FAILED */
#define	E_NOMEMORY			MAPI_E_NOT_ENOUGH_MEMORY	/*!< Alternate name for MAPI_E_NOT_ENOUGH_MEMORY */
#define	ecMAPIOOM			MAPI_E_NOT_ENOUGH_MEMORY	/*!< Alternate name for MAPI_E_NOT_ENOUGH_MEMORY */
#define	ecPropSize			MAPI_E_NOT_ENOUGH_MEMORY	/*!< Alternate name for MAPI_E_NOT_ENOUGH_MEMORY */
#define	E_INVALIDARG			MAPI_E_INVALID_PARAMETER	/*!< Alternate name for MAPI_E_INVALID_PARAMETER */
#define	ecInvalidParam			MAPI_E_INVALID_PARAMETER	/*!< Alternate name for MAPI_E_INVALID_PARAMETER */
#define	ecInvalidParameter		MAPI_E_INVALID_PARAMETER	/*!< Alternate name for MAPI_E_INVALID_PARAMETER */
#define	ecInvalidSession		MAPI_E_INVALID_PARAMETER	/*!< Alternate name for MAPI_E_INVALID_PARAMETER */
#define	ecBadBuffer			MAPI_E_INVALID_PARAMETER	/*!< Alternate name for MAPI_E_INVALID_PARAMETER */
#define	SYNC_E_INVALID_PARAMETER	MAPI_E_INVALID_PARAMETER	/*!< Alternate name for MAPI_E_INVALID_PARAMETER */
#define	E_NOINTERFACE			MAPI_E_INTERFACE_NOT_SUPPORTED	/*!< Alternate name for MAPI_E_INTERFACE_NOT_SUPPORTED */
#define	ecInterfaceNotSupported		MAPI_E_INTERFACE_NOT_SUPPORTED	/*!< Alternate name for MAPI_E_INTERFACE_NOT_SUPPORTED */
#define	E_ACCESSDENIED			MAPI_E_NO_ACCESS		/*!< Alternate name for MAPI_E_NO_ACCESS */
#define	ecAccessDenied			MAPI_E_NO_ACCESS		/*!< Alternate name for MAPI_E_NO_ACCESS */
#define	ecPropSecurityViolation		MAPI_E_NO_ACCESS		/*!< Alternate name for MAPI_E_NO_ACCESS */
#define	ecNotSupported			MAPI_E_NO_SUPPORT		/*!< Alternate name for MAPI_E_NO_SUPPORT */
#define	ecNotImplemented		MAPI_E_NO_SUPPORT		/*!< Alternate name for MAPI_E_NO_SUPPORT */
#define	ecBadCharwidth			MAPI_E_BAD_CHARWIDTH		/*!< Alternate name for MAPI_E_BAD_CHARWIDTH */
#define	ecStringTooLarge		MAPI_E_STRING_TOO_LONG		/*!< Alternate name for MAPI_E_STRING_TOO_LONG */
#define	ecUnknownFlags			MAPI_E_UNKNOWN_FLAGS		/*!< Alternate name for MAPI_E_UNKNOWN_FLAGS */
#define	SYNC_E_UNKNOWN_FLAGS		MAPI_E_UNKNOWN_FLAGS		/*!< Alternate name for MAPI_E_UNKNOWN_FLAGS */
#define	ecInvalidEntryId		MAPI_E_INVALID_ENTRYID		/*!< Alternate name for MAPI_E_INVALID_ENTRYID */
#define	ecInvalidObject			MAPI_E_INVALID_OBJECT		/*!< Alternate name for MAPI_E_INVALID_OBJECT */
#define	ecObjectModified		MAPI_E_OBJECT_CHANGED		/*!< Alternate name for MAPI_E_OBJECT_CHANGED */
#define	ecObjectDeleted			MAPI_E_OBJECT_DELETED		/*!< Alternate name for MAPI_E_OBJECT_DELETED */
#define	ecBusy				MAPI_E_BUSY			/*!< Alternate name for MAPI_E_BUSY */
#define	ecDiskFull			MAPI_E_NOT_ENOUGH_DISK		/*!< Alternate name for MAPI_E_NOT_ENOUGH_DISK */
#define	ecInsufficientResrc		MAPI_E_NOT_ENOUGH_RESOURCES	/*!< Alternate name for MAPI_E_NOT_ENOUGH_RESOURCES */
#define	ecNotFound			MAPI_E_NOT_FOUND		/*!< Alternate name for MAPI_E_NOT_FOUND */
#define	ecAttachNotFound		MAPI_E_NOT_FOUND		/*!< Alternate name for MAPI_E_NOT_FOUND */
#define	ecUnknownRecip			MAPI_E_NOT_FOUND		/*!< Alternate name for MAPI_E_NOT_FOUND */
#define	ecPropNotExistent		MAPI_E_NOT_FOUND		/*!< Alternate name for MAPI_E_NOT_FOUND */
#define	ecVersionMismatch		MAPI_E_VERSION			/*!< Alternate name for MAPI_E_VERSION */
#define	ecVersion			MAPI_E_VERSION			/*!< Alternate name for MAPI_E_VERSION */
#define	ecLoginFailure			MAPI_E_LOGON_FAILED		/*!< Alternate name for MAPI_E_LOGON_FAILED */
#define	ecTooManySessions		MAPI_E_SESSION_LIMIT		/*!< Alternate name for MAPI_E_SESSION_LIMIT */
#define	ecUserAbort			MAPI_E_USER_CANCEL		/*!< Alternate name for MAPI_E_USER_CANCEL */
#define ecUnableToAbort			MAPI_E_UNABLE_TO_ABORT		/*!< Alternate name for MAPI_E_UNABLE_TO_ABORT */
#define	ecNetwork			MAPI_E_NETWORK_ERROR		/*!< Alternate name for MAPI_E_NETWORK_ERROR */
#define	ecWriteFault			MAPI_E_DISK_ERROR		/*!< Alternate name for MAPI_E_DISK_ERROR */
#define	ecReadFault			MAPI_E_DISK_ERROR		/*!< Alternate name for MAPI_E_DISK_ERROR */
#define	ecTooComplex			MAPI_E_TOO_COMPLEX		/*!< Alternate name for MAPI_E_TOO_COMPLEX */
#define	ecComputed			MAPI_E_COMPUTED			/*!< Alternate name for MAPI_E_COMPUTED */
#define	ecCorruptData			MAPI_E_CORRUPT_DATA		/*!< Alternate name for MAPI_E_CORRUPT_DATA */
#define	ecTimeSkew			MAPI_E_INVALID_ACCESS_TIME	/*!< Alternate name for MAPI_E_INVALID_ACCESS_TIME */
#define	ecUnableToComplete		MAPI_E_UNABLE_TO_COMPLETE	/*!< Alternate name for MAPI_E_UNABLE_TO_COMPLETE */
#define	ecTimeout			MAPI_E_TIMEOUT			/*!< Alternate name for MAPI_E_TIMEOUT */
#define	ecTableEmpty			MAPI_E_TABLE_EMPTY		/*!< Alternate name for MAPI_E_TABLE_EMPTY */
#define	ecTableTooBig			MAPI_E_TABLE_TOO_BIG		/*!< Alternate name for MAPI_E_TABLE_TOO_BIG */
#define	ecInvalidBookmark		MAPI_E_INVALID_BOOKMARK		/*!< Alternate name for MAPI_E_INVALID_BOOKMARK */
#define	ecWait				MAPI_E_WAIT			/*!< Alternate name for MAPI_E_WAIT */
#define	ecCancel			MAPI_E_CANCEL			/*!< Alternate name for MAPI_E_CANCEL */
#define	ecDuplicateName			MAPI_E_COLLISION		/*!< Alternate name for MAPI_E_COLLISION */
#define	ecNotInitialized		MAPI_E_NOT_INITIALIZED		/*!< Alternate name for MAPI_E_NOT_INITIALIZED */
#define	ecSubmitted			MAPI_E_SUBMITTED		/*!< Alternate name for MAPI_E_SUBMITTED */
#define	ecFolderHasChildren		MAPI_E_HAS_FOLDERS		/*!< Alternate name for MAPI_E_HAS_FOLDERS */
#define	ecFolderHasContents		MAPI_E_HAS_MESSAGES		/*!< Alternate name for MAPI_E_HAS_MESSAGES */
#define	ecRootFolder			MAPI_E_FOLDER_CYCLE		/*!< Alternate name for MAPI_E_FOLDER_CYCLE */
#define	ecLockIdLimit			MAPI_E_LOCKID_LIMIT		/*!< Alternate name for MAPI_E_LOCKID_LIMIT */
#define	ecAmbiguousRecip		MAPI_E_AMBIGUOUS_RECIP		/*!< Alternate name for MAPI_E_AMBIGUOUS_RECIP */
#define	ecNPQuotaExceeded		MAPI_E_NAMED_PROP_QUOTA_EXCEEDED/*!< Alternate name for MAPI_E_NAMED_PROP_QUOTA_EXCEEDED */
#define	ecServerOOM			ecMemory			/*!< Alternate name for ecMemory */

#endif /*!__MAPISTATUS_H__ */
