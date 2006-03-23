/* 
   EyeDB Object Database Management System
   Copyright (C) 1994-1999,2004-2006 SYSRA
   
   EyeDB is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
   
   EyeDB is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA 
*/

/*
   Author: Eric Viara <viara@sysra.com>
*/


#include <assert.h>

#include "serv_lib.h"
namespace eyedb {
  class ObjectHeader;
}

#include "eyedb/base.h"
#include "eyedb/Error.h"
#include "eyedb/Exception.h"
#include "eyedb/TransactionParams.h"
#include "kernel.h"
#include <eyedblib/xdr.h>
#include <eyedbsm/xdr.h>
#include "eyedbsm/transaction.h"

namespace eyedb {

  rpc_ServerFunction
  *DBMCREATE_RPC,
    *DBMUPDATE_RPC,

    *DBCREATE_RPC,
    *DBDELETE_RPC,

    *DBINFO_RPC,
    *DBMOVE_RPC,
    *DBCOPY_RPC,
    *DBRENAME_RPC,

    *USER_ADD_RPC,
    *USER_DELETE_RPC,
    *USER_PASSWD_SET_RPC,
    *PASSWD_SET_RPC,

    *DEFAULT_DBACCESS_SET_RPC,
    *USER_DBACCESS_SET_RPC,
    *USER_SYSACCESS_SET_RPC,

    *BACKEND_INTERRUPT_RPC,

    *TRANSACTION_BEGIN_RPC,
    *TRANSACTION_ABORT_RPC,
    *TRANSACTION_COMMIT_RPC,

    *TRANSACTION_PARAMS_SET_RPC,
    *TRANSACTION_PARAMS_GET_RPC,

    *DBOPEN_RPC,
    *DBOPENLOCAL_RPC,
    *DBCLOSE_RPC,

    *OBJECT_CREATE_RPC,
    *OBJECT_READ_RPC,
    *OBJECT_WRITE_RPC,
    *OBJECT_DELETE_RPC,
    *OBJECT_HEADER_READ_RPC,
    *OBJECT_SIZE_MODIFY_RPC,
    *OBJECT_PROTECTION_SET_RPC,
    *OBJECT_PROTECTION_GET_RPC,
    *OBJECT_CHECK_RPC,

    *OID_MAKE_RPC,

    *DATA_CREATE_RPC,
    *DATA_READ_RPC,
    *DATA_WRITE_RPC,
    *DATA_DELETE_RPC,
    *DATA_SIZE_GET_RPC,
    *DATA_SIZE_MODIFY_RPC,

    *VDDATA_CREATE_RPC,
    *VDDATA_READ_RPC,
    *VDDATA_DELETE_RPC,

    *SCHEMA_COMPLETE_RPC,

    *ATTRIBUTE_INDEX_CREATE_RPC,
    *ATTRIBUTE_INDEX_REMOVE_RPC,

    *INDEX_CREATE_RPC,
    *INDEX_REMOVE_RPC,

    *CONSTRAINT_CREATE_RPC,
    *CONSTRAINT_DELETE_RPC,

    *COLLECTION_GET_BY_IND_RPC,
    *COLLECTION_GET_BY_VALUE_RPC,

    *SET_OBJECT_LOCK_RPC,
    *GET_OBJECT_LOCK_RPC,

    //*QUERY_LANG_CREATE_RPC,
    //*QUERY_DATABASE_CREATE_RPC,
    //*QUERY_CLASS_CREATE_RPC,
    *QUERY_COLLECTION_CREATE_RPC,
    *QUERY_ATTRIBUTE_CREATE_RPC,
    *QUERY_DELETE_RPC,
    *QUERY_SCAN_NEXT_RPC,

    *EXECUTABLE_CHECK_RPC,
    *EXECUTABLE_EXECUTE_RPC,
    *EXECUTABLE_SET_EXTREF_PATH_RPC,
    *EXECUTABLE_GET_EXTREF_PATH_RPC,

