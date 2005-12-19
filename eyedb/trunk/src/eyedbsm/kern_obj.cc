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

#include <eyedbconfig.h>

#include <kern_p.h>
#include <eyedblib/rpc_lib.h>

//int last_objsize;

#define MSYNC(x,y,z) /* msync(x,y,z) */
#define SLOT_INC 400
#define CHECK_NS_OID(DBH, NS, DATID, OID) \
((DATID) >= 0 && (NS) >= 0 && \
 (NS) <= x2h_u32(DBSADDR(DBH)->dat[DATID].__lastslot) && \
 OIDDBIDGET(OID) == (DBH)->vd->dbid)

namespace eyedbsm {

  const Oid::NX Oid::INVALID_NX = ~0U;
  const size_t Oid::SIZE = 12;
  const Oid Oid::nullOid = Oid::makeNullOid();

  Oid  Oid::makeNullOid()
  {
    Oid oid;
    oid.setNX(0);
    oid.setDbID(0);
    oid.setUnique(0);
    return oid;
  }

  inline static TransactionOP
  makeTOP(LockMode lockmode, TransactionOP op, Status &se)
  {
    se = Success;

    if (lockmode == DefaultLock)
      return op;

    if (lockmode == LockS)
      return (TransactionOP)(op | LOCKS);

    if (lockmode == LockX)
      return (TransactionOP)(op | LOCKX);

    if (lockmode == LockSX)
      return (TransactionOP)(op | LOCKSX);

    if (lockmode == LockN)
      return (TransactionOP)(op | LOCKN);

    se = statusMake(ERROR, "invalid lock mode for reading %d", lockmode);
    return op;
  }

  Boolean
  isPhy(DbHandle const *dbh, const Oid *oid)
  {
    if (getDbVersion((void *)dbh) < PHYOID_VERSION)
      return False;
    return (oid->getUnique() & PHYOID_BITMASK) ? True : False;
  }

  void
  setPhyInfo(Oid *oid, NS ns, short datid)
  {
    // 6/04/02: added NS_OFFSET
    oid->setNX(ns + NS_OFFSET);
    oid->setUnique(oid->getUnique() | (PHYOID_BITMASK | (datid << PHYDATID_SHIFT)));
  }

  void
  getPhyInfo(const Oid *oid, NS *pns, short *datid)
  {
    // 6/04/02: added NS_OFFSET
    *pns = oid->getNX() - NS_OFFSET;
    *datid = ((oid->getUnique() & ~PHYOID_BITMASK) >> PHYDATID_SHIFT);
  }

  /*@@@@ what is the diff between LINUX and SOLARIS for this ??*/
#if defined(LINUX) || defined(LINUX64) || defined(LINUX_IA64) || defined(LINUX_PPC64) || defined(ORIGIN) || defined(ALPHA) || defined(CYGWIN)
#define RAND_COEF ((unsigned int)0x7fffffff)
#else
#define RAND_COEF ((unsigned int)0x7fff)
#endif

  static double ratio_o = (float)(0x3fffff - UNIQUE_BASE)/RAND_COEF;
  static double ratio_n_log = (float)(0xfffff - UNIQUE_BASE)/RAND_COEF;
  static double ratio_n_phy = (float)(0x1fff - UNIQUE_BASE)/RAND_COEF;

  //int rand() {return 1;}

  static unsigned int
  uniqueGet_logical(DbHandle const *dbh)
  {
    double ratio = 
      (getDbVersion((void *)dbh) >= PHYOID_VERSION) ? ratio_n_log : ratio_o;
    return (unsigned int)(((double)rand()*ratio) + UNIQUE_BASE);
  }	  

  static unsigned int
  uniqueGet_physical(DbHandle const *dbh)
  {
    return (unsigned int)(((double)rand()*ratio_n_phy) + UNIQUE_BASE);
  }	  

  static void
  oidMake_logical(DbHandle const *dbh, Oid::NX nx, Oid *oid)
  {
    oid->setNX(nx);
    OIDDBIDMAKE(oid, dbh->vd->dbid);
    oid->setUnique(uniqueGet_logical(dbh));
  }

  static void
  oidMake_physical(DbHandle const *dbh, NS ns, short datid, Oid *oid)
  {
    OIDDBIDMAKE(oid, dbh->vd->dbid);
    oid->setUnique(uniqueGet_physical(dbh));
    setPhyInfo(oid, ns, datid);
  }

  /*
    static Status
    checkPhyLogOid(DbHandle const *dbh, short datid, unsigned int create_flags)
    {
    DatafileDesc *dat = &DBSADDR(dbh)->dat[datid];
    DatType dtype = getDatType(DBSADDR(dbh), datid);

    if ((create_flags & LogicalOid) && dtype != LogicalOidType)
    return statusMake(ERROR, "cannot create a logical oid on a physical "
    "oid type based datafile '%s'", (*dat->name ?
    std::string(dat->name) :
    std::string("#") +
    std::string(datid)));

    if ((create_flags & PhysicalOid) && dtype != PhysicalOidType)
    return statusMake(ERROR, "cannot create a physical oid on a logical "
    "oid type based datafile '%s'", (*dat->name ?
    std::string(dat->name) :
    std::string("#") +
    std::string(datid)));

    return Success;
    }
  */

