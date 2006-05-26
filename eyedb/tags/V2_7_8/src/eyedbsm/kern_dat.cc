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

#include <eyedbconfig.h>

#include "kern_p.h"

namespace eyedbsm {

  static Status
  get_datid(DbHandle const *dbh, const char *datfile, short &datid,
	    short *dspid = 0)
  {
    DbHeader _dbh(DBSADDR(dbh));
    DbHeader *h = &_dbh;
    assert(x2h_u32(h->__magic()) == MAGIC);
    if (!*datfile)
      return statusMake(INVALID_DATAFILE, "invalid empty datafile name");

    if (is_number(datfile))
      {
	datid = atoi(datfile);
	if (!isDatValid(dbh, datid))
	  return statusMake(INVALID_DATAFILE, "datafile '%s' not found",
			    datfile);
	if (dspid)
	  *dspid = getDataspace(&_dbh, datid);
	return Success;
      }

    unsigned int ndat = x2h_u32(h->__ndat());
    for (datid = 0; datid < ndat; datid++)
      if ((!strcmp(datfile, h->dat(datid).name()) ||
	   !strcmp(datfile, h->dat(datid).file())) &&
	  isDatValid(dbh, datid))
	break;

    if (datid == ndat)
      return statusMake(INVALID_DATAFILE, "datafile '%s' not found",
			datfile);

    if (dspid)
      *dspid = getDataspace(h, datid);

    return Success;
  }

  Status
  checkNewDatafile(DbHeader *h, const char *file, const char *name)
  {
    if (is_number(name))
      return statusMake(INVALID_DATAFILE, "a datafile symbolic name (%s) "
			"cannot be a number", name);

    if (strlen(name) >= L_NAME)
      return statusMake(INVALID_DATAFILE, "datafile name %s is too "
			"large, maximum size is %d",
			name, L_NAME);

    if (strlen(file) >= L_FILENAME)
      return statusMake(INVALID_DATAFILE, "datafile %s is too "
			"large, maximum size is %d",
			file, L_FILENAME);

    unsigned int ndat = x2h_u32(h->__ndat());
    if (*file)
      for (int i = 0; i < ndat; i++)
	if (!strcmp(file, h->dat(i).name()) || !strcmp(file, h->dat(i).file()))
	  return statusMake(INVALID_DATAFILE,
			    "datafile %s is already in used", file);

    if (*name)
      for (int i = 0; i < ndat; i++)
	if (!strcmp(name, h->dat(i).name()) || !strcmp(name, h->dat(i).file()))
	  return statusMake(INVALID_DATAFILE,
			    "datafile name %s is already in used", name);

    return Success;
  }

  extern short
  getDataspace(const DbHeader *dbh, short datid)
  {
    assert(x2h_u32(dbh->__magic()) == MAGIC);
    return DATTYPE_CLEAN(x2h_16(dbh->dat(datid).__dspid()));
  }

  extern short
  getDataspace_inplace(const DbHeader *dbh, short datid)
  {
    assert(dbh->__magic() == MAGIC);
    return DATTYPE_CLEAN(dbh->dat(datid).__dspid());
  }

  extern void
  setDataspace(DbHeader *dbh, short datid, short dspid)
  {
    assert(x2h_u32(dbh->__magic()) == MAGIC);
    short __dspid = x2h_16(dbh->dat(datid).__dspid());
    dbh->dat(datid).__dspid() = h2x_16(dspid | (__dspid & DATTYPE_BITMASK)); 
  }

  extern void
  setDataspace_inplace(DbHeader *dbh, short datid, short dspid)
  {
    assert(dbh->__magic() == MAGIC);
    dbh->dat(datid).__dspid() = 
      (dspid | (dbh->dat(datid).__dspid() & DATTYPE_BITMASK)); 
  }

  DatType
  getDatType(DbHeader const *dbh, short datid)
  {
    assert(x2h_u32(dbh->__magic()) == MAGIC);
    return (x2h_16(dbh->dat(datid).__dspid()) & DATTYPE_BITMASK)
      ? PhysicalOidType : LogicalOidType;
  }

  DatType
  getDatType_inplace(DbHeader const *dbh, short datid)
  {
    assert(dbh->__magic() == MAGIC);
    return (dbh->dat(datid).__dspid() & DATTYPE_BITMASK)
      ? PhysicalOidType : LogicalOidType;
  }