    *OQL_CREATE_RPC,
    *OQL_DELETE_RPC,
    *OQL_GETRESULT_RPC,

    *SET_CONN_INFO_RPC,
    *CHECK_AUTH_RPC,

    *SET_LOG_MASK_RPC,

    *INDEX_GET_COUNT_RPC,
    *INDEX_GET_STATS_RPC,
    *INDEX_SIMUL_STATS_RPC,
    *COLLECTION_GET_IMPLSTATS_RPC,
    *COLLECTION_SIMUL_IMPLSTATS_RPC,
    *INDEX_GET_IMPL_RPC,
    *COLLECTION_GET_IMPL_RPC,

    *GET_DEFAULT_DATASPACE_RPC,
    *SET_DEFAULT_DATASPACE_RPC,
    *DATASPACE_SET_CURRENT_DATAFILE_RPC,
    *DATASPACE_GET_CURRENT_DATAFILE_RPC,
    *GET_DEFAULT_INDEX_DATASPACE_RPC,
    *SET_DEFAULT_INDEX_DATASPACE_RPC,
    *GET_INDEX_LOCATIONS_RPC,
    *MOVE_INDEX_RPC,
    *GET_INSTANCE_CLASS_LOCATIONS_RPC,
    *MOVE_INSTANCE_CLASS_RPC,
    *GET_OBJECTS_LOCATIONS_RPC,
    *MOVE_OBJECTS_RPC,
    *GET_ATTRIBUTE_LOCATIONS_RPC,
    *MOVE_ATTRIBUTE_RPC,

    *CREATE_DATAFILE_RPC,
    *DELETE_DATAFILE_RPC,
    *MOVE_DATAFILE_RPC,
    *DEFRAGMENT_DATAFILE_RPC,
    *RESIZE_DATAFILE_RPC,
    *GET_DATAFILEI_NFO_RPC,
    *RENAME_DATAFILE_RPC,
    *CREATE_DATASPACE_RPC,
    *UPDATE_DATASPACE_RPC,
    *DELETE_DATASPACE_RPC,
    *RENAME_DATASPACE_RPC,
    *GET_SERVER_OUTOFBAND_DATA_RPC;

  static rpc_Server *server;

#ifdef MTHREADS
#include <synch.h>
  static mutex_t mp_sem;
#endif

  rpc_ArgType
  rpcDB_LocalDBContextType,
    OidType,
    /*  BoolType, */
    RPCStatusType;

  static void status_ua_server(rpc_Arg *, char **, void *,
			       rpc_SendRcv, rpc_FromTo);
  static void oid_ua_server(rpc_Arg *, char **, void *,
			    rpc_SendRcv, rpc_FromTo);

  /*#define STATUS_VARIABLE*/

  /*#define STATUS_SZ sizeof(RPCStatusRec)*/
  static int STATUS_SZ;

  extern Bool edb_is_back_end;

  extern void setConnInfo(rpc_ConnInfo *ci);

  static void
  b_init(int *fd, int fd_cnt, rpc_ConnInfo *ci)
  {
    extern void config_init();

    config_init();

    setConnInfo(ci);
    eyedbsm::mutexes_init();
  }

  static void
  b_release(rpc_ConnInfo *ci)
  {
    eyedbsm::mutexes_release();
  }

  static void
  begin(int which, void *data)
  {
    if (!which)
      IDB_backendInterruptReset();
  }