  static Status
  ESM_objectCreate_server(DbHandle const *dbh, void const *const object,
			  unsigned int size, short datid, short dspid, Oid *oid,
			  rpc_ServerData *data, OPMode opmode)
  {
    Status se;
    Boolean opsync = False;

    if (opmode != OPGrowingPhase)
      return statusMake(ERROR, "internal error: unexpected opmode %d",
			opmode);
#undef PR
#define PR "objectCreate: "
    if (!check_dbh(dbh))
      return statusMake(INVALID_DB_HANDLE, PR IDBH);

    if (!(dbh->vd->flags & VOLRW))
      return statusMake(WRITE_FORBIDDEN, PR WF, dbh->dbfile);

    if (size < 0)
      return statusMake(INVALID_OBJECT_SIZE, PR "object size is negative: `%d'", size);

    int tridx = dbh->vd->tr_cnt-1;
    TransactionContext *trctx = &dbh->vd->trctx[tridx];

    if (trctx->params.recovmode == RecoveryFull ||
	trctx->params.recovmode == RecoveryPartial)
      {
	//printf("should perform incomplete creation\n");
	// 1. create an oid (nx from lastidxbusy cache) and unique
	// 2. objectLock(..., &tro);
	// 3. addr = ESM_trobjDataGet(dbh, tro, size);
	// 4. memcpy(addr, &objh, sizeof(ObjectHeader));
	// 5. if (data) ... (partie actuelle finale du code de
	//    ESM_objectCreate_server)
	/*return statusMake(ERROR,
	  "recovery on mode is not yet implemented");
	*/
      }

    //OidLoc oidloc = {0};
    OidLoc oidloc;
    memset(&oidloc, 0, sizeof(oidloc));

    int rsize = size + sizeof(ObjectHeader);
    MmapH hdl;
    const MapHeader *mp;

    for (;;) {
      mp = DAT2MP(dbh, datid);
      oidloc.datid = datid;

      se = mapAlloc(dbh, datid, rsize, &oidloc.ns);
      if (se) return se;
      if (oidloc.ns >= 0)
	break;

      if (dspid == DefaultDspid || !ESM_getNextDatafile(dbh, dspid, datid))
	return statusMake(NO_DATAFILESPACE_LEFT,
			  PR "database '%s' %s ",
			  dbh->dbfile,
			  (dspid != DefaultDspid ?
			   std::string("dataspace ") +
			   DBSADDR(dbh)->dsp[dspid].name :
			   std::string("unspecified dataspace")).c_str());
    }

    /*
      if (se = checkPhyLogOid(dbh, oidloc.datid, create_flags))
      return se;
    */

    ObjectHeader objh;
    char *addr;
#ifdef SEXDR
    unsigned int sizeslot = x2h_u32(mp->sizeslot);
#else
    unsigned int sizeslot = mp->sizeslot;
#endif
    int ls = oidloc.ns + SZ2NS_XDR(rsize, mp);
    Mutex *mt = LSL_MTX(dbh);
    unsigned int xid = dbh->vd->xid;
    DatType dtype = getDatType(DBSADDR(dbh), datid);
    if (dtype == LogicalOidType) {
      Oid::NX nx;
      Status se = nxAlloc(dbh, oidloc, &nx);
      if (se) return se;

      if (nx == Oid::INVALID_NX)
	return statusMake(TOO_MANY_OBJECTS, PR "database '%s'", dbh->dbfile);

      oidMake_logical(dbh, nx, oid);
    }
    else if (getDbVersion((void *)dbh) < PHYOID_VERSION)
      return statusMake(INVALID_DB_HANDLE, PR "cannot use physical oid "
			"on versions previous to %d", PHYOID_VERSION);
    else
      oidMake_physical(dbh, oidloc.ns, oidloc.datid, oid);

    if (se = ESM_objectLock(dbh, oid, OCREATE, 0, 0))
      return se;

#ifdef SEXDR

    objh.unique = h2x_u32(oid->getUnique());
    objh.size = h2x_u32(makeInvalid(rsize));

#else

    objh.unique = oid->getUnique();
    objh.size = makeInvalid(rsize);
#endif

    memset(&objh.prot_oid, 0, sizeof(Oid));

    if (NEED_LOCK(trctx))
      MUTEX_LOCK(mt, xid);

    // was: if (ls >= dbh->vd->dbhead->lastslot)
    if (ls >= x2h_u32(DBSADDR(dbh)->dat[datid].__lastslot))
      {
	static char zero = 0;
	int ls1 = ls + SLOT_INC;
	int fd = dbh->vd->dmd[datid].fd;

	if (se = syscheck(PR, lseek(fd, ((long long)ls1 * sizeslot) - 1,
				    0), ""))
	  {
	    if (NEED_LOCK(trctx))
	      MUTEX_UNLOCK(mt, xid);
	    return se;
	  }

	if (se = syscheckn(PR, write(fd, &zero, sizeof(char)), sizeof(char), ""))
	  {
	    if (NEED_LOCK(trctx))
	      MUTEX_UNLOCK(mt, xid);
	    return se;
	  }

	DBSADDR(dbh)->dat[datid].__lastslot = h2x_u32(ls1);
      }

    if (NEED_LOCK(trctx))
      MUTEX_UNLOCK(mt, xid);

    addr = oidloc2addr_(oidloc, dbh, rsize, &addr, &hdl);

    memcpy(addr, &objh, sizeof(ObjectHeader));

    if (data) {
      if (data->data != ObjectNone) {
	if (data->data)
	  rpc_socketRead(data->fd, addr + sizeof(ObjectHeader),
			 size);
	else
	  memset(addr + sizeof(ObjectHeader), 0, size);
      }
    }
    else if (object != ObjectNone) {
      if (object) /* != ObjectZero */
	memcpy(addr + sizeof(ObjectHeader), object, size);
      else
	memset(addr + sizeof(ObjectHeader), 0, size);
    }

    //MSYNC((caddr_t)ROUND_PAGE(addr), ROUND_UP_PAGE(rsize), MS_ASYNC);

    hdl_release(&hdl);
    ESM_REGISTER(dbh, CreateOP, ESM_addToRegisterCreate(dbh, oid, size));

    return Success;
  }

