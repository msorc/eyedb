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


/*
 * 4/12/05
 * structures alignments:
 * - the eXternal data (database data) must be raw data => void * or
 *   const void * (or char *)
 * - the Host data must be structured data (MapHeader etc.)
 * - the conversion must deal with predefined offsets, for instance:

  void x2h_mapHeader(MapHeader *hmap, const char *xmap)
  {
#ifdef EYEDBLIB_BIG_ENDIAN
    hmap->mtype = *(short *)(xmap+MAPHEADER_MTYPE_OFF);
    hmap->sizelot = *(unsigned *)(xmap+MAPHEADER_SIZESLOT_OFF);
#else
    hmap->mtype = x2h_16(*(short *)(xmap+MAPHEADER_MTYPE_OFF));
    hmap->sizelot = x2h_u32(*(unsigned *)(xmap+MAPHEADER_SIZESLOT_OFF));
#endif
  }

  in calling function:

   char xmh[SIZEOF_MAPHEADR];
   objectRead(..., xmh, SIZEOF_MAPHEADER, ...);

   MapHeader mh;
   x2h_mapHeader(&mh, xmh);
 */

#include <eyedbconfig.h>

#include "kern_p.h"

#define NEW_OID_CODE

namespace eyedbsm {

  void x2h_oidloc(OidLoc *oidloc, const void *addr)
  {
    memcpy(oidloc, addr, OIDLOCSIZE);
#ifndef EYEDBLIB_BIG_ENDIAN
    oidloc->ns = x2h_u32(oidloc->ns);
    oidloc->datid = x2h_16(oidloc->datid);
#endif
  }

  // #2 argument is conceptually constant
  void h2x_oidloc(void *addr, OidLoc *oidloc)
  {
#ifndef EYEDBLIB_BIG_ENDIAN
    oidloc->ns = h2x_u32(oidloc->ns);
    oidloc->datid = h2x_16(oidloc->datid);
#endif
    memcpy(addr, oidloc, OIDLOCSIZE);
  }

  struct OidCode {
    unsigned int x[2];
  };

  void x2h_oid(Oid *hoid, const void *xoid)
  {
#ifndef EYEDBLIB_BIG_ENDIAN

#ifdef NEW_OID_CODE
    OidCode o;
    memcpy(&o, xoid, sizeof(o));

    o.x[0] = x2h_u32(o.x[0]);
    o.x[1] = x2h_u32(o.x[1]);

    hoid->setNX(o.x[0]);
    hoid->setDbID((o.x[1] >> Oid_UNIQUE) & 0x3ff);
    hoid->setUnique(o.x[1] & 0x3fffff);

#else
    unsigned long long l = 0;
    memcpy(&l, xoid, sizeof(l));
    l = x2h_u64(l);

    hoid->setNX(l >> Oid_NX);
    hoid->setDbID((l >> Oid_UNIQUE) & 0x3ff);
    hoid->setUnique(l & 0x3fffff);
#endif


#else
    if (hoid != xoid)
      memcpy(hoid, xoid, sizeof(Oid));
#endif
  }

  void h2x_oid(void *xoid, const Oid *hoid)
  {
#ifndef EYEDBLIB_BIG_ENDIAN

#ifdef NEW_OID_CODE
    OidCode o;
    o.x[0] = h2x_u32(hoid->getNX());
    o.x[1] = h2x_u32(hoid->getDbID() << Oid_UNIQUE | hoid->getUnique());

    memcpy(xoid, &o, sizeof(o));
#else
    unsigned long long l = h2x_u64(((unsigned long long)hoid->getNX() << Oid_NX |
				    hoid->getDbID() << Oid_UNIQUE | hoid->getUnique()));
    memcpy(xoid, &l, sizeof(l));
#endif

#else
    if (hoid != xoid)
      memcpy(xoid, hoid, sizeof(Oid));

#endif
  }

