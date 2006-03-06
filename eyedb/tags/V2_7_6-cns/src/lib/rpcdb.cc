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

#include "eyedbconfig.h"

#include <stdlib.h>
#include <stdio.h>

#include <eyedblib/rpc.h>
#include <eyedblib/rpcdb.h>
#include <eyedblib/rpc_be.h>
#include <eyedblib/rpcdb_be.h>

/*@@@@/*why is SOLARIS APART ??*/

#if defined(SOLARIS) || defined(ULTRASOL7)
#include <synch.h>
#define MUTEX_T mutex_t
#define MUTEX_LOCK mutex_lock
#define MUTEX_UNLOCK mutex_unlock
#endif

#if defined(LINUX) || defined(LINUX64) || defined(LINUX_IA64) || defined(LINUX_PPC64) || defined(ORIGIN) || defined(ALPHA)  || defined(AIX) || defined(CYGWIN)
#include <pthread.h>
#define MUTEX_T pthread_mutex_t
#define MUTEX_LOCK pthread_mutex_lock
#define MUTEX_UNLOCK pthread_mutex_unlock
#endif

static MUTEX_T mp_dbhinfo, mp_sem;

void rpcDB_mutexInit(void)
{
  /*@@@@*/
#if defined(SOLARIS) || defined(ULTRASOL7)
  mutex_init(&mp_dbhinfo, USYNC_PROCESS, 0);
  mutex_init(&mp_sem, USYNC_PROCESS, 0);
#endif
#if defined(LINUX) || defined(LINUX64) || defined(LINUX_IA64) || defined(LINUX_PPC64) || defined(ORIGIN) || defined(ALPHA) || defined(CYGWIN)
  pthread_mutexattr_t mattr;

  pthread_mutexattr_init(&mattr);
  pthread_mutex_init(&mp_dbhinfo, &mattr);
  pthread_mutex_init(&mp_sem, &mattr);
#endif
}

int
rpcDB_getDbhId()
{
  /* THERE IS A GREAT PROBLEM, DUE TO CONCURRENCE IN LOCAL ACCESS ! */
  /* this dbhid is used both for XID and RHDBID */
#if 1
  static int dbhid = 1000;
  return dbhid++;
#else
  static int dbhid = 1;
  /* this kludge ensure that there is no concurency problem */
  /* BUT THE PROBLEM IS THAT the transaction XID are not the
     same in the server and in the client in case of a local opening! */
  return 65536 + getpid() + dbhid++;
#endif
}

#define MAX_DBHINFO 128

static rpcDB_DbHandleInfo *rpcDB_dbhInfo[MAX_DBHINFO];

rpcDB_DbHandleInfo *
rpcDB_dbhinfoNew(int id)
{
  int i;

  MUTEX_LOCK(&mp_dbhinfo);
  for (i = 0; i < MAX_DBHINFO; i++)
    if (!rpcDB_dbhInfo[i])
      {
	rpcDB_dbhInfo[i] = (rpcDB_DbHandleInfo *)calloc(sizeof(rpcDB_DbHandleInfo), 1);
	rpcDB_dbhInfo[i]->id = id;
	rpcDB_dbhInfo[i]->refcnt = 1;
	MUTEX_UNLOCK(&mp_dbhinfo);
	return rpcDB_dbhInfo[i];
      }
  MUTEX_UNLOCK(&mp_dbhinfo);
  return 0;
}

rpcDB_DbHandleInfo *
rpcDB_dbhinfoGet(int id)
{
  int i;

  MUTEX_LOCK(&mp_dbhinfo);
  for (i = 0; i < MAX_DBHINFO; i++)
    if (rpcDB_dbhInfo[i] && rpcDB_dbhInfo[i]->id == id)
      {
	rpcDB_dbhInfo[i]->refcnt++;
#ifdef TRACE
	printf("incrementing dbhinfo %d\n", rpcDB_dbhInfo[i]->refcnt);
#endif
	MUTEX_UNLOCK(&mp_dbhinfo);
	return rpcDB_dbhInfo[i];
      }
  MUTEX_UNLOCK(&mp_dbhinfo);
  return 0;
}

rpcDB_ClientInfo *
rpcDB_clientInfoGet(rpc_ClientId clientid)
{
#if 0
  static rpcDB_ClientInfo rpcDB_clientsInfo[512];
  return &rpcDB_clientsInfo[(int)clientid];
#else
  static rpcDB_ClientInfo rpcDB_clientInfo;
  return &rpcDB_clientInfo;
#endif
}

void
rpcDB_clientDbhDelete(rpcDB_DbHandleClientInfo *dbhclientinfo)
{
  MUTEX_LOCK(&mp_dbhinfo);

  if (--dbhclientinfo->dbhinfo->refcnt == 0)
    {
      int i;
      for (i = 0; i < MAX_DBHINFO; i++)
	if (rpcDB_dbhInfo[i] == dbhclientinfo->dbhinfo)
	  {
	    free(rpcDB_dbhInfo[i]);
	    rpcDB_dbhInfo[i] = 0;
	    break;
	  }
    }
  free(dbhclientinfo);
  MUTEX_UNLOCK(&mp_dbhinfo);
}