  Status
  ESM_objectCreate(DbHandle const *dbh, void const *const object,
		   unsigned int size, short dspid, Oid *oid, OPMode opmode)
  {
    short datid;
    Status s = ESM_getDatafile(dbh, dspid, datid);
    if (s) return s;

    return ESM_objectCreate_server(dbh, object, size, datid, dspid, oid, 0,
				   opmode);
  }

  Status
  ESM_objectDelete(DbHandle const *dbh, Oid const *const oid,
		   OPMode opmode)
  {
#undef PR
#define PR "objectDelete: "
    Status se;
    Boolean opsync = False;

#ifndef SHR_SECURE
    if (opmode != OPShrinkingPhase)
#endif
      {
	if (!check_dbh(dbh))
	  return statusMake(INVALID_DB_HANDLE, PR IDBH);
	if (!(dbh->vd->flags & VOLRW))
	  return statusMake(WRITE_FORBIDDEN, PR WF, dbh->dbfile);
	if (!check_oid(dbh, oid))
	  return statusMake(INVALID_OID, PR "invalid oid '%s'", getOidString(oid));

	if (se = ESM_objectLock(dbh, oid, ODELETE, &opsync, 0))
	  return se;
      }

    if (opsync || opmode != OPGrowingPhase)
      {
	MmapH hdl;
	ObjectHeader *objh;
	Boolean oid2addr_failed;
	if (!(objh = oid2objh(oid, dbh, &objh, &hdl, &oid2addr_failed)))
	  {
	    if (oid2addr_failed)
	      return statusMake(FATAL_ERROR, PR "failed to map segment for oid '%s'", getOidString(oid));
	    return statusMake(INVALID_OID, PR "invalid oid '%s'", getOidString(oid));
	  }

	OidLoc oidloc = oidLocGet(dbh, oid);
#ifdef SEXDR
	mapFree(dbh->vd, oidloc.ns, oidloc.datid, x2h_getSize(objh->size));
#else
	mapFree(dbh->vd, oidloc.ns, oidloc.datid, getSize(objh->size));
#endif

	if (!isPhy(dbh, oid))
	  nxFree(dbh, oid->getNX());

	memset(objh, 0, sizeof(ObjectHeader));
      
	/*
	  if (trace_it0)
	  printf("ESM_objectDelete(oid=%s, size=%d)\n", getOidString(oid),
	  getSize(objh->size));
	*/
	hdl_release(&hdl);
	ESM_REGISTER(dbh, DeleteOP, ESM_addToRegisterDelete(dbh, oid));
      }

    return Success;
  }

  Status
  ESM_objectDeleteByOidLoc(DbHandle const *dbh, Oid const *const oid,
			   NS ns, short datid)
  {
#undef PR
#define PR "objectDelete: "
    Status se;
    MmapH hdl;
    ObjectHeader *objh;
    Boolean dummy;
    OidLoc oidloc_o = oidLocGet_(dbh, oid->getNX());
    nxSet(dbh, oid->getNX(), ns, datid);

    objh = oid2objh(oid, dbh, &objh, &hdl, &dummy);

    OidLoc oidloc = oidLocGet(dbh, oid);
#ifdef SEXDR
    mapFree(dbh->vd, oidloc.ns, oidloc.datid, x2h_getSize(objh->size));
#else
    mapFree(dbh->vd, oidloc.ns, oidloc.datid, getSize(objh->size));
#endif

    nxSet(dbh, oid->getNX(), oidloc_o.ns, oidloc_o.datid);
    memset(objh, 0, sizeof(ObjectHeader));
      
    hdl_release(&hdl);
    return Success;
  }

  Status
  ESM_objectRestore(DbHandle const *dbh, Oid const *const oid,
		    NS ns, short datid)
  {
#undef PR
#define PR "objectRestore: "

    nxSet(dbh, oid->getNX(), ns, datid);

    return Success;
  }