  void
  setDatType(DbHeader *dbh, short datid, DatType dtype)
  {
    assert(x2h_u32(dbh->__magic()) == MAGIC);
    short dspid = x2h_16(dbh->dat(datid).__dspid());
    if (dtype == PhysicalOidType)
      dspid |= DATTYPE_BITMASK;
    else
      dspid &= ~DATTYPE_BITMASK;
  
    dbh->dat(datid).__dspid() = h2x_16(dspid);
  }

  void
  setDatType_inplace(DbHeader *dbh, short datid, DatType dtype)
  {
    assert(dbh->__magic() == MAGIC);
    if (dtype == PhysicalOidType)
      dbh->dat(datid).__dspid() |= DATTYPE_BITMASK;
    else
      dbh->dat(datid).__dspid() &= ~DATTYPE_BITMASK;
  }

  Status
  ESM_datCheck(DbHandle const *dbh, const char *datfile, short *datid,
	       short *dspid)
  {
    return get_datid(dbh, datfile, *datid, dspid);
  }

  Status
  ESM_datMoveObjects(DbHandle const *dbh, const char *dat_src,
		     const char *dat_dest)
  {
    short datid_src, datid_dest;
    Status s;

    if (s = get_datid(dbh, dat_src, datid_src))
      return s;

    DbHeader _dbh(DBSADDR(dbh));
    DatType dtype = getDatType(&_dbh, datid_src);
    if (dtype == PhysicalOidType)
      return statusMake(ERROR, "cannot move objects within a "
			"physical oid type based datafile");

    if (s = get_datid(dbh, dat_dest, datid_dest))
      return s;

    if (datid_src == datid_dest)
      return Success;

    Boolean found;
    Oid oid;

    if (s = ESM_firstOidGet_omp(dbh, &oid, &found))
      return s;

    while (found)
      {
	/*
	  if (s = ESM_objectLock(dbh, &oid, LOCKX, 0, 0))
	  return s;
	*/

	OidLoc oidloc = oidLocGet(dbh, &oid);
	if (oidloc.datid == datid_src) {
	  if (s = ESM_objectMoveDatDsp(dbh, &oid, datid_dest, -1, True,
				       OPDefault))
	    return s;
	}

	Oid newoid;
	if (s = ESM_nextOidGet_omp(dbh, &oid, &newoid, &found))
	  return s;

	oid = newoid;
      }

    return Success;
  }

  Status
  ESM_datCreate(DbHandle const *dbh, const char *file, const char *name,
		unsigned long long maxsize, MapType mtype, unsigned int sizeslot,
		DatType dtype)
  {
    CHECK_X(dbh, "creating a datafile");

    if (dtype != LogicalOidType &&
	dtype != PhysicalOidType)
      return statusMake(ERROR, "datafile creation: "
			"invalid datatype %d", dtype);

    DbHeader _dbh(DBSADDR(dbh));
    int ndat = x2h_u32(_dbh.__ndat());
    short datid;

    for (datid = 0; datid < MAX_DATAFILES; datid++)
      if (!isDatValid(dbh, datid))
	break;

#undef PR
#define PR "datCreate: "
    if (datid == MAX_DATAFILES)
      return statusMake(INVALID_DATAFILE_CNT,
			PR " datafile number too large: `%d'", ndat);

    DbCreateDescription dbc;
    strcpy(dbc.dat[datid].name, name);
    strcpy(dbc.dat[datid].file, file);
    dbc.dat[datid].maxsize = maxsize;
    dbc.dat[datid].mtype = mtype;
    dbc.dat[datid].sizeslot = sizeslot;
    dbc.dat[datid].dtype = dtype;

    DBFD dbfd;
    Status s = checkDatafile(PR, dbh->dbfile, &_dbh, &dbc,
			     datid, &dbfd, False, 0, True);

    if (s) return s;

    if (datid == ndat) {
      ndat++;
      _dbh.__ndat() = h2x_u32(ndat);
    }

    LASTNSBLKALLOC(dbh, datid) = 0;
    return Success;
  }

