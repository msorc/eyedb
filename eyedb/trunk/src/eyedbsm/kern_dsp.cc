/* 
   EyeDB Object Database Management System
   Copyright (C) 1994-2008 SYSRA
   
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

Boolean
isDspValid(DbHandle const *dbh, short dspid)
{
  DbHeader _dbh(DBSADDR(dbh));
  return (dspid >= 0 && dspid < x2h_u32(_dbh.__ndsp()) &&
	  *_dbh.dsp(dspid).name()) ?
    True : False;
}

Status
ESM_dspGet(DbHandle const *dbh, const char *dataspace, short *dspid)
{
  if (is_number(dataspace))
    {
      *dspid = atoi(dataspace);
      if (!isDspValid(dbh, *dspid))
	return statusMake(INVALID_DATASPACE, "invalid dataspace #%d",
			     *dspid);
      return Success;
    }

  DbHeader _dbh(DBSADDR(dbh));
  for (int i = 0; i < MAX_DATASPACES; i++)
    if (!strcmp(_dbh.dsp(i).name(), dataspace)) {
      *dspid = i;
      return Success;
    }

  return statusMake(INVALID_DATASPACE, "dataspace %s not found",
		    dataspace);
}

Status
ESM_getDatafile(DbHandle const *dbh, short &dspid, short &datid)
{
  Status s;
  if (dspid == DefaultDspid) {
    s = ESM_dspGetDefault(dbh, &dspid);
    if (s) return s;
  }

  if (!isDspValid(dbh, dspid))
    return statusMake(INVALID_DATASPACE, "invalid dataspace #%d", dspid);

  DataspaceDesc dsp = DbHeader(DBSADDR(dbh)).dsp(dspid);
  datid = x2h_16(dsp.__datid(x2h_32(dsp.__cur())));

  /*
  if (!isDatValid(dbh, datid))
    return statusMake(INVALID_DATAFILE, "datafile #%d is not valid",
			 datpid);
  */
  return Success;
}

Boolean
ESM_getNextDatafile(DbHandle const *dbh, short dspid, short &datid)
{
  DataspaceDesc dsp = DbHeader(DBSADDR(dbh)).dsp(dspid);
  int cur = x2h_32(dsp.__cur());

  // 07/01/05: see below
  // could be: if (datid != x2h_16(dsp->__datid[cur])) return False; ??
  //assert(datid == x2h_16(dsp->__datid[cur]));
  if (datid != x2h_16(dsp.__datid(cur))) {
    fprintf(stderr, "*WARNING*: ESM_getNextDataFile : "
	    "datid != x2h_16(dsp->__datid[cur]): %d != %d\n", datid,
	    x2h_16(dsp.__datid(cur)));
  }

  int ndat = x2h_32(dsp.__ndat());
  if (cur == ndat - 1)
    return False;

  // 07/01/05: corrected this bug (see tests/dataspace/bug_dsp.sh)
  //datid = x2h_16(dsp->__datid[cur]);
  //cur++;
  datid = x2h_16(dsp.__datid(++cur));

  dsp.__cur() = h2x_32(cur);
  return True;
}

Status
ESM_dspSetDefault(DbHandle const *dbh, const char *dataspace,
		 Boolean fromDbCreate)
{
  if (!fromDbCreate) {
    CHECK_X(dbh, "setting a default dataspace");
  }

  short dspid;
  Status s = ESM_dspGet(dbh, dataspace, &dspid);
  if (s) return s;
  DbHeader(DBSADDR(dbh)).__def_dspid() = h2x_16(dspid);
  return Success;
}

Status
ESM_dspGetDefault(DbHandle const *dbh, short *dspid)
{
  *dspid = x2h_16(DbHeader(DBSADDR(dbh)).__def_dspid());
  return Success;
}