  rpc_Server *
  rpcBeInit()
  {
    rpc_ServerMode mode;

    edb_is_back_end = True;

    if (getenv("EYEDBDBX"))
      mode = rpc_MonoProc;
    else
      mode = rpc_MultiProcs;

    server = rpc_serverCreate(mode, RPC_PROTOCOL_MAGIC,
			      CONN_COUNT, 0, b_init,
			      b_release, begin, 0, 0);

    STATUS_SZ = getenv("STATUS_SZ") ? atoi(getenv("STATUS_SZ")) :
      sizeof(RPCStatusRec);

    /*printf("STATUS_SZ server is %d\n", STATUS_SZ);*/

    /* make types */
    rpcDB_LocalDBContextType = rpc_makeServerUserType(server, sizeof(rpcDB_LocalDBContext), 0);
    /*  BoolType = rpc_makeServerUserType(server, sizeof(Bool), 0); */
    OidType = rpc_makeServerUserType(server, sizeof(eyedbsm::Oid), oid_ua_server);
#ifdef STATUS_VARIABLE
    RPCStatusType = rpc_makeServerUserType(server, rpc_SizeVariable, status_ua_server);
#else
    RPCStatusType = rpc_makeServerUserType(server, STATUS_SZ, status_ua_server);
#endif

    /* make functions */

    DBCREATE_RPC =
      rpc_makeUserServerFunction(server, makeDBCREATE(), DBCREATE_realize);

    DBDELETE_RPC =
      rpc_makeUserServerFunction(server, makeDBDELETE(), DBDELETE_realize);

    DBINFO_RPC =
      rpc_makeUserServerFunction(server, makeDBINFO(), DBINFO_realize);

    DBMOVE_RPC =
      rpc_makeUserServerFunction(server, makeDBMOVE(), DBMOVE_realize);

    DBCOPY_RPC =
      rpc_makeUserServerFunction(server, makeDBCOPY(), DBCOPY_realize);

    DBRENAME_RPC =
      rpc_makeUserServerFunction(server, makeDBRENAME(), DBRENAME_realize);

    DBMCREATE_RPC =
      rpc_makeUserServerFunction(server, makeDBMCREATE(), DBMCREATE_realize);

    DBMUPDATE_RPC =
      rpc_makeUserServerFunction(server, makeDBMUPDATE(), DBMUPDATE_realize);

    USER_ADD_RPC =
      rpc_makeUserServerFunction(server, makeUSER_ADD(), USER_ADD_realize);

    USER_DELETE_RPC =
      rpc_makeUserServerFunction(server, makeUSER_DELETE(), USER_DELETE_realize);

    USER_PASSWD_SET_RPC =
      rpc_makeUserServerFunction(server, makeUSER_PASSWD_SET(), USER_PASSWD_SET_realize);

    PASSWD_SET_RPC =
      rpc_makeUserServerFunction(server, makePASSWD_SET(), PASSWD_SET_realize);

    DEFAULT_DBACCESS_SET_RPC =
      rpc_makeUserServerFunction(server, makeDEFAULT_DBACCESS_SET(), DEFAULT_DBACCESS_SET_realize);

    USER_DBACCESS_SET_RPC =
      rpc_makeUserServerFunction(server, makeUSER_DBACCESS_SET(), USER_DBACCESS_SET_realize);

    USER_SYSACCESS_SET_RPC =
      rpc_makeUserServerFunction(server, makeUSER_SYSACCESS_SET(), USER_SYSACCESS_SET_realize);

    BACKEND_INTERRUPT_RPC =
      rpc_makeUserServerFunction(server, makeBACKEND_INTERRUPT(), BACKEND_INTERRUPT_realize);

    TRANSACTION_BEGIN_RPC =
      rpc_makeUserServerFunction(server, makeTRANSACTION_BEGIN(),
				 TRANSACTION_BEGIN_realize);

    TRANSACTION_COMMIT_RPC =
      rpc_makeUserServerFunction(server, makeTRANSACTION_COMMIT(),
				 TRANSACTION_COMMIT_realize);

    TRANSACTION_ABORT_RPC =
      rpc_makeUserServerFunction(server, makeTRANSACTION_ABORT(),
				 TRANSACTION_ABORT_realize);

    TRANSACTION_PARAMS_SET_RPC =
      rpc_makeUserServerFunction(server, makeTRANSACTION_PARAMS_SET(),
				 TRANSACTION_PARAMS_SET_realize);

    TRANSACTION_PARAMS_GET_RPC =
      rpc_makeUserServerFunction(server, makeTRANSACTION_PARAMS_GET(),
				 TRANSACTION_PARAMS_GET_realize);

    DBOPEN_RPC =
      rpc_makeUserServerFunction(server, makeDBOPEN(), DBOPEN_realize);

    DBOPENLOCAL_RPC =
      rpc_makeUserServerFunction(server, makeDBOPENLOCAL(), DBOPENLOCAL_realize);

    DBCLOSE_RPC =
      rpc_makeUserServerFunction(server, makeDBCLOSE(), DBCLOSE_realize);

    OBJECT_CREATE_RPC =
      rpc_makeUserServerFunction(server, makeOBJECT_CREATE(),
				 OBJECT_CREATE_realize);

    OBJECT_WRITE_RPC =
      rpc_makeUserServerFunction(server, makeOBJECT_WRITE(),
				 OBJECT_WRITE_realize);

    OBJECT_READ_RPC =
      rpc_makeUserServerFunction(server, makeOBJECT_READ(),
				 OBJECT_READ_realize);

    OBJECT_DELETE_RPC =
      rpc_makeUserServerFunction(server, makeOBJECT_DELETE(),
				 OBJECT_DELETE_realize);

    OBJECT_HEADER_READ_RPC =
      rpc_makeUserServerFunction(server, makeOBJECT_HEADER_READ(),
				 OBJECT_HEADER_READ_realize);

    OBJECT_SIZE_MODIFY_RPC =
      rpc_makeUserServerFunction(server, makeOBJECT_SIZE_MODIFY(),
				 OBJECT_SIZE_MODIFY_realize);

    OBJECT_CHECK_RPC =
      rpc_makeUserServerFunction(server, makeOBJECT_CHECK(),
				 OBJECT_CHECK_realize);

    OBJECT_PROTECTION_SET_RPC =
      rpc_makeUserServerFunction(server, makeOBJECT_PROTECTION_SET(),
				 OBJECT_PROTECTION_SET_realize);

    OBJECT_PROTECTION_GET_RPC =
      rpc_makeUserServerFunction(server, makeOBJECT_PROTECTION_GET(),
				 OBJECT_PROTECTION_GET_realize);

    OID_MAKE_RPC =
      rpc_makeUserServerFunction(server, makeOID_MAKE(),
				 OID_MAKE_realize);

    DATA_CREATE_RPC =
      rpc_makeUserServerFunction(server, makeDATA_CREATE(),
				 DATA_CREATE_realize);

    DATA_WRITE_RPC =
      rpc_makeUserServerFunction(server, makeDATA_WRITE(),
				 DATA_WRITE_realize);

    DATA_READ_RPC =
      rpc_makeUserServerFunction(server, makeDATA_READ(),
				 DATA_READ_realize);

    DATA_DELETE_RPC =
      rpc_makeUserServerFunction(server, makeDATA_DELETE(),
				 DATA_DELETE_realize);

    DATA_SIZE_GET_RPC =
      rpc_makeUserServerFunction(server, makeDATA_SIZE_GET(),
				 DATA_SIZE_GET_realize);

    DATA_SIZE_MODIFY_RPC =
      rpc_makeUserServerFunction(server, makeOBJECT_SIZE_MODIFY(),
				 DATA_SIZE_MODIFY_realize);

    VDDATA_CREATE_RPC =
      rpc_makeUserServerFunction(server, makeVDDATA_CREATE(),
				 VDDATA_CREATE_realize);

    VDDATA_WRITE_RPC =
      rpc_makeUserServerFunction(server, makeVDDATA_WRITE(),
				 VDDATA_WRITE_realize);

    VDDATA_DELETE_RPC =
      rpc_makeUserServerFunction(server, makeVDDATA_DELETE(),
				 VDDATA_DELETE_realize);

    SCHEMA_COMPLETE_RPC =
      rpc_makeUserServerFunction(server, makeSCHEMA_COMPLETE(),
				 SCHEMA_COMPLETE_realize);

    ATTRIBUTE_INDEX_CREATE_RPC =
      rpc_makeUserServerFunction(server, makeATTRIBUTE_INDEX_CREATE(),
				 ATTRIBUTE_INDEX_CREATE_realize);

    ATTRIBUTE_INDEX_REMOVE_RPC =
      rpc_makeUserServerFunction(server, makeATTRIBUTE_INDEX_REMOVE(),
				 ATTRIBUTE_INDEX_REMOVE_realize);

    INDEX_CREATE_RPC =
      rpc_makeUserServerFunction(server, makeINDEX_CREATE(), INDEX_CREATE_realize);

    INDEX_REMOVE_RPC =
      rpc_makeUserServerFunction(server, makeINDEX_REMOVE(), INDEX_REMOVE_realize);

    CONSTRAINT_CREATE_RPC =
      rpc_makeUserServerFunction(server, makeCONSTRAINT_CREATE(), CONSTRAINT_CREATE_realize);

    CONSTRAINT_DELETE_RPC =
      rpc_makeUserServerFunction(server, makeCONSTRAINT_DELETE(), CONSTRAINT_DELETE_realize);

    COLLECTION_GET_BY_IND_RPC =
      rpc_makeUserServerFunction(server, makeCOLLECTION_GET_BY_IND(),
				 COLLECTION_GET_BY_IND_realize);

    COLLECTION_GET_BY_VALUE_RPC =
      rpc_makeUserServerFunction(server, makeCOLLECTION_GET_BY_VALUE(),
				 COLLECTION_GET_BY_VALUE_realize);

    SET_OBJECT_LOCK_RPC =
      rpc_makeUserServerFunction(server, makeSET_OBJECT_LOCK(), SET_OBJECT_LOCK_realize);

    GET_OBJECT_LOCK_RPC =
      rpc_makeUserServerFunction(server, makeGET_OBJECT_LOCK(), GET_OBJECT_LOCK_realize);

    /*
    QUERY_LANG_CREATE_RPC =
      rpc_makeUserServerFunction(server, makeQUERY_LANG_CREATE(),
				 QUERY_LANG_CREATE_realize);

    QUERY_DATABASE_CREATE_RPC =
      rpc_makeUserServerFunction(server, makeQUERY_DATABASE_CREATE(),
				 QUERY_DATABASE_CREATE_realize);

    QUERY_CLASS_CREATE_RPC =
      rpc_makeUserServerFunction(server, makeQUERY_CLASS_CREATE(),
				 QUERY_CLASS_CREATE_realize);
    */

    QUERY_COLLECTION_CREATE_RPC =
      rpc_makeUserServerFunction(server, makeQUERY_COLLECTION_CREATE(),
				 QUERY_COLLECTION_CREATE_realize);

    QUERY_ATTRIBUTE_CREATE_RPC =
      rpc_makeUserServerFunction(server, makeQUERY_ATTRIBUTE_CREATE(),
				 QUERY_ATTRIBUTE_CREATE_realize);

    QUERY_DELETE_RPC =
      rpc_makeUserServerFunction(server, makeQUERY_DELETE(),
				 QUERY_DELETE_realize);

    QUERY_SCAN_NEXT_RPC =
      rpc_makeUserServerFunction(server, makeQUERY_SCAN_NEXT(),
				 QUERY_SCAN_NEXT_realize);

    EXECUTABLE_CHECK_RPC = 
      rpc_makeUserServerFunction(server, makeEXECUTABLE_CHECK(),
				 EXECUTABLE_CHECK_realize);

    EXECUTABLE_EXECUTE_RPC = 
      rpc_makeUserServerFunction(server, makeEXECUTABLE_EXECUTE(),
				 EXECUTABLE_EXECUTE_realize);

    EXECUTABLE_SET_EXTREF_PATH_RPC =
      rpc_makeUserServerFunction(server,
				 makeEXECUTABLE_SET_EXTREF_PATH(),
				 EXECUTABLE_SET_EXTREF_PATH_realize);

    EXECUTABLE_GET_EXTREF_PATH_RPC =
      rpc_makeUserServerFunction(server,
				 makeEXECUTABLE_GET_EXTREF_PATH(),
				 EXECUTABLE_GET_EXTREF_PATH_realize);

    OQL_CREATE_RPC =
      rpc_makeUserServerFunction(server, makeOQL_CREATE(), OQL_CREATE_realize);

    OQL_DELETE_RPC =
      rpc_makeUserServerFunction(server, makeOQL_DELETE(), OQL_DELETE_realize);

    OQL_GETRESULT_RPC =
      rpc_makeUserServerFunction(server, makeOQL_GETRESULT(), OQL_GETRESULT_realize);

    SET_CONN_INFO_RPC =
      rpc_makeUserServerFunction(server, makeSET_CONN_INFO(),
				 SET_CONN_INFO_realize);

    CHECK_AUTH_RPC =
      rpc_makeUserServerFunction(server, makeCHECK_AUTH(), CHECK_AUTH_realize);

    SET_LOG_MASK_RPC =
      rpc_makeUserServerFunction(server, makeSET_LOG_MASK(), SET_LOG_MASK_realize);

    INDEX_GET_COUNT_RPC =
      rpc_makeUserServerFunction(server, makeINDEX_GET_COUNT(), INDEX_GET_COUNT_realize);

    INDEX_GET_STATS_RPC =
      rpc_makeUserServerFunction(server, makeINDEX_GET_STATS(), INDEX_GET_STATS_realize);

    INDEX_SIMUL_STATS_RPC =
      rpc_makeUserServerFunction(server, makeINDEX_SIMUL_STATS(), INDEX_SIMUL_STATS_realize);

    COLLECTION_GET_IMPLSTATS_RPC =
      rpc_makeUserServerFunction(server, makeCOLLECTION_GET_IMPLSTATS(), COLLECTION_GET_IMPLSTATS_realize);

    COLLECTION_SIMUL_IMPLSTATS_RPC =
      rpc_makeUserServerFunction(server, makeCOLLECTION_SIMUL_IMPLSTATS(), COLLECTION_SIMUL_IMPLSTATS_realize);

    INDEX_GET_IMPL_RPC =
      rpc_makeUserServerFunction(server, makeINDEX_GET_IMPL(), INDEX_GET_IMPL_realize);

    COLLECTION_GET_IMPL_RPC =
      rpc_makeUserServerFunction(server, makeCOLLECTION_GET_IMPL(), COLLECTION_GET_IMPL_realize);

    GET_DEFAULT_DATASPACE_RPC =
      rpc_makeUserServerFunction(server, makeGET_DEFAULT_DATASPACE(), GET_DEFAULT_DATASPACE_realize);

    SET_DEFAULT_DATASPACE_RPC =
      rpc_makeUserServerFunction(server, makeSET_DEFAULT_DATASPACE(), SET_DEFAULT_DATASPACE_realize);

    DATASPACE_SET_CURRENT_DATAFILE_RPC =
      rpc_makeUserServerFunction(server, makeDATASPACE_SET_CURRENT_DATAFILE(), DATASPACE_SET_CURRENT_DATAFILE_realize);

    DATASPACE_GET_CURRENT_DATAFILE_RPC =
      rpc_makeUserServerFunction(server, makeDATASPACE_GET_CURRENT_DATAFILE(), DATASPACE_GET_CURRENT_DATAFILE_realize);

    GET_DEFAULT_INDEX_DATASPACE_RPC =
      rpc_makeUserServerFunction(server, makeGET_DEFAULT_INDEX_DATASPACE(), GET_DEFAULT_INDEX_DATASPACE_realize);

    SET_DEFAULT_INDEX_DATASPACE_RPC =
      rpc_makeUserServerFunction(server, makeSET_DEFAULT_INDEX_DATASPACE(), SET_DEFAULT_INDEX_DATASPACE_realize);

    GET_INDEX_LOCATIONS_RPC =
      rpc_makeUserServerFunction(server, makeGET_INDEX_LOCATIONS(), GET_INDEX_LOCATIONS_realize);

    MOVE_INDEX_RPC =
      rpc_makeUserServerFunction(server, makeMOVE_INDEX(), MOVE_INDEX_realize);

    GET_INSTANCE_CLASS_LOCATIONS_RPC =
      rpc_makeUserServerFunction(server, makeGET_INSTANCE_CLASS_LOCATIONS(), GET_INSTANCE_CLASS_LOCATIONS_realize);

    MOVE_INSTANCE_CLASS_RPC =
      rpc_makeUserServerFunction(server, makeMOVE_INSTANCE_CLASS(), MOVE_INSTANCE_CLASS_realize);

    GET_OBJECTS_LOCATIONS_RPC =
      rpc_makeUserServerFunction(server, makeGET_OBJECTS_LOCATIONS(), GET_OBJECTS_LOCATIONS_realize);

    MOVE_OBJECTS_RPC =
      rpc_makeUserServerFunction(server, makeMOVE_OBJECTS(), MOVE_OBJECTS_realize);

    GET_ATTRIBUTE_LOCATIONS_RPC =
      rpc_makeUserServerFunction(server, makeGET_ATTRIBUTE_LOCATIONS(), GET_ATTRIBUTE_LOCATIONS_realize);

    MOVE_ATTRIBUTE_RPC =
      rpc_makeUserServerFunction(server, makeMOVE_ATTRIBUTE(), MOVE_ATTRIBUTE_realize);

    CREATE_DATAFILE_RPC =
      rpc_makeUserServerFunction(server, makeCREATE_DATAFILE(), CREATE_DATAFILE_realize);

    DELETE_DATAFILE_RPC =
      rpc_makeUserServerFunction(server, makeDELETE_DATAFILE(), DELETE_DATAFILE_realize);

    MOVE_DATAFILE_RPC =
      rpc_makeUserServerFunction(server, makeMOVE_DATAFILE(), MOVE_DATAFILE_realize);

    DEFRAGMENT_DATAFILE_RPC =
      rpc_makeUserServerFunction(server, makeDEFRAGMENT_DATAFILE(), DEFRAGMENT_DATAFILE_realize);

    RESIZE_DATAFILE_RPC =
      rpc_makeUserServerFunction(server, makeRESIZE_DATAFILE(), RESIZE_DATAFILE_realize);

    GET_DATAFILEI_NFO_RPC =
      rpc_makeUserServerFunction(server, makeGET_DATAFILEI_NFO(), GET_DATAFILEI_NFO_realize);

    RENAME_DATAFILE_RPC =
      rpc_makeUserServerFunction(server, makeRENAME_DATAFILE(), RENAME_DATAFILE_realize);

    CREATE_DATASPACE_RPC =
      rpc_makeUserServerFunction(server, makeCREATE_DATASPACE(), CREATE_DATASPACE_realize);

    UPDATE_DATASPACE_RPC =
      rpc_makeUserServerFunction(server, makeUPDATE_DATASPACE(), UPDATE_DATASPACE_realize);

    DELETE_DATASPACE_RPC =
      rpc_makeUserServerFunction(server, makeDELETE_DATASPACE(), DELETE_DATASPACE_realize);

    RENAME_DATASPACE_RPC =
      rpc_makeUserServerFunction(server, makeRENAME_DATASPACE(), RENAME_DATASPACE_realize);

    GET_SERVER_OUTOFBAND_DATA_RPC =
      rpc_makeUserServerFunction(server, makeGET_SERVER_OUTOFBAND_DATA(), GET_SERVER_OUTOFBAND_DATA_realize);

    /* declare arg size */
    rpc_setServerArgSize(server, sizeof(ServerArg));

    /* add handler */
    (void)rpc_setConnectionHandler(server, connection_handler);

#ifdef MTHREADS
    rpcDB_mutexInit();
    mutex_init(&mp_sem, USYNC_PROCESS, 0);
#endif
    eyedbsm::init();
    be_init();
    return server;
  }