  Status
  ESM_datMove(DbHandle const *dbh, const char *datfile, const char *newdatfile,
	      Boolean force)
  {
    if (!force) {
      CHECK_X(dbh, "moving a datafile");
    }

    int fd;
    short datid;

    DbHeader _dbh(DBSADDR(dbh));
    DbHeader *h = &_dbh;

    int ndat = x2h_u32(h->__ndat());
    const char *dbdir = get_dir(dbh->dbfile);
    Status s;

    //printf("datMove '%s' '%s'\n", datfile, newdatfile);
    /* check if newdatfile already exist */
    if ((fd = ::open(newdatfile, O_RDONLY)) >= 0)
      {
	close(fd);
	return statusMake(INVALID_DATAFILE,
			  "datafile '%s' already exists", newdatfile);
      }

    /* check if datfile is registered */
    s = get_datid(dbh, datfile, datid);
    if (s) return s;

    /* check extension */
    int newdatfile_len = strlen(newdatfile);
    if (newdatfile_len <= datext_len ||
	strcmp(&newdatfile[newdatfile_len-datext_len], datext))
      return statusMake(INVALID_DBFILE,
			"invalid database file extension for `%s' "
			"(must be %s)", newdatfile, datext);

    /* perform rename */
    char *from = makefile(dbdir, h->dat(datid).file());
    char *to = makefile(dbdir, newdatfile);
    if (renamefile(from, to, dbdir, dbdir, 1))
      return statusMake(INVALID_DATAFILE,
			"move/operation failed between '%s' and '%s'",
			from, to);
	  
    from = makefile(dbdir, dmpfileGet(h->dat(datid).file()));
    to = makefile(dbdir, dmpfileGet(newdatfile));
    if (renamefile(from, to, dbdir, dbdir, 1))
      return statusMake(INVALID_DATAFILE,
			"move/operation failed between '%s' and '%s'",
			from, to);
	  
    strcpy(h->dat(datid).file(), newdatfile);
    return Success;
  }

  Status
  ESM_datResize(DbHandle const *dbh, const char *datfile,
		unsigned long long newmaxsize)
  {
    CHECK_X(dbh, "resizing a datafile");

    short datid;
    DbHeader _dbh(DBSADDR(dbh));
    DbHeader *h = &_dbh;
    Status s;

    /* check if datfile is registered */
    s = get_datid(dbh, datfile, datid);
    if (s) return s;

    /* check newmaxsize */
    if ((s = checkVolMaxSize(newmaxsize)))
      return s;

    DatafileDesc _dfd = h->dat(datid);
    DatafileDesc *dfd = &_dfd;
    MapHeader *xmp = dfd->mp();
    x2h_prologue(xmp, mp);
    unsigned int nslots;

    /* nslots number */
    if (mp->mtype() == BitmapType)
      {
	unsigned int sizeslot = mp->sizeslot();
	nslots = KB2SLOT(newmaxsize, mp->pow2());
      }
    else
      nslots = ((unsigned long long)(newmaxsize)*ONE_K)/32;

    /*
      if (nslots >= (unsigned long long)MAX_SLOTS)
      return statusMake(SIZE_TOO_LARGE, 
      "maximum slots `%d' exceeded", MAX_SLOTS);
    */

    if (nslots < mp->u_bmh_slot_lastbusy())
      return statusMake(INVALID_DATAFILEMAXSIZE,
			"datafile '%s' is partially used: "
			"size can be reduced to a minimum of "
			"'%d' Kbytes or size maybe extended.",
			dfd->file(),
			SLOT2KB(mp->u_bmh_slot_lastbusy(), mp->sizeslot()));

    // should not !!!
    // LASTNSBLKALLOC(dbh, datid) = 0;

    dfd->__maxsize() = h2x_u32(newmaxsize);
    mp->nslots() = h2x_u32(nslots);

    h2x_epilogue(xmp, mp);

    return Success;
  }

  static const char *
  get_dspname(DbHeader *h, short dspid)
  {
    const char *name = h->dsp(dspid).name();
    if (*name) return name;
    static char strname[16];
    sprintf(strname, "#%d", dspid);
    return strname;
  }

  Status
  ESM_datDelete(DbHandle const *dbh, const char *datfile, Boolean force)
  {
    if (!force) {
      CHECK_X(dbh, "deleting a datafile");
    }

    short datid;
    DbHeader _dbh(DBSADDR(dbh));
    DbHeader *h = &_dbh;

    Status s;

    /* check if datfile is registered */
    s = get_datid(dbh, datfile, datid);
    if (s) return s;

    DatafileDesc _dfd = h->dat(datid);
    DatafileDesc *dfd = &_dfd;

    short dspid = getDataspace(h, datid);
    if (!force && dspid != DefaultDspid)
      return statusMake(ERROR, "datafile '%s' is part of the dataspace "
			"'%s': cannot be deleted",
			datfile, get_dspname(h, dspid));

    MapHeader *mp = dfd->mp();

    if (!force && x2h_u32(mp->mstat_u_bmstat_busy_slots()))
      return statusMake(ERROR, "datafile '%s' is partially used: cannot "
			"be deleted", datfile);

    char *pwd;
    s = push_dir(dbh->dbfile, &pwd);
    if (s) return s;

    unlink(dfd->file());
    unlink(dmpfileGet(dfd->file()));

    s = pop_dir(pwd);
    if (s) return s;

    memset(dfd, 0, sizeof(*dfd));
    close(dbh->vd->dmd[datid].fd);
    dbh->vd->dmd[datid].fd = -1;
    unsigned int ndat = x2h_u32(h->__ndat());
    if (datid == ndat - 1) {
      ndat--;
      h->__ndat() = h2x_u32(ndat);
    }

    return Success;
  }