Status
ESM_dspCreateRealize(DbHandle const *dbh, const char *op,
		    short dspid, const char *dataspace,
		    const char **datfiles, unsigned int datfile_cnt)
{
  if (datfile_cnt >= MAX_DAT_PER_DSP) {
    return statusMake(INVALID_DATAFILE_CNT_IN_DATASPACE,
		      "%s too many datafiles in dataspace: %u, maximum is %u",
		      op, datfile_cnt, MAX_DAT_PER_DSP);
  }

  short *datid = new short[datfile_cnt];
  DbHeader _dbh(DBSADDR(dbh));
  DatType dtype;
  for (int i = 0; i < datfile_cnt; i++) {
    short xdspid;
    Status s = ESM_datCheck(dbh, datfiles[i], &datid[i], &xdspid);
    if (s) {
      delete [] datid;
      return s;
    }

    if (xdspid != DefaultDspid && xdspid != dspid) {
      delete [] datid;
      return statusMake(INVALID_DATASPACE, "datafile %s is already "
			   "tied to the dataspace %s",
			   datfiles[i], _dbh.dsp(xdspid).name());
    }

    if (!i)
      dtype = getDatType(&_dbh, datid[i]);
    else if (dtype != getDatType(&_dbh, datid[i])) {
      delete [] datid;
      return statusMake(INVALID_DATASPACE, "cannot gather different "
			   "oid type based datafiles into a dataspace");
    }
  }

  DataspaceDesc dsp = _dbh.dsp(dspid);
  strcpy(dsp.name(), dataspace);
  dsp.__cur() = 0;
  dsp.__ndat() = h2x_u32(datfile_cnt);
  //memcpy(dsp->__datid, datid, sizeof(short) * datfile_cnt);
  for (int i = 0; i < datfile_cnt; i++) {
    dsp.__datid(i) = h2x_16(datid[i]);
    //DBSADDR(dbh)->dat[datid[i]].__dspid = h2x_16(dspid);
    setDataspace(&_dbh, datid[i], dspid);
  }

  unsigned int ndsp = x2h_32(_dbh.__ndsp());
  if (dspid == ndsp) {
    ndsp++;
    _dbh.__ndsp() = h2x_32(ndsp);
  }

  delete [] datid;
  return Success;
}

Status
ESM_dspSetCurDat(DbHandle const *dbh, const char *dataspace, const char *datfile)
{
  CHECK_X(dbh, "setting current datafile to a dataspace");

  short dspid;
  Status s = ESM_dspGet(dbh, dataspace, &dspid);
  if (s) return s;

  short datid, xdspid;
  s = ESM_datCheck(dbh, datfile, &datid, &xdspid);
  if (s) return s;
  
  DataspaceDesc dsp = DbHeader(DBSADDR(dbh)).dsp(dspid);

  unsigned int ndat = x2h_u32(dsp.__ndat());
  for (int i = 0; i < ndat; i++) {
    if (x2h_16(dsp.__datid(i)) == datid) {
      dsp.__cur() = h2x_32(i);
      return Success;
    }
  }

  return statusMake(ERROR, "datafile %s is not tied to "
		       "to dataspace #%d [%s]",
		       datfile, dspid, dsp.name());
}

Status
ESM_dspGetCurDat(DbHandle const *dbh, const char *dataspace, short *datid)
{
  short dspid;
  Status s = ESM_dspGet(dbh, dataspace, &dspid);
  if (s) return s;

  DataspaceDesc dsp = DbHeader(DBSADDR(dbh)).dsp(dspid);

  *datid = x2h_16(dsp.__datid(x2h_32(dsp.__cur())));
  return Success;
}

Status
ESM_dspCheck(DbHandle const *dbh, const char *dataspace, short *dspid,
	    short datid[], unsigned int *ndat)
{
  Status s = ESM_dspGet(dbh, dataspace, dspid);
  if (s) return s;

  if (ndat || datid) {
    DataspaceDesc dsp = DbHeader(DBSADDR(dbh)).dsp(*dspid);
    if (ndat) *ndat = x2h_u32(dsp.__ndat());
    if (datid) {
      unsigned int _ndat = x2h_u32(dsp.__ndat());
      for (int i = 0; i < _ndat; i++)
	datid[i] = x2h_16(dsp.__datid(i));
    }
  }

  return Success;
}