  Status
  ESM_objectSizeGet(DbHandle const *dbh, unsigned int *size,
		    LockMode lockmode, Oid const *const oid,
		    OPMode opmode)
  {
#undef PR
#define PR "objectSizeGet: "
    if (!check_dbh(dbh))
      return statusMake(INVALID_DB_HANDLE, PR IDBH);
    else
      {
	if (!check_oid(dbh,oid))
	  return statusMake(INVALID_OID, PR "invalid oid '%s'", getOidString(oid));
	else
	  {
	    MmapH hdl;
	    ObjectHeader *objh;
	    Status se;
	    Boolean oid2addr_failed;

	    TransactionOP lop = makeTOP(lockmode, OREAD, se);
	    if (se) return se;

	    if (se = ESM_objectLock(dbh, oid, lop, 0, 0))
	      return se;

	    if (!(objh = oid2objh(oid, dbh, &objh, &hdl, &oid2addr_failed)))
	      {
		if (oid2addr_failed)
		  return statusMake(FATAL_ERROR, PR "failed to map segment for oid '%s'", getOidString(oid));
		return statusMake(INVALID_OID, PR "invalid oid '%s'", getOidString(oid));
	      }

#ifdef SEXDR
	    *size = x2h_getSize(objh->size) - sizeof(ObjectHeader);
#else
	    *size = getSize(objh->size) - sizeof(ObjectHeader);
#endif
	  
	    ESM_REGISTER(dbh, SizeGetOP, ESM_addToRegisterSizeGet(dbh, oid));
	    hdl_release(&hdl);
	    return Success;
	  }
      }
  }

  Status
  ESM_objectValidate(DbHandle const *dbh, Oid const *const oid)
  {
#undef PR
#define PR "objectValidate: "
    MmapH hdl;
    ObjectHeader *objh;
    Boolean oid2addr_failed;

    if (!(objh = oid2objh(oid, dbh, &objh, &hdl, &oid2addr_failed)))
      {
	if (oid2addr_failed)
	  return statusMake(FATAL_ERROR, PR "failed to map segment for oid '%s'", getOidString(oid));
	return statusMake(INVALID_OID, PR "invalid oid '%s'", getOidString(oid));
      }

#ifdef SEXDR
    objh->size = h2x_u32(x2h_makeValid(objh->size));
#else
    objh->size = makeValid(objh->size);
#endif

    hdl_release(&hdl);
    return Success;
  }

  static Status
  ESM_objectBornAgain(DbHandle const *dbh,
		      ObjectHeader *objh,
		      Oid const *const o_oid,
		      Oid const *const n_oid,
		      int keepDatid,
		      const OidLoc &xoidloc, Boolean opsync) // added 18/07/01
  {
    MmapH hdl0;
    Boolean dummy;

    if (isPhy(dbh, o_oid))
      return statusMake(INVALID_OID, PR "cannot move a physical oid");

    if (isPhy(dbh, n_oid))
      return statusMake(INVALID_OID, PR "cannot move an oid to a physical oid type based datafile");

    /* 1/ copy the old oid into the new oid */
    objh = oid2objh(n_oid, dbh, &objh, &hdl0, &dummy);
#if 0
    OidLoc oidloc1 = oidLocGet_(dbh, n_oid->nx);
    OidLoc oidloc2 = oidLocGet_(dbh, o_oid->nx);
    printf("setting unique %s %s %d => %d [%d %d]\n", getOidString(n_oid),
	   getOidString(o_oid), objh->unique, o_oid->unique,
	   oidloc1.ns, oidloc2.ns);
#endif

#ifdef SEXDR
    objh->unique = h2x_u32(o_oid->getUnique());
#else
    objh->unique = o_oid->getUnique();
#endif

    hdl_release(&hdl0);
  
    /* 2/ born again the nx of the old object */
    OidLoc oidloc = oidLocGet_(dbh, n_oid->getNX());
    nxSet(dbh, o_oid->getNX(), oidloc.ns,
	  (keepDatid >= 0 ? keepDatid : oidloc.datid));

    /* 3/ free the nx of the new object */
    nxFree(dbh, n_oid->getNX());

    if (!opsync)
      ESM_bornAgainEpilogue(dbh, o_oid, n_oid, xoidloc.ns, xoidloc.datid);

    return Success;
  }