  static const char *
  get_tmp_datfile(const char *datfile)
  {
    static char tmp_datfile[512];

    const char *dir = get_dir(datfile);
    *tmp_datfile = 0;

    if (*dir)
      {
	strcat(tmp_datfile, dir);
	strcat(tmp_datfile, "/");
      }

    strcat(tmp_datfile, "__##__");

    const char *p = strrchr(datfile, '/');
    if (p)
      strcat(tmp_datfile, p+1);
    else
      strcat(tmp_datfile, datfile);

    return tmp_datfile;
  }

  Status
  ESM_datRename(DbHandle const *dbh, const char *datfile, const char *name)
  {
    CHECK_X(dbh, "renaming a datafile");

    short datid;
    Status s = get_datid(dbh, datfile, datid);
    if (s) return s;

    DbHeader _dbh(DBSADDR(dbh));
    s = checkNewDatafile(&_dbh, "", name);
    if (s) return s;

    strcpy(_dbh.dat(datid).name(), name);

    return Success;
  }

  Status
  ESM_datDefragment(DbHandle const *dbh, const char *datfile)
  {
    CHECK_X(dbh, "defragmenting a datafile");

    short datid;
    DbHeader _dbh(DBSADDR(dbh));
    DbHeader *h = &_dbh;

    Status s;

    /* check if datfile is registered */
    s = get_datid(dbh, datfile, datid);
    if (s) return s;

    DatType dtype = getDatType(&_dbh, datid);
    if (dtype == PhysicalOidType)
      return statusMake(ERROR, "cannot defragment a physical oid type "
			"based datafile");

    DatafileDesc _dfd = h->dat(datid);
    DatafileDesc *dfd = &_dfd;
    const MapHeader *mp = dfd->mp();

    const char *tmp_datfile = get_tmp_datfile(dfd->file());
    if (s = ESM_datCreate(dbh, tmp_datfile, "", x2h_u32(dfd->__maxsize()),
			  (MapType)x2h_u16(mp->mtype()), x2h_u32(mp->sizeslot()),
			  dtype))
      return s;

    DbHandle *dbh_n;
    if (s = ESM_dbOpen(dbh->dbfile, VOLRW, 0, 0, 0, 0, 0, 0, &dbh_n)) {
      (void)ESM_datDelete(dbh, tmp_datfile, True);
      return s;
    }

    short tmp_datid;
    s = get_datid(dbh_n, tmp_datfile, tmp_datid);

    if (s) {
      (void)ESM_datDelete(dbh, tmp_datfile, True);
      return s;
    }

    if (s = ESM_datMoveObjects(dbh_n, datfile, tmp_datfile))
      return s;

    char *datfile_keep = strdup(dfd->file());
    char *datname_keep = strdup(dfd->name());
    if (s = ESM_datDelete(dbh_n, datfile, True))
      {
	free(datfile_keep);
	free(datname_keep);
	return s;
      }

    if (s = ESM_datMove(dbh_n, tmp_datfile, datfile_keep, True))
      {
	free(datfile_keep);
	free(datname_keep);
	return s;
      }

    //DBSADDR(dbh_n)->dat[datid] = DBSADDR(dbh_n)->dat[tmp_datid];
    DbHeader _dbh_n(DBSADDR(dbh_n));

    _dbh_n.dat(datid) = _dbh_n.dat(tmp_datid);
    strcpy(_dbh_n.dat(datid).file(), datfile_keep);
    strcpy(_dbh_n.dat(datid).name(), datname_keep);
    *_dbh_n.dat(tmp_datid).file() = 0;
    *_dbh_n.dat(tmp_datid).name() = 0;

    // added the 20/03/02
    LASTNSBLKALLOC(dbh_n, datid) = LASTNSBLKALLOC(dbh_n, tmp_datid);

    unsigned int ndat = x2h_u32(_dbh_n.__ndat());
    if (tmp_datid == ndat - 1) {
      ndat--;
      _dbh_n.__ndat() = h2x_u32(ndat);
    }

    ESM_dbClose(dbh_n);

    free(datfile_keep);
    free(datname_keep);
    return Success;
  }