Status
ESM_dspCreate(DbHandle const *dbh, const char *dataspace,
	     const char **datfiles, unsigned int datfile_cnt,
	     Boolean fromDbCreate)
{
  if (!fromDbCreate) {
    CHECK_X(dbh, "creating a dataspace");
  }

  short dspid;
  Status s = ESM_dspGet(dbh, dataspace, &dspid);
  if (!s)
    return statusMake(INVALID_DATASPACE, "dataspace already exist %s",
			 dataspace);

  if (strlen(dataspace) >= L_NAME)
    return statusMake(INVALID_DATASPACE, "dataspace name %s is too "
			 "large, maximum size is %d",
			 dataspace, L_NAME);

  for (dspid = 0; dspid < MAX_DATASPACES; dspid++)
    if (!isDspValid(dbh, dspid))
      break;

#undef PR
#define PR "dspCreate: "
  if (dspid == MAX_DATASPACES)
    return statusMake(INVALID_DATAFILE_CNT,
			 PR " dataspace number too large: `%d'",
			 x2h_u32(DbHeader(DBSADDR(dbh)).__ndsp()));

  return ESM_dspCreateRealize(dbh, PR, dspid, dataspace, datfiles, datfile_cnt);
}

Status
ESM_dspUpdate(DbHandle const *dbh, const char *dataspace,
	     const char **datfiles, unsigned int datfile_cnt)
{
  CHECK_X(dbh, "updating a dataspace");

  short dspid;
  Status s = ESM_dspGet(dbh, dataspace, &dspid);
  if (s) return s;

#undef PR
#define PR "dspUpdate: "
  return ESM_dspCreateRealize(dbh, PR, dspid, DbHeader(DBSADDR(dbh)).dsp(dspid).name(),
			     datfiles, datfile_cnt);
}

Status
ESM_dspDelete(DbHandle const *dbh, const char *dataspace)
{
  CHECK_X(dbh, "deleting a dataspace");

  short dspid;
  Status s = ESM_dspGet(dbh, dataspace, &dspid);
  if (s) return s;

  short dspid_def;
  s = ESM_dspGetDefault(dbh, &dspid_def);
  if (s) return s;

  DataspaceDesc dsp = DbHeader(DBSADDR(dbh)).dsp(dspid);

  if (dspid == dspid_def)
    return statusMake(ERROR, "cannot delete default dataspace #%d [%s]",
			 dspid, dsp.name());

  unsigned int ndat = x2h_u32(dsp.__ndat());
  /*
  for (int i = 0; i < ndat; i++)
    DBSADDR(dbh)->dat[x2h_16(dsp->__datid[i])].__dspid =
      h2x_32(DefaultDspid);
  */

  DbHeader _dbh(DBSADDR(dbh));
  for (int i = 0; i < ndat; i++) {
    setDataspace(&_dbh, x2h_16(dsp.__datid(i)), DefaultDspid);
  }

  dsp.__ndat() = 0;
  *dsp.name() = 0;

  unsigned int ndsp = x2h_u32(_dbh.__ndsp());
  if (dspid == ndsp - 1) {
    ndsp--;
    DbHeader(DBSADDR(dbh)).__ndsp() = h2x_u32(ndsp);
  }

  return Success;
}

Status
ESM_dspRename(DbHandle const *dbh, const char *dataspace,
	     const char *dataspace_new)
{
  CHECK_X(dbh, "renaming a dataspace");

  short dspid;
  Status s = ESM_dspGet(dbh, dataspace, &dspid);
  if (s) return s;

  if (strlen(dataspace_new) >= L_NAME)
    return statusMake(INVALID_DATASPACE, "dataspace name %s is too "
			 "large, maximum size is %d",
			 dataspace_new, L_NAME);

  strcpy(DbHeader(DBSADDR(dbh)).dsp(dspid).name(), dataspace_new);
  return Success;
}

}