  Status
  ESM_objectSizeModify(DbHandle const *dbh, unsigned int size, Boolean copy,
		       Oid const *const oid, OPMode opmode)
  {
#undef PR
#define PR "objectSizeModify: "
    Status se = Success;
    MmapH hdl0;
    ObjectHeader *objh;
    int osize;
    PObject *po = 0;
    Boolean opsync = False;
    char *buf = 0;
    Boolean oid2addr_failed;

    if (isPhy(dbh, oid))
      return statusMake(INVALID_OID, PR "cannot change the size of a "
			"physical oid");
#ifndef SHR_SECURE
    if (opmode != OPShrinkingPhase)
#endif
      {
	if (!check_dbh(dbh))
	  return statusMake(INVALID_DB_HANDLE, PR IDBH);

	if (!check_oid(dbh,oid))
	  return statusMake(INVALID_OID, PR "invalid oid '%s'", getOidString(oid));
	if (se = ESM_objectLock(dbh, oid, OCHSIZE, &opsync, 0))
	  return se;
      }

    if (!(objh = oid2objh(oid, dbh, &objh, &hdl0, &oid2addr_failed)))
      {
	if (oid2addr_failed)
	  return statusMake(FATAL_ERROR, PR "failed to map segment for oid '%s'", getOidString(oid));
	return statusMake(INVALID_OID, PR "invalid oid '%s'", getOidString(oid));
      }

#ifdef SEXDR
    osize = x2h_getSize(objh->size) - sizeof(ObjectHeader);
#else
    osize = getSize(objh->size) - sizeof(ObjectHeader);
#endif
  
    hdl_release(&hdl0);
  
    if (osize != size)
      {
	Oid noid;
	OidLoc oidloc;
	int cpsize;

	oidloc = oidLocGet_(dbh, oid->getNX());

	/* 1/ create new object of size 'size' */
	/* changed the 23/08/01 */
	/*
	  if (se = ESM_objectCreate_server(dbh, 0, size, oidloc.datid, DefaultDspid, &noid, 0, OPDefault))
	  goto error;
	*/
	if (se = ESM_objectCreate(dbh, 0, size, getDataspace(DBSADDR(dbh), oidloc.datid), &noid, OPDefault))
	  goto error;

	if (copy)
	  {
	    cpsize = MIN(size, osize);
	    buf = (char *)malloc(cpsize);

	    /* 2/ copy old object into new object (at most size) */
	    if (se = ESM_objectRead(dbh, 0, cpsize, buf, LockS, 0, 0, oid,
				    OPDefault))
	      goto error;

	    if (se = ESM_objectWrite(dbh, 0, cpsize, buf, &noid, opmode))
	      goto error;
	  }

	/* 3/ remove old object, but keep its oid */
	if (se = ESM_objectDelete(dbh, oid, opmode))
	  goto error;

	/* 4/ oid management */
	if (se = ESM_objectBornAgain(dbh, objh, oid, &noid, -1, oidloc, opsync))
	  goto error;

	ESM_REGISTER(dbh, SizeModOP, ESM_addToRegisterSizeMod(dbh, oid, size));
      }


  error:
    free(buf);
    return se;
  }

  Status
  ESM_objectCheckAccess(DbHandle const *dbh, Boolean write,
			Oid const *const oid, Boolean *access)
  {
    ObjectHeader objh;
    MmapH hdl;
    ObjectHeader *pobjh;
    const Protection *prot;
    Boolean oid2addr_failed;

    /* really? */
    if (OIDDBIDGET(oid) != dbh->vd->dbid)
      {
	*access = True;
	return Success;
      }

    if (!(pobjh = oid2objh(oid, dbh, &pobjh, &hdl, &oid2addr_failed)))
      {
	if (oid2addr_failed)
	  return statusMake(FATAL_ERROR, PR "failed to map segment for oid '%s'", getOidString(oid));

	return statusMake(INVALID_OID, "invalid oid '%s'", getOidString(oid));
      }


    /* should perharps look in tro->prot_oid is tro exists for this
       object and if tro->prot_oid_set */
    prot = protGet(dbh, ESM_getProtection(dbh, oid, &pobjh->prot_oid),
		   getUid(dbh));

    hdl_release(&hdl);

    if (prot->r != ReadAll || (write && prot->w != WriteAll))
      {
	*access = False;
	return Success;
      }

    *access = True;
    return Success;
  }
      
  Status
  ESM_objectReadCache(DbHandle const *dbh, int start, void **object,
		      LockMode lockmode, Oid const *const oid)
  {
    static const char *pre = "objectReadCache";
    if (!check_dbh(dbh))
      return statusMake(INVALID_DB_HANDLE, "%s%s", pre, IDBH);

    Status se;
    TransactionOP lop;

    lop = makeTOP(lockmode, OREAD, se);
    if (se) return se;

    Boolean mustExist = True;
    TRObject *tro = 0;
    if (se = ESM_objectLockCheck(dbh, oid, lop, 0, &mustExist, &tro))
      return se;

    if (!mustExist) {
      *(char **)object = 0;
      return Success; 
    }

    char *addr = ESM_trobjDataGetIfExist(dbh, tro);
    if (!addr) {
      *(char **)object = 0;
      return Success; 
    }

    *(char **)object = addr + start;
    return Success;
  }

  Status
  ESM_objectWriteCache(DbHandle const *dbh, int __start,
		       void const * const __object, Oid const *const oid)
  {
    static const char *pre = "objectWriteCache";
    if (!check_dbh(dbh))
      return statusMake(INVALID_DB_HANDLE, "%s%s", pre, IDBH);

    Status se;
    TransactionOP lop;
    TRObject *tro;

    lop = makeTOP(LockS, OWRITE, se);
    if (se) return se;

    return ESM_objectLock(dbh, oid, lop, 0, &tro);
  }