  rpc_Server *
  getRpcServer()
  {
    return server;
  }

  static const char *sePort;

  void
  setSePort(const char *seport)
  {
    sePort = seport;
  }

  const char *
  getSePort(void)
  {
    return sePort;
  }

  extern pid_t getpid(void);
  void
  connection_handler(rpc_Server *server, rpc_ClientId clientid, rpc_Boolean isnew)
  {
    rpcDB_ClientInfo *ci = rpcDB_clientInfoGet(clientid);

    /*
      fprintf(stderr, "[thread %d#%d] idbd: connection_handler fd=%d, isnew=%d\n",
      getpid(), pthread_self(), clientid, isnew);

    */
    if (isnew)
      {
	/*
	  rpc_Client *client = se_rpcFeInit();
	  //se_ConnHandle *conn;
	  eyedbsm::Status status;
	*/
	ConnHandle *idbc =  rpc_new(ConnHandle);
	//idbc->sech = conn;
	ci->user_data = idbc;
      }
    else
      {
	int i;
	RPCStatus status;
	register rpcDB_ClientInfo *ci = rpcDB_clientInfoGet(clientid);
	rpcDB_DbHandleClientInfo *dbhclientinfo;
	extern void idbRelease(void);

	// disconnected 15/02/06 because of a freeze
	// question : these calls were really necessary !?
#if 1
	for (i = 0; i < RPCDB_MAX_DBH; i++)
	  rpcDB_close_do(server, &ci->dbhclientinfo[i], close_realize, (void **)&status);
#endif

	idbRelease();

	/*
	  if (ci->user_data)
	  se_connClose(((ConnHandle *)ci->user_data)->sech);
	*/
      }
  }

#include "eyedblib/rpc_lib.h"

#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))