  void x2h_oids(Oid *hoid, const void *xoid, unsigned int cnt)
  {
#ifdef EYEDBLIB_BIG_ENDIAN
    if (hoid != xoid)
#endif
      for (int i = 0; i < cnt; i++)
	//    x2h_oid(&hoid[i], &xoid[i]);
	x2h_oid(&hoid[i], (char *)xoid+i*sizeof(Oid)); //, &xoid[i]);
  }

  void h2x_oids(void *xoid, const Oid *hoid, unsigned int cnt)
  {
#ifdef EYEDBLIB_BIG_ENDIAN
    if (hoid != xoid)
#endif
      for (int i = 0; i < cnt; i++)
	//      h2x_oid(&xoid[i], &hoid[i]);
	h2x_oid((char *)xoid+i*sizeof(Oid), &hoid[i]);
  }

  int cmp_oid(const void *xoid, const Oid *hoid)
  {
    Oid hoid_t;
    x2h_oid(&hoid_t, xoid);
    return memcmp(&hoid_t, hoid, sizeof(Oid));
  }

  int cmp_oids(const void *xoid, const Oid *hoid, unsigned int cnt)
  {
    for (int i = 0; i < cnt; i++) {
      Oid hoid_t;
      x2h_oid(&hoid_t, (char *)xoid+i*sizeof(Oid));
      int r = memcmp(&hoid_t, &hoid[i], sizeof(Oid));
      if (r) return r;
    }
  
    return 0;
  }

  unsigned int x2h_getSize(unsigned int size)
  {
    return getSize(x2h_u32(size));
  }

  unsigned int x2h_makeValid(unsigned int size)
  {
    return makeValid(x2h_u32(size));
  }

  void x2h_mapHeader(MapHeader *hmap, const MapHeader *xmap)
  {
#ifdef EYEDBLIB_BIG_ENDIAN
    if (hmap != xmap)
      memcpy(hmap, xmap, sizeof(*hmap));
#else
    hmap->mtype = x2h_16(xmap->mtype);
    hmap->sizeslot = x2h_u32(xmap->sizeslot);
    hmap->pow2 = x2h_u32(xmap->pow2);
    hmap->nslots = x2h_u32(xmap->nslots);
    hmap->nbobjs = x2h_u32(xmap->nbobjs);
    hmap->mstat.mtype = x2h_16(xmap->mstat.mtype);

    if (hmap->mtype == BitmapType) {
      hmap->u.bmh.slot_cur = x2h_u32(xmap->u.bmh.slot_cur);
      hmap->u.bmh.slot_lastbusy = x2h_u32(xmap->u.bmh.slot_lastbusy);
      hmap->u.bmh.retry = x2h_u16(xmap->u.bmh.retry);

      hmap->mstat.u.bmstat.obj_count = x2h_u32(xmap->mstat.u.bmstat.obj_count);
      hmap->mstat.u.bmstat.busy_slots = x2h_u32(xmap->mstat.u.bmstat.busy_slots);
      hmap->mstat.u.bmstat.busy_size = x2h_u32(xmap->mstat.u.bmstat.busy_size);
      hmap->mstat.u.bmstat.hole_size = x2h_u32(xmap->mstat.u.bmstat.hole_size);
    }
    else if (hmap->mtype == LinkmapType) {
      abort();
    }
    else
      abort();
#endif
  }

  void h2x_mapHeader(MapHeader *xmap, const MapHeader *hmap)
  {
#ifdef EYEDBLIB_BIG_ENDIAN
    if (hmap != xmap)
      memcpy(xmap, hmap, sizeof(*hmap));
#else
    unsigned short mtype = hmap->mtype;
    xmap->mtype = h2x_16(hmap->mtype);
    xmap->sizeslot = h2x_u32(hmap->sizeslot);
    xmap->pow2 = h2x_u32(hmap->pow2);
    xmap->nslots = h2x_u32(hmap->nslots);
    xmap->nbobjs = h2x_u32(hmap->nbobjs);
    xmap->mstat.mtype = h2x_16(hmap->mstat.mtype);

    if (mtype == BitmapType) {
      xmap->u.bmh.slot_cur = h2x_u32(hmap->u.bmh.slot_cur);
      xmap->u.bmh.slot_lastbusy = h2x_u32(hmap->u.bmh.slot_lastbusy);
      xmap->u.bmh.retry = h2x_u16(hmap->u.bmh.retry);

      xmap->mstat.u.bmstat.obj_count = h2x_u32(hmap->mstat.u.bmstat.obj_count);
      xmap->mstat.u.bmstat.busy_slots = h2x_u32(hmap->mstat.u.bmstat.busy_slots);
      xmap->mstat.u.bmstat.busy_size = h2x_u32(hmap->mstat.u.bmstat.busy_size);
      xmap->mstat.u.bmstat.hole_size = h2x_u32(hmap->mstat.u.bmstat.hole_size);
    }
    else if (mtype == LinkmapType) {
      abort();
    }
    else
      abort();
#endif
  }