int
rpcDB_clientDbhSet(rpc_ClientId clientid, rpc_Boolean local, int flags, rpcDB_DbHandleInfo *dbhinfo, void *dbh)
{
  int i;
  register rpcDB_ClientInfo *ci = rpcDB_clientInfoGet(clientid);

  for (i = 0; i < RPCDB_MAX_DBH; i++)
    if (!ci->dbhclientinfo[i])
      {
	rpcDB_DbHandleClientInfo *dbhclientinfo = (rpcDB_DbHandleClientInfo *)
	  calloc(sizeof(rpcDB_DbHandleClientInfo), 1);
	dbhclientinfo->dbhinfo = dbhinfo;
	dbhclientinfo->local = local;
	dbhclientinfo->dbh = dbh;
	dbhclientinfo->flags = flags;
	ci->dbhclientinfo[i] = dbhclientinfo;
	return dbhinfo->id;
      }
  return 0;
}

rpcDB_DbHandleClientInfo *
rpcDB_clientDbhGet(rpc_ClientId clientid, int id)
{
  register rpcDB_ClientInfo *ci = rpcDB_clientInfoGet(clientid);
  rpcDB_DbHandleClientInfo *dbhclientinfo;
  int i;

  for (i = 0; i < RPCDB_MAX_DBH; i++)
    if ((dbhclientinfo = ci->dbhclientinfo[i]) &&
	dbhclientinfo->dbhinfo->id == id)
      return dbhclientinfo;
  return 0;
}

void
rpcDB_lock()
{
  MUTEX_LOCK(&mp_dbhinfo);
}

void
rpcDB_unlock()
{
  MUTEX_UNLOCK(&mp_dbhinfo);
}

#if 0
rpcDB_DbHandleInfo *
rpcDB_open_realize(rpc_Server *server, int id)
{
  rpcDB_DbHandleInfo *dbhinfo;

  dbhinfo = rpcDB_dbhinfoGet(id);

  if (!dbhinfo)
    {
      dbhinfo = rpcDB_dbhinfoNew(id);
#ifndef LOCKMTX
      MUTEX_LOCK(&mp_sem);
      dbhinfo->ldbctx.semid[0] =
	sem_find(&dbhinfo->ldbctx.semkey[0], rpc_True);
      dbhinfo->ldbctx.semid[1] =
	sem_find(&dbhinfo->ldbctx.semkey[1], rpc_True);
      MUTEX_UNLOCK(&mp_sem);
#endif
    }
  return dbhinfo;
}
#endif

rpcDB_DbHandleInfo *
rpcDB_open_simple_realize(rpc_Server *server, int id)
{
  rpcDB_DbHandleInfo *dbhinfo;

  dbhinfo = rpcDB_dbhinfoGet(id);

  if (!dbhinfo)
    dbhinfo = rpcDB_dbhinfoNew(id);

  return dbhinfo;
}

static void
rpcDB_sem_rm(int semid)
{
#ifdef TRACE
  printf("removing %d\n", semid);
#endif
#ifndef LOCKMTX
  if (semid && sem_rm(semid) < 0)
    fprintf(stderr, "warning removing semaphore %d\n", semid);
#endif
}

rpc_Boolean
rpcDB_close_do(rpc_Server *server, rpcDB_DbHandleClientInfo **pdbhclientinfo,
	       void *(*close)(rpcDB_DbHandleClientInfo *),
	       void **pstatus)
{
  rpcDB_DbHandleClientInfo *dbhclientinfo = *pdbhclientinfo;
  *pstatus = 0;

  if (!dbhclientinfo)
    return rpc_False;
  else
    {
#ifdef TRACE
      printf("rpcDB_close_do %p\n", dbhclientinfo->dbh);
#endif
      rpcDB_lock();
      if (dbhclientinfo->dbhinfo->refcnt == 1 /* || abortProgram */)
	{
#ifndef LOCKMTX
	  rpcDB_sem_rm(dbhclientinfo->dbhinfo->ldbctx.semid[0]);
	  rpcDB_sem_rm(dbhclientinfo->dbhinfo->ldbctx.semid[1]);
#endif
	}
      
      rpcDB_unlock();
      *pstatus = (*close)(dbhclientinfo);

      rpcDB_clientDbhDelete(dbhclientinfo);

      *pdbhclientinfo = 0;
    }

  return rpc_True;
}

rpc_Boolean
rpcDB_close_realize(rpc_Server *server, rpc_ClientId clientid, int dbid,
		    void *(*close)(rpcDB_DbHandleClientInfo *),
		    void **pstatus)
{
  register rpcDB_ClientInfo *ci = rpcDB_clientInfoGet(clientid);
  rpcDB_DbHandleClientInfo *dbhclientinfo;
  int i;

#ifdef TRACE
  printf("rpcDB_close_realize(())\n");
#endif
  *pstatus = 0;
  for (i = 0; i < RPCDB_MAX_DBH; i++)
    if ((dbhclientinfo = ci->dbhclientinfo[i]) &&
	(dbhclientinfo->dbhinfo->id == dbid || !dbid))
      return rpcDB_close_do(server, &ci->dbhclientinfo[i], close, pstatus);

  return rpc_False;
}