  static Status
  read_write(const char *pre, DbHandle const *dbh, int start,
	     int length, void *object, Oid const *const oid,
	     short *pdatid, unsigned int *psize, TransactionOP op,
	     LockMode lockmode, rpc_ServerData *data, OPMode opmode,
	     Boolean nocopy = False)
  {
    TransactionOP lop;
    ObjectHeader objh;
    Status status;
    MmapH hdl0, hdl1;
    ObjectHeader *pobjh;
    unsigned int size;
    Status se;
    Boolean opsync = False;
    TRObject *tro = 0;
    int immediate;
    int up, must_release = 1;
    Boolean oid2addr_failed;

    //printf("read_write(%s, uid = %d)\n", getOidString(oid), dbh->vd->uid);
    if (nocopy) {
      if (dbh->vd->hints.maph != WholeMap)
	return statusMake(ERROR, "no-copy read may be only used in "
			  "whole-map opened databases");
      //printf("reading %s with no copy\n", getOidString(oid));
    }

    OidLoc oidloc = oidLocGet(dbh, oid);
    if (pdatid) *pdatid = oidloc.datid;

#ifndef SHR_SECURE
    if (opmode != OPShrinkingPhase)
#endif
      {
	if (!check_dbh(dbh))
	  return statusMake(INVALID_DB_HANDLE, "%s%s", pre, IDBH);
	if ((op == OWRITE || op == PWRITE )&& !(dbh->vd->flags & VOLRW))
	  return statusMake(WRITE_FORBIDDEN, "%s%s", pre, WF_P);
	if (start < 0)
	  return statusMake(INVALID_OFFSET, "%soffset is negative: `%d'", pre, start);
	if (length < 0)
	  return statusMake(INVALID_SIZE, "%ssize is negative: `%d'", pre, length);
	if (!CHECK_NS_OID(dbh, oidloc.ns, oidloc.datid, oid))
	  {
	    /*
	      printf("INVALID OID oidloc = 0x%x %d [nx=%d]\n",
	      oidloc.ns, oidloc.datid, oid->nx);
	      if (oidloc.datid >= 0)
	      printf("... lastslot %d %d %d\n",
	      x2h_u32(DBSADDR(dbh)->dat[oidloc.datid].__lastslot),
	      OIDDBIDGET(oid), (dbh)->vd->dbid);
	    */
#ifdef ESM_POST_RECOVERY
	    /* all this is !!NULL!! */
	    if (post_recovery)
	      {
		if (length)
		  memset(object, 0, length);

		if (post_recovery != 2)
		  {
		    utlog("EyeDB Recovery System: %sinvalid oid '%s'", pre, getOidString(oid));
		    fprintf(stderr, "EyeDB Recovery System: %sinvalid oid '%s'", pre, getOidString(oid));
		  }
		return Success;
	      }
#endif
#if 1
	    Oid xoid;
	    x2h_oid(&xoid, oid);
	    printf("INVALID: %s\n", getOidString(&xoid));
#endif
	    return statusMake(INVALID_OID, "%sinvalid oid '%s'", pre, getOidString(oid));
	  }

	lop = makeTOP(lockmode, op, se);
	if (se) return se;

	if (se = ESM_objectLock(dbh, oid, lop, &opsync, &tro))
	  return se;
      }

    if (!(pobjh = oid2objh_(oid, oidloc.ns, oidloc.datid, dbh, &pobjh, &hdl0, &up, &oid2addr_failed)))
      {
	if (oid2addr_failed)
	  return statusMake(FATAL_ERROR, "%sfailed to map segment for oid '%s'", pre, getOidString(oid));
	return statusMake(INVALID_OID, "%sinvalid oid '%s'", pre, getOidString(oid));
      }

    if (!length && start) {
      hdl_release(&hdl0);
      return statusMake(INVALID_SIZE, "%soffset must be null when length "
			"is not specified", pre);
    }

#ifdef SEXDR
    size = x2h_getSize(pobjh->size) - sizeof(ObjectHeader);
#else
    size = getSize(pobjh->size) - sizeof(ObjectHeader);
#endif

    /*
    // 4/07/01: this should be in the argument of the function instead
    // of an horrible global variable!
    last_objsize = size;
    */
    // 17/05/02: replaced by a parameter
    if (psize) *psize = size;

    if (length != 0 && (start + length > size))
      {
	hdl_release(&hdl0);
	return statusMake(INVALID_SIZE, "%sobject size exceeded: `%d', object size is `%d' [%s]", pre, start+length, size, getOidString(oid));
      }
    else
      {
	char *addr, *dbaddr = 0, *traddr = 0;
	const MapHeader *mp = DAT2MP(dbh, oidloc.datid);
#ifdef SEXDR
	unsigned int pow2 = x2h_u32(mp->pow2);
	unsigned int sizeslot = x2h_u32(mp->sizeslot);
#else
	unsigned int pow2 = mp->pow2;
	unsigned int sizeslot = mp->sizeslot;
#endif
	const Protection *prot;
      
	length = ((length == 0) ? size : length);
      
	immediate = (opmode != OPGrowingPhase || opsync);

	if (immediate || op == OREAD)
	  {
	    /*
	      faire un test pour voir si:
	      - ce range a deja ete mapp�
	    */
	    int l = length + sizeof(ObjectHeader) + start;
	    int l1 = l >> pow2;

	    if (!up || (oidloc.ns+l1+1) < up)
	      {
		dbaddr = ((char *)pobjh) + sizeof(ObjectHeader);
		must_release = 0;
		/*printf("already in range [ns = %d, l/sizeslot %d, up %d]\n",
		  ns, l/sizeslot, up);*/
	      }
	    else
	      {
		dbaddr = oid2addr_(oidloc.ns, oidloc.datid, dbh, l, &dbaddr, &hdl1, 0) +
		  sizeof(ObjectHeader);
		/*printf("not in range [ns = %d, l/sizeslot %d, up %d]\n",
		  ns, l/sizeslot, up); */
	      }
	  }

	if (!immediate)
	  {
	    traddr = ESM_trobjDataGet(dbh, tro, size);
	    if (!traddr) {
	      hdl_release(&hdl0);
	      if (must_release && (immediate || op == OREAD))
		hdl_release(&hdl1);
	      return statusMake(NO_SHMSPACE_LEFT, "for object size %d",
				size);
	    }
	  }
      
	addr = (immediate ? dbaddr : traddr);

	/* protection check */
	prot = protGet(dbh, (tro && tro->prot_oid_set ? &tro->prot_oid :
			     &pobjh->prot_oid), getUid(dbh));
      
	switch(op)
	  {
	  case OWRITE:
	    if (prot->w == WriteAll || (start + length) <= (int)prot->w) {
	      if (data)
		rpc_socketRead(data->fd, addr, length);
	      else
		ESM_trobjDataWrite(addr, (const char *)object, start, length, opmode, opsync);
	      
	      //MSYNC((caddr_t)ROUND_PAGE(addr), ROUND_UP_PAGE(length), MS_ASYNC);	
	      status = Success;
	    }
	    else
	      status = statusMake(OBJECT_PROTECTED, "%sobject '%s' is protected", pre, getOidString(oid));
	    ESM_REGISTER(dbh, WriteOP, ESM_addToRegisterWrite(dbh, oid, start, length));
	    break;
	  
	  case OREAD:
	    if (prot->r == ReadAll || (start + length) <= (int)prot->r) {
	      if (data) {
		if (length <= data->buff_size) {
		  memcpy(data->data, addr, length);
		  data->status = rpc_BuffUsed;
		}
		else {
		  data->data = addr;
		  data->status = rpc_PermDataUsed;
		}
	      
		data->size = length;
		status = Success;
	      }
	      else
		status = ESM_trobjDataRead((char *)object, addr, dbaddr, start,
					   length, opsync, nocopy);

	      ESM_REGISTER(dbh, ReadOP, ESM_addToRegisterRead(dbh, oid, start, length));
	    }
	    else {
#ifdef TRACE
	      printf("protected '%d %d'\n", pobjh->prot_oid.ns,
		     pobjh->prot_oid.unique);
#endif
	      status = statusMake(OBJECT_PROTECTED, "%sobject is protected: '%s'", pre, getOidString(oid));
	    }
	    break;
	  
	  case PWRITE:
	    if (immediate)
	      memcpy(&pobjh->prot_oid, object, sizeof(Oid));
	    else
	      ESM_transactionObjectProtSet(tro, (Oid *)object);
	    status = Success;
	    break;
	  
	  case PREAD:
	    memcpy(object, &pobjh->prot_oid, sizeof(Oid));
	    status = Success;
	    break;

	  default:
	    status = statusMake(ERROR, "internal error: "
				"unexpected object operation: %x", op);
	    break;
	  }
      
	hdl_release(&hdl0);

	if (must_release && (immediate || op == OREAD))
	  hdl_release(&hdl1);
      
	return status;
      }
  }

