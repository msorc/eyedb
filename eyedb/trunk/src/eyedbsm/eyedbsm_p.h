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


#ifndef _EYEDBSM_EYEDBSM_P_H
#define _EYEDBSM_EYEDBSM_P_H

#include "eyedbconfig.h"

#include <sys/types.h>

#include <eyedblib/m_mem.h>
#include <eyedbsm/smd.h>
#include <eyedblib/machtypes.h>
#include "eyedbsm/transaction.h"
#include <eyedbsm/eyedbsm.h>
#include "eyedbsm/mutex.h"
#include <eyedblib/log.h>

namespace eyedbsm {

struct ObjectHeader {
  unsigned int unique;
  unsigned int size;
  Oid prot_oid;
};

#define ESM_IND_OID

  static const unsigned int MAGIC = 0xa81726e1U;

  static const int DEFAULT_CREATION_MODE = 0600;

  static const int MAX_ROOT_ENTRIES = 32;

  static const int UNIQUE_BASE = 128;
  static const unsigned int UNIQUE_BOUND = (unsigned int)((1 << Oid_UNIQUE)-1);

  //static const int DBID_L_MASK =  ((1 << Oid_DBID_L)-1);
  static const int MAX_FREE_CELLS = 1000000;

struct DbRootEntry {
  char key[MAX_ROOT_KEY];
  char data[MAX_ROOT_DATA];
};

typedef DbRootEntry DbRootEntries[MAX_ROOT_ENTRIES];

struct LinkmapCell {
  u_int size;
  u_int ns;
  u_int prev, next;
};

struct BitmapHeader {
  unsigned int slot_cur;
  unsigned int slot_lastbusy;
  short retry; /* was Boolean */
};

struct LinkmapHeader {
  int firstcell;
};

struct MapHeader {
  short mtype; /* was MapType */
#ifdef X86
  // 3/12/05 
  // char pad1[4];
#endif
  unsigned int sizeslot;
  unsigned int pow2;
  NS nslots;
  Oid::NX nbobjs;
#ifdef X86
  // 3/12/05 
  char pad3[4];
#endif
  MapStat mstat;
  union {
    BitmapHeader bmh;
    LinkmapHeader lmh;
  } u;
#ifdef X86
  // 3/12/05 
  //char pad2[4];
#endif
};

struct ProtectionDescriptionInternal {
  char name[PROT_NAME];
  unsigned int nprot;
  Protection prot[1];
};

#define protectionDescriptionInternalSize(N) \
  (sizeof(ProtectionDescriptionInternal) + \
   ( ((N) - 1) * sizeof(Protection) ))

#define LOCK_COND

#define MAXCLIENTS_PERDB 128

struct DbLock {
  MutexP mp;
  CondWaitP cond_wait;
  unsigned short S, X;
  int wt_cnt;
  unsigned int xidX;
  unsigned int xidS[MAXCLIENTS_PERDB];
};

enum {
  MAP,
  TRS,
  NX,
  SLT,
  LSL,
  MTX_CNT
};

struct DbMutexes {
  MutexP mp[MTX_CNT];
};

struct DbStat {
  unsigned int total_db_access_cnt;
  unsigned int current_db_access_cnt;
  unsigned int tr_begin_cnt;
  unsigned int tr_commit_cnt;
  unsigned int tr_abort_cnt;
};

/*#define USE_SHM_REFCNT*/

struct DbShmHeader {
  unsigned int magic;
  unsigned int version;
  DbStat stat;
  unsigned int xid;
  eyedblib::int64 hostid;
  char hostname[64];
  char arch[16];
  MutexP main_mp;
#ifdef USE_SHM_REFCNT
  int refcnt;
#else
  int dummy;
#endif
  DbLock lock;
  TransHeader trs_hdr;
  DbMutexes mtx;
};
  
struct DatafileDesc {
  char file[L_FILENAME];
  char name[L_NAME+1];
  unsigned int __maxsize;
#ifdef X86
  // 3/12/05
  char pad[4];
#endif
  MapHeader mp;
#ifdef X86
  char pad1[4];
#endif
  unsigned int __lastslot;
  unsigned short __dspid;
};

struct DataspaceDesc {
  char name[L_NAME+1];
  int __cur;
  int __ndat;
  short __datid[MAX_DAT_PER_DSP];
};

struct DbHeader {
  unsigned int __magic;
  int __dbid;
  char state;
  int __guest_uid;
  Oid __prot_uid_oid;
  Oid __prot_list_oid;
  Oid __prot_lock_oid;
  char shmfile[L_FILENAME];
  Oid::NX __nbobjs;
  unsigned int __ndat;
  DatafileDesc dat[MAX_DATAFILES];
  unsigned int __ndsp;
  DataspaceDesc dsp[MAX_DATASPACES];
  short __def_dspid;
  DbRootEntries vre;
  Oid::NX __lastidxbusy;
  Oid::NX __curidxbusy;
  Oid::NX __lastidxblkalloc;
  Oid::NX __lastnsblkalloc[MAX_DATAFILES];
#ifdef X86
  char pad[4];
#endif
};