  void x2h_datafileDesc(DatafileDesc *hdat,
			const DatafileDesc *xdat)
  {
#ifdef EYEDBLIB_BIG_ENDIAN
    if (hdat != xdat)
      memcpy(hdat, xdat, sizeof(*xdat));
#else
    hdat->__maxsize = x2h_u32(xdat->__maxsize);
    x2h_mapHeader(&hdat->mp, &xdat->mp);
    hdat->__lastslot = x2h_u32(xdat->__lastslot);
    unsigned short __dspid = xdat->__dspid;
    hdat->__dspid = x2h_16(xdat->__dspid);
#endif
  }

  void h2x_datafileDesc(DatafileDesc *xdat,
			const DatafileDesc *hdat)
  {
#ifdef EYEDBLIB_BIG_ENDIAN
    if (hdat != xdat)
      memcpy(xdat, hdat, sizeof(*xdat));
#else
    xdat->__maxsize = h2x_u32(hdat->__maxsize);
    h2x_mapHeader(&xdat->mp, &hdat->mp);
    xdat->__lastslot = h2x_u32(hdat->__lastslot);
    unsigned short __dspid = hdat->__dspid;
    xdat->__dspid = h2x_16(hdat->__dspid);
#endif
  }


  void x2h_dataspaceDesc(DataspaceDesc *hdsp,
			 const DataspaceDesc *xdsp)
  {
#ifdef EYEDBLIB_BIG_ENDIAN
    if (hdsp != xdsp)
      memcpy(hdsp, xdsp, sizeof(*xdsp));
#else
    hdsp->__cur = x2h_32(xdsp->__cur);
    hdsp->__ndat = x2h_32(xdsp->__ndat);
    for (int i = 0; i < hdsp->__ndat; i++)
      hdsp->__datid[i] = x2h_16(xdsp->__datid[i]);
#endif
  }

  void h2x_dataspaceDesc(DataspaceDesc *xdsp,
			 const DataspaceDesc *hdsp)
  {
#ifdef EYEDBLIB_BIG_ENDIAN
    if (hdsp != xdsp)
      memcpy(xdsp, hdsp, sizeof(*hdsp));
#else
    unsigned int ndat = hdsp->__ndat;

    xdsp->__cur = h2x_32(hdsp->__cur);
    xdsp->__ndat = h2x_32(hdsp->__ndat);
    for (int i = 0; i < ndat; i++)
      xdsp->__datid[i] = h2x_16(hdsp->__datid[i]);
#endif
  }