  /*
    Status
    ESM_objectWrite_server(DbHandle const *dbh, int start, int length,
    void const *const object, Oid const *const oid,
    rpc_ServerData *data, OPMode opmode)
    {
    return read_write("objectWrite: ", dbh, start, length,
    (void *)object, oid, 0, 0, OWRITE, LockS, data,
    opmode);
    }

    Status
    ESM_objectRead_server(DbHandle const *dbh, int start, int length,
    void *object, Oid const *const oid,
    rpc_ServerData *data, OPMode opmode)
    {
    assert(0);
    return read_write("objectRead: ", dbh, start, length, object,
    oid, 0, OREAD, DefaultLock, data, opmode);
    }
  */

  Status
  ESM_objectWrite(DbHandle const *dbh, int start, int length,
		  void const *const object, Oid const *const oid,
		  OPMode opmode)
  {
    return read_write("objectWrite: ", dbh, start, length,
		      (void *)object, oid, 0, 0, OWRITE, LockS, 0,
		      opmode);
  }

  Status
  ESM_objectRead(DbHandle const *dbh, int start, int length, void *object,
		 LockMode lockmode, short *pdatid, unsigned int *psize, 
		 Oid const *const oid, OPMode opmode)
  {
    return read_write("objectRead: ", dbh, start, length, object,
		      oid, pdatid, psize, OREAD, lockmode, 0, opmode);
  }

  Status
  ESM_objectReadNoCopy(DbHandle const *dbh, int start, int length,
		       void *object, LockMode lockmode, short *pdatid,
		       unsigned int *psize, Oid const *const oid, OPMode opmode)
  {
    return read_write("objectRead: ", dbh, start, length, object,
		      oid, pdatid, psize, OREAD, lockmode, 0, opmode,
		      True);
  }

  Status
  ESM_objectsMoveDatDsp(DbHandle const *dbh, Oid const *const oid,
			unsigned int oid_cnt, short datid, short dspid,
			Boolean keepDatid, OPMode opmode)
  {
    Status s;
    for (int i = 0; i < oid_cnt; i++)
      if (s = ESM_objectMoveDatDsp(dbh, &oid[i], datid, dspid, keepDatid, opmode))
	return s;

    return Success;
  }