  static void
  ESM_datGetFragmentation(DbHandle const *dbh, MapHeader *mp, short datid,
			  unsigned int &nfrags)
  {
    char *mapaddr = dbh->vd->dmp_addr[datid];
    char *s, *start, *end;
    int nbusy, ns;

    start = mapaddr;
    end = mapaddr + (mp->u_bmh_slot_lastbusy() / 8);
    nfrags = 0;
    ns = 0;

    for (s = start; s <= end; s++)
      {
	char v = *s;
	int b;

	for (b = 7; b >= 0; b--, ns++)
	  {
	    if (!(v & (1 << b)))
	      nfrags++;

	    if (ns >= mp->u_bmh_slot_lastbusy())
	      break;
	  }
      }

    if (nfrags > mp->u_bmh_slot_lastbusy())
      nfrags = mp->u_bmh_slot_lastbusy();
  }	

  Status
  ESM_datGetDspid(DbHandle const *dbh, short datid, short *dspid)
  {
    if (!isDatValid(dbh, datid))
      return statusMake(INVALID_DATAFILE, "datafile #%d not found",
			datid);
    DbHeader _dbh(DBSADDR(dbh));
    *dspid = getDataspace(&_dbh, datid);
    return Success;
  }

  Status
  ESM_datGetInfo(DbHandle const *dbh, const char *datfile, DatafileInfo *info)
  {
    short datid;
    Status s;

    /* check if datfile is registered */
    s = get_datid(dbh, datfile, datid);
    if (s) return s;

    DbHeader _dbh(DBSADDR(dbh));
    DbHeader *h = &_dbh;

    DatafileDesc _dat = h->dat(datid);
    DatafileDesc *dat = &_dat;
    const MapHeader *xmp = dat->mp();
    x2h_prologue(xmp, mp);

    char *pwd;
    s = push_dir(dbh->dbfile, &pwd);
    if (s) return s;

    strcpy(info->datafile.file, dat->file());
    strcpy(info->datafile.name, dat->name());
    info->datid = datid;
    info->datafile.dspid = getDataspace(&_dbh, datid);
    info->datafile.maxsize = x2h_u32(dat->__maxsize());
    info->datafile.mtype = mp->mtype();
    info->datafile.sizeslot = mp->sizeslot();
    info->slotcnt = mp->nslots();
    info->objcnt = mp->mstat_u_bmstat_obj_count();
    info->totalsize = mp->mstat_u_bmstat_busy_size();
    info->avgsize = (mp->mstat_u_bmstat_obj_count() ?
		     mp->mstat_u_bmstat_busy_size()/mp->mstat_u_bmstat_obj_count() : 0);

    info->busyslotcnt = mp->mstat_u_bmstat_busy_slots();
    info->lastbusyslot = mp->u_bmh_slot_lastbusy();
    info->lastslot = x2h_u32(dat->__lastslot());
    info->busyslotsize = (unsigned long long)mp->mstat_u_bmstat_busy_slots() * mp->sizeslot();
    s = fileSizesGet(dat->file(),
		     info->datfilesize, info->datfileblksize);
    if (s) {pop_dir(pwd); return s;}

    s = fileSizesGet(dmpfileGet(dat->file()),
		     info->dmpfilesize, info->dmpfileblksize);
    pop_dir(pwd);
    if (s) return s;

    //info->totafilesize = (unsigned long long)mp->u_bmh_slot_lastbusy * mp->sizeslot;

    info->curslot = mp->u_bmh_slot_cur();
    if (!mp->u_bmh_slot_cur())
      info->defragmentablesize = 0;
    else
      info->defragmentablesize = (((unsigned long long)mp->u_bmh_slot_lastbusy()+1 - mp->mstat_u_bmstat_busy_slots()) * mp->sizeslot());

    // 22/08/01: it is not necessary to compute fragmentation with
    // ESM_datGetFragmentation
    //ESM_datGetFragmentation(dbh, mp, datid, info->slotfragcnt);
    // instead:
    info->slotfragcnt = info->defragmentablesize / mp->sizeslot();

    //printf("stats : mp->nslots %d\n", mp->nslots);
    info->used = ((double)mp->mstat_u_bmstat_busy_slots()/(double)mp->nslots())*100.;

    return Success;
  }
}