  static void
  status_ua_server(rpc_Arg *arg, char **pbuff, void *pua,
		   rpc_SendRcv send_rcv, rpc_FromTo fromto)
  {
    RPCStatusRec *s = (RPCStatusRec *)pua;
    char *buff = *pbuff;

    rpc_copy_fast_xdr(arg, buff, &s->err, sizeof(eyedblib::int32), send_rcv, fromto,
		      x2h_32_cpy, h2x_32_cpy);

    if ((arg->send_rcv & rpc_Send) && fromto == rpc_From)
      assert(0);
    else if ((arg->send_rcv & rpc_Rcv) && fromto == rpc_To)
      {
#ifdef STATUS_VARIABLE
	strcpy(buff, s->err_msg);
	printf("sending status '%s'\n", buff);
	*pbuff += sizeof(eyedblib::int32)+strlen(buff)+1;
#else
	strncpy(buff, s->err_msg, STATUS_SZ-4);
	buff[MIN(STATUS_SZ-4, strlen(s->err_msg))] = 0;
	*pbuff += STATUS_SZ;
#endif
      }
  }

  static void
  oid_ua_server(rpc_Arg *arg, char **pbuff, void *pua,
		rpc_SendRcv send_rcv, rpc_FromTo fromto)
  {
    eyedbsm::Oid oid;

    if (send_rcv & arg->send_rcv) {
      if (fromto == rpc_To) {
	memcpy(&oid, pua, sizeof(oid));
	eyedbsm::h2x_oid(&oid, &oid);
	memcpy(*pbuff, &oid, sizeof(oid));
      }
      else {
	memcpy(&oid, *pbuff, sizeof(oid));
	eyedbsm::x2h_oid(&oid, &oid);
	memcpy(pua, &oid, sizeof(oid));
      }
      *pbuff += sizeof(eyedbsm::Oid);
    }
  }
}