 static char* msg_1 = __FILE__;
#define TTT(S) (write( 1, S, strlen(S)))
 static int dummy_off_t_1 = (sizeof(off_t) != 8 ? (TTT(msg_1), *(char *)0 = 0) : 1);

struct MmapDesc {
  Boolean ismapped, locked;
  off_t s_start, s_end, a_start, a_end;
  char *mapaddr;
  unsigned int ref;
  int npts, nalloc;
  char ***pts;
  m_Map *m;
};

struct MmapH {
  MmapDesc *mmd;
  int pos;
  char **pt;
};

  static const int MAX_MMAP_SEGMENTS = 256;

  static const char OPENED_STATE =  1;
  static const char OPENING_STATE = 2;

struct DatDesc {
  int fd;
  const char *file;
  MmapDesc mmd[MAX_MMAP_SEGMENTS];
  m_Map *m_dat;
  char *addr;
};

struct ProtectionHeader {
  int nprot_uid;
  Oid prot_uid_oid;
  int nprot_list;
  Oid prot_list_oid;
};

#define OIDLOCSIZE 6

#ifdef UT_SEM
#define ESM_NSEMS 2
#else
#define ESM_NSEMS 0
#endif

struct DbDescription {
  /* general information */
  int dbid, flags, shmfd;
  OpenHints hints;
  Boolean rsuser, suser;
  int uid, uid_ind;

  /* dbs file */
  m_Map *m_dbs;
  DbHeader *dbs_addr;
  
  /* shmg file */
  m_Map *m_shm;
  DbShmHeader *shm_addr;

  /* omp file */
  m_Map *m_omp;
  void *omp_addr;

  /* dmp files */
  m_Map *m_dmp[MAX_DATAFILES+1];
  char *dmp_addr[MAX_DATAFILES+1];

  /* dat files */
  DatDesc dmd[MAX_DATAFILES+1]; 

  /* registering */
  unsigned reg_mask;
  unsigned int reg_alloc;
  Register *reg;

  /* protection management */
  int nprot_uid;
  DbProtectionDescription *vol_uid;
  int nprot_list;
  Oid *vol_prot_list_oid;
  ProtectionDescriptionInternal **vol_desc_list;

  smdcli_conn_t *conn;

  /* transaction management */
  int tr_cnt;
  TransactionContext trctx[MAXTRS];
  unsigned int xid;
  unsigned int mapwide;
  unsigned int mapwide2;
  XMHandle *trs_mh;
  Mutex mp[MTX_CNT+1];
  CondWaitList cdw_list;
#ifdef UT_SEM
  int locked;
  int semkeys[ESM_NSEMS];
#endif
};

struct DbHandle {
  DbDescription *vd;
  char *dbfile;
  int xid;
  int tr_cnt;
  int flags;
};

#define VD2MTX(VD, W)  ((VD) ? &(VD)->mp[(W)] : 0)
#define MTX(DBH, W)    VD2MTX((DBH)->vd, W)

#define MAP_MTX(DBH) MTX(DBH, MAP)
#define TRS_MTX(DBH) MTX(DBH, TRS)
#define NX_MTX(DBH)  MTX(DBH, NX)
#define SLT_MTX(DBH) MTX(DBH, SLT)
#define LSL_MTX(DBH) MTX(DBH, LSL)

#define DBLOCK_MTX(VD)  VD2MTX(VD, MTX_CNT)
#define DBLOCK_COND(VD) ((VD) ? &VD2MTX(VD, MTX_CNT)->cond : 0)

extern Status
ESM_transactionsRelease(DbDescription *vd, DbShmHeader *shmh,
		       const char *dbfile, int xid,
		       XMHandle *xmh, Boolean *);

extern Status privilegeCheck(void);

extern Boolean backend_interrupt;

extern int lockTimeout;

#define ESM_ASSERT_ABORT(X, MT, XID) \
if (!(X)) \
{ \
  if (MT) MUTEX_UNLOCK(MT, XID); \
  utlog("ASSERT '%s' file \"%s\", line #%d\n", #X, __FILE__, __LINE__); \
  abort(); \
}

#define ESM_ASSERT(X, MT, XID) \
if (!(X)) \
{ \
  return statusMake(INTERNAL_ERROR, "assertion failed `%s' file `%s', line #%d\n", #X, __FILE__, __LINE__); \
}

#define DBH2TRCTX(DBH) (&(DBH)->vd->trctx[(DBH)->vd->tr_cnt-1])

#define NEED_LOCK(TRCTX) \
((TRCTX)->params.lockmode != DatabaseX)

#define NEED_OBJLOCK(TRCTX) \
((TRCTX)->params.lockmode != ReadNWriteN && \
 (TRCTX)->params.lockmode != DatabaseX)

}
#endif