  Boolean
  isDatInDsp(DbHandle const *dbh, short dspid, short datid)
  {
    DataspaceDesc *dsp = &DBSADDR(dbh)->dsp[dspid];
    unsigned int ndat = x2h_u32(dsp->__ndat);
    for (int i = 0; i < ndat; i++)
      if (x2h_16(dsp->__datid[i]) == datid)
	return True;
    return False;
  }

  Status
  ESM_objectMoveDatDsp(DbHandle const *dbh, Oid const *const oid,
		       short datid, short dspid, Boolean keepDatid,
		       OPMode opmode)
  {
#undef PR
#define PR "objectMoveDatDsp: "
    Status se = Success;
    MmapH hdl0;
    ObjectHeader *objh;
    unsigned int size;
    PObject *po = 0;
    Boolean opsync = False;
    Boolean oid2addr_failed;

    /*
      if (isPhy(dbh, oid))
      return statusMake(INVALID_OID, PR "cannot move a physical oid");
    */

#ifndef SHR_SECURE
    if (opmode != OPShrinkingPhase)
#endif
      {
	if (!check_dbh(dbh))
	  return statusMake(INVALID_DB_HANDLE, PR IDBH);

	if (datid >= 0) {
	  if (!isDatValid(dbh, datid))
	    return statusMake(INVALID_DATAFILE, PR "invalid datafile '%d'",
			      datid);
	}
	else {
	  if (!isDspValid(dbh, dspid))
	    return statusMake(INVALID_DATASPACE,
			      PR "invalid dataspace '%d'", dspid);
	}

	if (!check_oid(dbh, oid))
	  return statusMake(INVALID_OID, PR "invalid oid '%s'", getOidString(oid));

	if (datid >= 0 &&
	    getDatType(DBSADDR(dbh), datid) == PhysicalOidType)
	  return statusMake(INVALID_OID, PR "cannot move an oid to a "
			    "physical oid type based datafile");

	if (se = ESM_objectLock(dbh, oid, OCHSIZE, &opsync, 0))
	  return se;
      }

    if (!(objh = oid2objh(oid, dbh, &objh, &hdl0, &oid2addr_failed)))
      {
	if (oid2addr_failed)
	  return statusMake(FATAL_ERROR, PR "failed to map segment for oid '%s'", getOidString(oid));
	return statusMake(INVALID_OID, PR "invalid oid '%s'", getOidString(oid));
      }

#ifdef SEXDR
    size = x2h_getSize(objh->size) - sizeof(ObjectHeader);
#else
    size = getSize(objh->size) - sizeof(ObjectHeader);
#endif
  
    hdl_release(&hdl0);
  
    Oid noid;
    OidLoc oidloc;
  
    oidloc = oidLocGet_(dbh, oid->getNX());
  
    if (datid >= 0) {
      if (oidloc.datid == datid)
	return Success;
    }
    else if (isDatInDsp(dbh, dspid, oidloc.datid))
      return Success;

    if (isPhy(dbh, oid))
      return statusMake(INVALID_OID, PR "cannot move a physical oid");

    char *buf = 0;
    /* 1/ create new object of size 'size' */
    if (datid >= 0) {
      if (se = ESM_objectCreate_server(dbh, 0, size, datid, DefaultDspid, &noid, 0, opmode))
	goto error;
    }
    else {
      if (se = ESM_objectCreate(dbh, 0, size, dspid, &noid, opmode))
	goto error;
    }
      
    if (isPhy(dbh, &noid))
      return statusMake(INVALID_OID, PR "cannot move an oid to a "
			"physical oid type based datafile");

    buf = (char *)malloc(size);

    /* 2/ copy old object into new object (at most size) */
    if (se = ESM_objectRead(dbh, 0, size, buf, LockS, 0, 0, oid, opmode))
      goto error;

    if (se = ESM_objectWrite(dbh, 0, size, buf, &noid, opmode))
      goto error;

    /* 3/ remove old object, but keep its oid */
    if (se = ESM_objectDelete(dbh, oid, opmode))
      goto error;
      
    /* 4/ oid management */
    if (se = ESM_objectBornAgain(dbh, objh, oid, &noid,
				 (keepDatid ? oidloc.datid : -1),
				 oidloc, opsync))
      goto error;

  error:
    free(buf);
    return se;
  }

  Status
  ESM_objectProtectionSet(DbHandle const *dbh, Oid const *const oid,
			  Oid const *const protoid, OPMode opmode)
  {
    if (protGet(dbh, protoid, getUid(dbh)) == &p_none)
      return statusMake_s(PROTECTION_NOT_FOUND);

    return read_write("objectProtectionSet: ", dbh, 0, 4,
		      (void *)protoid, oid, 0, 0, PWRITE, LockS, 0,
		      opmode);
  }

  Status
  ESM_objectProtectionGet(DbHandle const *dbh, Oid const *const oid,
			  Oid *protoid)
  {
    return read_write("objectProtectionGet: ", dbh, 0, 4,
		      (void *)protoid, oid, 0, 0, PREAD, LockS, 0,
		      OPDefault);
  }

}
