/* 
   EyeDB Object Database Management System
   Copyright (C) 1994-1999,2004,2005 SYSRA
   
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


#ifndef _eyedb_idblib_be_
#define _eyedb_idblib_be_

#include <eyedblib/rpc_be.h>
#include <eyedblib/rpc_fe.h>
#include "rpc_lib.h"
#include <eyedblib/rpcdb_be.h>

namespace eyedb {

  typedef union {
    eyedblib::int32 a_int;
    eyedblib::int64 a_int64;
    char *a_string;
    rpc_ServerData a_data;

    /* specific idb */
    rpcDB_LocalDBContext a_ldbctx;
    eyedbsm::Oid a_oid;
    Bool a_bool;
    RPCStatusRec a_status;
  } ServerArg;

  extern rpc_ServerFunction
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

    *MAKE_OID_RPC,

    *DATA_CREATE_RPC,
    *DATA_READ_RPC,
    *DATA_WRITE_RPC,
    *DATA_DELETE_RPC,
    *DATA_SIZE_GET_RPC,
    *DATA_SIZE_MODIFY_RPC,

    *VDDATA_CREATE_RPC,
    *VDDATA_WRITE_RPC,
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

    *QUERY_LANG_CREATE_RPC,
    *QUERY_DATABASE_CREATE_RPC,
    *QUERY_CLASS_CREATE_RPC,
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

  extern rpc_Server *
  rpcBeInit(void);

  extern rpc_Server *
  getRpcServer(void);

  extern void
  connection_handler(rpc_Server *, rpc_ClientId, rpc_Boolean);

  extern void
  DBCREATE_realize(rpc_ClientId, void *);

  extern void
  DBINFO_realize(rpc_ClientId, void *);

  extern void
  DBDELETE_realize(rpc_ClientId, void *);

  extern void
  DBMOVE_realize(rpc_ClientId, void *);

  extern void
  DBCOPY_realize(rpc_ClientId, void *);

  extern void
  DBRENAME_realize(rpc_ClientId, void *);

  extern void
  DBMCREATE_realize(rpc_ClientId, void *);

  extern void
  DBMUPDATE_realize(rpc_ClientId, void *);

  extern void
  USER_ADD_realize(rpc_ClientId, void *);

  extern void
  USER_DELETE_realize(rpc_ClientId, void *);

  extern void
  USER_PASSWD_SET_realize(rpc_ClientId, void *);

  extern void
  PASSWD_SET_realize(rpc_ClientId, void *);

  extern void
  DEFAULT_DBACCESS_SET_realize(rpc_ClientId, void *);

  extern void
  USER_DBACCESS_SET_realize(rpc_ClientId, void *);

  extern void
  USER_SYSACCESS_SET_realize(rpc_ClientId, void *);

  extern void
  BACKEND_INTERRUPT_realize(rpc_ClientId, void *);

  extern void
  TRANSACTION_BEGIN_realize(rpc_ClientId, void *);

  extern void
  TRANSACTION_COMMIT_realize(rpc_ClientId, void *);

  extern void
  TRANSACTION_ABORT_realize(rpc_ClientId, void *);

  extern void
  TRANSACTION_PARAMS_SET_realize(rpc_ClientId, void *);

  extern void
  TRANSACTION_PARAMS_GET_realize(rpc_ClientId, void *);

  extern void
  DBOPEN_realize(rpc_ClientId, void *);

  extern void
  DBOPENLOCAL_realize(rpc_ClientId, void *);

  extern void
  DBCLOSE_realize(rpc_ClientId, void *);

  /*
    extern void
    SCHEMA_CREATE_realize(rpc_ClientId, void *);

    extern void
    CLASS_CREATE_realize(rpc_ClientId, void *);

    extern void
    CLASS_UPDATE_realize(rpc_ClientId, void *);

    extern void
    CLASS_LOAD_realize(rpc_ClientId, void *);

    extern void
    CLASS_DELETE_realize(rpc_ClientId, void *);
  */

  extern void
  OBJECT_CREATE_realize(rpc_ClientId, void *);

  extern void
  OBJECT_READ_realize(rpc_ClientId, void *);

  extern void
  OBJECT_WRITE_realize(rpc_ClientId, void *);

  extern void
  OBJECT_DELETE_realize(rpc_ClientId, void *);

  extern void
  OBJECT_HEADER_READ_realize(rpc_ClientId, void *);

  extern void
  OBJECT_SIZE_MODIFY_realize(rpc_ClientId, void *);

  extern void
  OBJECT_CHECK_realize(rpc_ClientId, void *);

  extern void
  OBJECT_PROTECTION_SET_realize(rpc_ClientId, void *);

  extern void
  OBJECT_PROTECTION_GET_realize(rpc_ClientId, void *);

  extern void
  OID_MAKE_realize(rpc_ClientId, void *);

  extern void
  DATA_CREATE_realize(rpc_ClientId, void *);

  extern void
  DATA_READ_realize(rpc_ClientId, void *);

  extern void
  DATA_WRITE_realize(rpc_ClientId, void *);

  extern void
  DATA_DELETE_realize(rpc_ClientId, void *);

  extern void
  DATA_SIZE_GET_realize(rpc_ClientId, void *);

  extern void
  DATA_SIZE_MODIFY_realize(rpc_ClientId, void *);

  extern void
  VDDATA_CREATE_realize(rpc_ClientId, void *);

  extern void
  VDDATA_WRITE_realize(rpc_ClientId, void *);

  extern void
  VDDATA_DELETE_realize(rpc_ClientId, void *);

  extern void
  SCHEMA_COMPLETE_realize(rpc_ClientId, void *);

  extern void
  ATTRIBUTE_INDEX_CREATE_realize(rpc_ClientId, void *);

  extern void
  ATTRIBUTE_INDEX_REMOVE_realize(rpc_ClientId, void *);

  extern void
  INDEX_CREATE_realize(rpc_ClientId, void *);

  extern void
  INDEX_REMOVE_realize(rpc_ClientId, void *);

  extern void
  CONSTRAINT_CREATE_realize(rpc_ClientId, void *);

  extern void
  CONSTRAINT_DELETE_realize(rpc_ClientId, void *);

  extern void
  COLLECTION_GET_BY_IND_realize(rpc_ClientId, void *);

  extern void
  COLLECTION_GET_BY_VALUE_realize(rpc_ClientId, void *);

  extern void
  SET_OBJECT_LOCK_realize(rpc_ClientId, void *);

  extern void
  GET_OBJECT_LOCK_realize(rpc_ClientId, void *);

  extern void
  QUERY_LANG_CREATE_realize(rpc_ClientId, void *);

  extern void
  QUERY_DATABASE_CREATE_realize(rpc_ClientId, void *);

  extern void
  QUERY_CLASS_CREATE_realize(rpc_ClientId, void *);

  extern void
  QUERY_COLLECTION_CREATE_realize(rpc_ClientId, void *);

  extern void
  QUERY_ATTRIBUTE_CREATE_realize(rpc_ClientId, void *);

  extern void
  QUERY_DELETE_realize(rpc_ClientId, void *);

  extern void
  QUERY_SCAN_NEXT_realize(rpc_ClientId, void *);

  /* executables */
  extern void
  EXECUTABLE_CHECK_realize(rpc_ClientId, void *);

  extern void
  EXECUTABLE_EXECUTE_realize(rpc_ClientId, void *);

  extern void
  EXECUTABLE_SET_EXTREF_PATH_realize(rpc_ClientId, void *);

  extern void
  EXECUTABLE_GET_EXTREF_PATH_realize(rpc_ClientId, void *);

  extern void
  OQL_CREATE_realize(rpc_ClientId, void *);

  extern void
  OQL_DELETE_realize(rpc_ClientId, void *);

  extern void
  OQL_GETRESULT_realize(rpc_ClientId, void *);

  extern void
  SET_CONN_INFO_realize(rpc_ClientId, void *);

  extern void
  CHECK_AUTH_realize(rpc_ClientId, void *);

  extern void
  SET_LOG_MASK_realize(rpc_ClientId, void *);

  extern void
  INDEX_GET_COUNT_realize(rpc_ClientId, void *);

  extern void
  INDEX_GET_STATS_realize(rpc_ClientId, void *);

  extern void
  INDEX_SIMUL_STATS_realize(rpc_ClientId, void *);

  extern void
  COLLECTION_GET_IMPLSTATS_realize(rpc_ClientId, void *);

  extern void
  COLLECTION_SIMUL_IMPLSTATS_realize(rpc_ClientId, void *);

  extern void
  INDEX_GET_IMPL_realize(rpc_ClientId, void *);

  extern void
  COLLECTION_GET_IMPL_realize(rpc_ClientId, void *);

  extern void
  GET_DEFAULT_DATASPACE_realize(rpc_ClientId, void *);

  extern void
  SET_DEFAULT_DATASPACE_realize(rpc_ClientId, void *);

  extern void
  DATASPACE_SET_CURRENT_DATAFILE_realize(rpc_ClientId, void *);

  extern void
  DATASPACE_GET_CURRENT_DATAFILE_realize(rpc_ClientId, void *);

  extern void
  GET_DEFAULT_INDEX_DATASPACE_realize(rpc_ClientId, void *);

  extern void
  SET_DEFAULT_INDEX_DATASPACE_realize(rpc_ClientId, void *);

  extern void
  GET_INDEX_LOCATIONS_realize(rpc_ClientId, void *);

  extern void
  MOVE_INDEX_realize(rpc_ClientId, void *);

  extern void
  GET_INSTANCE_CLASS_LOCATIONS_realize(rpc_ClientId, void *);

  extern void
  MOVE_INSTANCE_CLASS_realize(rpc_ClientId, void *);

  extern void
  GET_OBJECTS_LOCATIONS_realize(rpc_ClientId, void *);

  extern void
  MOVE_OBJECTS_realize(rpc_ClientId, void *);

  extern void
  GET_ATTRIBUTE_LOCATIONS_realize(rpc_ClientId, void *);

  extern void
  MOVE_ATTRIBUTE_realize(rpc_ClientId, void *);

  extern void
  CREATE_DATAFILE_realize(rpc_ClientId, void *);

  extern void
  DELETE_DATAFILE_realize(rpc_ClientId, void *);

  extern void
  MOVE_DATAFILE_realize(rpc_ClientId, void *);

  extern void
  DEFRAGMENT_DATAFILE_realize(rpc_ClientId, void *);

  extern void
  RESIZE_DATAFILE_realize(rpc_ClientId, void *);

  extern void
  GET_DATAFILEI_NFO_realize(rpc_ClientId, void *);

  extern void
  RENAME_DATAFILE_realize(rpc_ClientId, void *);

  extern void
  CREATE_DATASPACE_realize(rpc_ClientId, void *);

  extern void
  UPDATE_DATASPACE_realize(rpc_ClientId, void *);

  extern void
  DELETE_DATASPACE_realize(rpc_ClientId, void *);

  extern void
  RENAME_DATASPACE_realize(rpc_ClientId, void *);

  extern void
  GET_SERVER_OUTOFBAND_DATA_realize(rpc_ClientId, void *);

  extern void
  setSePort(const char *);

  extern const char *
  getSePort(void);

  extern void *
  close_realize(rpcDB_DbHandleClientInfo *);

  extern void
  GARBAGE();

  extern void
  be_init();
}

#endif