  void x2h_dbHeader(DbHeader *hdbh, const DbHeader *xdbh)
  {
#ifdef EYEDBLIB_BIG_ENDIAN
    if (hdbh != xdbh)
      memcpy(hdbh, xdbh, sizeof(*hdbh));
#else
    hdbh->__magic = x2h_u32(xdbh->__magic);
    hdbh->__dbid = x2h_32(xdbh->__dbid);
    hdbh->__guest_uid = x2h_32(xdbh->__guest_uid);
    x2h_oid(&hdbh->__prot_uid_oid, &xdbh->__prot_uid_oid);
    x2h_oid(&hdbh->__prot_list_oid, &xdbh->__prot_list_oid);
    x2h_oid(&hdbh->__prot_lock_oid, &xdbh->__prot_lock_oid);
    hdbh->__nbobjs = x2h_u32(xdbh->__nbobjs);
    hdbh->__ndat = x2h_u32(xdbh->__ndat);
    for (int i = 0; i < hdbh->__ndat; i++)
      x2h_datafileDesc(&hdbh->dat[i], &xdbh->dat[i]);
    hdbh->__ndsp = x2h_u32(xdbh->__ndsp);
    for (int i = 0; i < hdbh->__ndsp; i++)
      x2h_dataspaceDesc(&hdbh->dsp[i], &xdbh->dsp[i]);
    hdbh->__def_dspid = x2h_16(xdbh->__def_dspid);
    hdbh->__lastidxbusy = x2h_u32(xdbh->__lastidxbusy);
    hdbh->__curidxbusy = x2h_u32(xdbh->__curidxbusy);
    hdbh->__lastidxblkalloc = x2h_u32(xdbh->__lastidxblkalloc);

    for (int i = 0; i < hdbh->__ndat; i++)
      hdbh->__lastnsblkalloc[i] = x2h_u32(xdbh->__lastnsblkalloc[i]);
#endif
  }

  void h2x_dbHeader(DbHeader *xdbh, const DbHeader *hdbh)
  {
#ifdef EYEDBLIB_BIG_ENDIAN
    if (hdbh != xdbh)
      memcpy(xdbh, hdbh, sizeof(*hdbh));
#else
    unsigned int ndat = hdbh->__ndat;
    unsigned int ndsp = hdbh->__ndsp;

    xdbh->__magic = h2x_u32(hdbh->__magic);
    xdbh->__dbid = h2x_32(hdbh->__dbid);
    xdbh->__guest_uid = h2x_32(hdbh->__guest_uid);
    h2x_oid(&xdbh->__prot_uid_oid, &hdbh->__prot_uid_oid);
    h2x_oid(&xdbh->__prot_list_oid, &hdbh->__prot_list_oid);
    h2x_oid(&xdbh->__prot_lock_oid, &hdbh->__prot_lock_oid);
    xdbh->__nbobjs = h2x_u32(hdbh->__nbobjs);
    xdbh->__ndat = h2x_u32(hdbh->__ndat);
    for (int i = 0; i < ndat; i++)
      h2x_datafileDesc(&xdbh->dat[i], &hdbh->dat[i]);
    xdbh->__ndsp = h2x_u32(hdbh->__ndsp);
    for (int i = 0; i < ndsp; i++)
      h2x_dataspaceDesc(&xdbh->dsp[i], &hdbh->dsp[i]);
    xdbh->__def_dspid = h2x_16(hdbh->__def_dspid);
    xdbh->__lastidxbusy = h2x_u32(hdbh->__lastidxbusy);
    xdbh->__curidxbusy = h2x_u32(hdbh->__curidxbusy);
    xdbh->__lastidxblkalloc = h2x_u32(hdbh->__lastidxblkalloc);
    for (int i = 0; i < ndat; i++)
      xdbh->__lastnsblkalloc[i] = h2x_u32(hdbh->__lastnsblkalloc[i]);
#endif
  }

  void
  x2h_protoids(Oid *prot_lock_oid, Oid *prot_list_oid,
	       Oid *prot_uid_oid, DbHeader *dbh)
  {
    x2h_oid(prot_list_oid, &dbh->__prot_list_oid);
    x2h_oid(prot_lock_oid, &dbh->__prot_lock_oid);
    x2h_oid(prot_uid_oid, &dbh->__prot_uid_oid);
  }

  void
  h2x_protoids(Oid *prot_lock_oid, Oid *prot_list_oid,
	       Oid *prot_uid_oid, DbHeader *dbh)
  {
    h2x_oid(&dbh->__prot_list_oid, prot_list_oid);
    h2x_oid(&dbh->__prot_lock_oid, prot_lock_oid);
    h2x_oid(&dbh->__prot_uid_oid, prot_uid_oid);
  }

}
