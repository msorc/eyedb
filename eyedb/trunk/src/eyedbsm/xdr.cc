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

//#define USE_STRICT_XDR

#include "kern_p.h"

#define NEW_OID_CODE

namespace eyedbsm {

  /*
  extern void x2h_datafileDesc(DatafileDesc *hdat,
			       const DatafileDesc *xdat);  extern void h2x_datafileDesc(DatafileDesc *xdat,
			       const DatafileDesc *hdat);

  extern void x2h_dataspaceDesc(DataspaceDesc *hdsp,
				const DataspaceDesc *xdsp);
  extern void h2x_dataspaceDesc(DataspaceDesc *xdsp,
				const DataspaceDesc *hdsp);
  */

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

#if 0

#define int8_off(x, off) ((eyedblib::int8 *)((x)+(off)))
#define int16_off(x, off) ((eyedblib::int16 *)((x)+(off)))
#define uint16_off(x, off) ((eyedblib::uint16 *)((x)+(off)))
#define int32_off(x, off) ((eyedblib::int32 *)((x)+(off)))
#define uint32_off(x, off) ((eyedblib::uint32 *)((x)+(off)))

#define X2H_16(x, off) x2h_16(*int16_off(x, off))
#define X2H_32(x, off) x2h_32(*int32_off(x, off))

#define X2H_U16(x, off) x2h_u16(*uint16_off(x, off))
#define X2H_U32(x, off) x2h_u32(*uint32_off(x, off))

  void x2h_mapHeader(MapHeader *hmap, const unsigned char *xmap)
  {
    hmap->mtype() = X2H_16(xmap, MapHeader_mtype_OFF);

    hmap->sizeslot() = X2H_U32(xmap, MapHeader_sizeslot_OFF);
    hmap->pow2() = X2H_U32(xmap, MapHeader_pow2_OFF);
    hmap->nslots() = X2H_U32(xmap, MapHeader_nslots_OFF);
    hmap->nbobjs() = X2H_U32(xmap, MapHeader_nbobjs_OFF);
    hmap->mstat_mtype() = X2H_16(xmap, MapHeader_mstat_mtype_OFF);

    if (hmap->mtype() == BitmapType) {
      hmap->u_bmh_slot_cur() = X2H_U32(xmap, MapHeader_u_bmh_slot_cur_OFF);
      hmap->u_bmh_slot_lastbusy() = X2H_U32(xmap, MapHeader_u_bmh_slot_lastbusy_OFF);
      hmap->u_bmh_retry() = X2H_U16(xmap, MapHeader_u_bmh_retry_OFF);

      hmap->mstat_u_bmstat_obj_count() = X2H_U32(xmap, MapHeader_mstat_u_bmstat_obj_count_OFF);
      hmap->mstat_u_bmstat_busy_slots() = X2H_U32(xmap, MapHeader_mstat_u_bmstat_busy_slots_OFF);
      hmap->mstat_u_bmstat_busy_size() = X2H_U32(xmap, MapHeader_mstat_u_bmstat_busy_size_OFF);
      hmap->mstat_u_bmstat_hole_size() = X2H_U32(xmap, MapHeader_mstat_u_bmstat_hole_size_OFF);
    }
    else if (hmap->mtype() == LinkmapType) {
      abort();
    }
    else
      abort();
  }

  void h2x_mapHeader(unsigned char *xmap, const MapHeader *_hmap)
  {
    MapHeader *hmap = const_cast<MapHeader *>(_hmap);
    unsigned short mtype = hmap->mtype();

    *int16_off(xmap, MapHeader_mtype_OFF) = h2x_16(hmap->mtype());
    *uint32_off(xmap, MapHeader_sizeslot_OFF) = h2x_u32(hmap->sizeslot());
    *uint32_off(xmap, MapHeader_pow2_OFF) = h2x_u32(hmap->pow2());
    *uint32_off(xmap, MapHeader_nslots_OFF) = h2x_u32(hmap->nslots());
    *uint32_off(xmap, MapHeader_nbobjs_OFF) = h2x_u32(hmap->nbobjs());
    *int16_off(xmap, MapHeader_mstat_mtype_OFF) = h2x_16(hmap->mstat_mtype());

    if (mtype == BitmapType) {
      *uint32_off(xmap, MapHeader_u_bmh_slot_cur_OFF) = h2x_u32(hmap->u_bmh_slot_cur());
      *uint32_off(xmap, MapHeader_u_bmh_slot_lastbusy_OFF) = h2x_u32(hmap->u_bmh_slot_lastbusy());
      *uint16_off(xmap, MapHeader_u_bmh_retry_OFF) = h2x_u16(hmap->u_bmh_retry());

      *uint32_off(xmap, MapHeader_mstat_u_bmstat_obj_count_OFF) = h2x_u32(hmap->mstat_u_bmstat_obj_count());
      *uint32_off(xmap, MapHeader_mstat_u_bmstat_busy_slots_OFF) = h2x_u32(hmap->mstat_u_bmstat_busy_slots());
      *uint32_off(xmap, MapHeader_mstat_u_bmstat_busy_size_OFF) = h2x_u32(hmap->mstat_u_bmstat_busy_size());
      *uint32_off(xmap, MapHeader_mstat_u_bmstat_hole_size_OFF) = h2x_u32(hmap->mstat_u_bmstat_hole_size());
    }
    else if (mtype == LinkmapType) {
      abort();
    }
    else
      abort();
  }

  void x2h_datafileDesc(DatafileDesc *hdat,
			const unsigned char *xdat)
  {
    memcpy(hdat->file(), xdat + DatafileDesc_file_OFF, L_FILENAME);
    memcpy(hdat->name(), xdat + DatafileDesc_name_OFF, L_NAME+1);

    hdat->__maxsize() = X2H_U32(xdat, DatafileDesc___maxsize_OFF);
    x2h_mapHeader(hdat->mp(), xdat + DatafileDesc_mp_OFF);
    hdat->__lastslot() = X2H_U32(xdat, DatafileDesc___lastslot_OFF);
    hdat->__dspid() = X2H_16(xdat, DatafileDesc___dspid_OFF);
  }

  void h2x_datafileDesc(unsigned char *xdat,
			const DatafileDesc *_hdat)
  {
    DatafileDesc *hdat = const_cast<DatafileDesc *>(_hdat);
    memcpy(xdat + DatafileDesc_file_OFF, hdat->file(), L_FILENAME);
    memcpy(xdat + DatafileDesc_name_OFF, hdat->name(), L_NAME+1);

    *uint32_off(xdat, DatafileDesc___maxsize_OFF) = h2x_u32(hdat->__maxsize());
    h2x_mapHeader(xdat + DatafileDesc_mp_OFF, hdat->mp());
    *uint32_off(xdat, DatafileDesc___lastslot_OFF) = h2x_u32(hdat->__lastslot());
    *int16_off(xdat, DatafileDesc___dspid_OFF) = h2x_16(hdat->__dspid());
  }


  void x2h_dataspaceDesc(DataspaceDesc *hdsp,
			 const unsigned char *xdsp)
  {
    memcpy(hdsp->name(), xdsp + DataspaceDesc_name_OFF, L_NAME+1);

    hdsp->__cur() = X2H_32(xdsp, DataspaceDesc___cur_OFF);
    hdsp->__ndat() = X2H_32(xdsp, DataspaceDesc___ndat_OFF);
    for (int i = 0; i < hdsp->__ndat(); i++)
      hdsp->__datid(i) = X2H_16(xdsp, DataspaceDesc___datid_OFF(i));
  }

  void h2x_dataspaceDesc(unsigned char *xdsp,
			 const DataspaceDesc *_hdsp)
  {
    DataspaceDesc *hdsp = const_cast<DataspaceDesc *>(_hdsp);
    unsigned int ndat = hdsp->__ndat();

    memcpy(xdsp + DataspaceDesc_name_OFF, hdsp->name(), L_NAME+1);

    *int32_off(xdsp, DataspaceDesc___cur_OFF) = h2x_32(hdsp->__cur());
    *int32_off(xdsp, DataspaceDesc___ndat_OFF) = h2x_32(hdsp->__ndat());
    for (int i = 0; i < ndat; i++)
      *int16_off(xdsp, DataspaceDesc___datid_OFF(i)) = h2x_16(hdsp->__datid(i));
  }

  void x2h_dbHeader(DbHeader *hdbh, const unsigned char *xdbh)
  {
    hdbh->__magic() = X2H_U32(xdbh, DbHeader___magic_OFF);
    hdbh->state() = *int8_off(xdbh, DbHeader_state_OFF);
    hdbh->__dbid() = X2H_32(xdbh, DbHeader___dbid_OFF);
    hdbh->__guest_uid() = X2H_32(xdbh, DbHeader___guest_uid_OFF);
    x2h_oid(&hdbh->__prot_uid_oid(), xdbh + DbHeader___prot_uid_oid_OFF);
    x2h_oid(&hdbh->__prot_list_oid(), xdbh + DbHeader___prot_list_oid_OFF);
    x2h_oid(&hdbh->__prot_lock_oid(), xdbh + DbHeader___prot_lock_oid_OFF);
    memcpy(hdbh->shmfile(), xdbh + DbHeader_shmfile_OFF, L_FILENAME);
    hdbh->__nbobjs() = X2H_U32(xdbh, DbHeader___nbobjs_OFF);
    hdbh->__ndat() = X2H_U32(xdbh, DbHeader___ndat_OFF);
    for (int i = 0; i < hdbh->__ndat(); i++) {
      DatafileDesc d = hdbh->dat(i);
      x2h_datafileDesc(&d, xdbh + DbHeader_dat_OFF(i));
    }
    hdbh->__ndsp() = X2H_U32(xdbh, DbHeader___ndsp_OFF);
    for (int i = 0; i < hdbh->__ndsp(); i++) {
      DataspaceDesc d = hdbh->dsp(i);
      x2h_dataspaceDesc(&d, xdbh + DbHeader_dsp_OFF(i));
    }
    hdbh->__def_dspid() = X2H_16(xdbh, DbHeader___def_dspid_OFF);
    hdbh->__lastidxbusy() = X2H_U32(xdbh, DbHeader___lastidxbusy_OFF);
    hdbh->__curidxbusy() = X2H_U32(xdbh, DbHeader___curidxbusy_OFF);
    hdbh->__lastidxblkalloc() = X2H_U32(xdbh, DbHeader___lastidxblkalloc_OFF);

    for (int i = 0; i < hdbh->__ndat(); i++)
      hdbh->__lastnsblkalloc(i) = X2H_U32(xdbh, DbHeader___lastnsblkalloc_OFF(i));
  }

  void h2x_dbHeader(unsigned char *xdbh, const DbHeader *_hdbh)
  {
    DbHeader *hdbh = const_cast<DbHeader *>(_hdbh);
    unsigned int ndat = hdbh->__ndat();
    unsigned int ndsp = hdbh->__ndsp();

    *uint32_off(xdbh, DbHeader___magic_OFF) = h2x_u32(hdbh->__magic());
    *int32_off(xdbh, DbHeader___dbid_OFF) = h2x_32(hdbh->__dbid());
    *int8_off(xdbh, DbHeader_state_OFF) = hdbh->state();
    *int32_off(xdbh, DbHeader___guest_uid_OFF) = h2x_32(hdbh->__guest_uid());
    h2x_oid(xdbh + DbHeader___prot_uid_oid_OFF, &hdbh->__prot_uid_oid());
    h2x_oid(xdbh + DbHeader___prot_list_oid_OFF, &hdbh->__prot_list_oid());
    h2x_oid(xdbh + DbHeader___prot_lock_oid_OFF, &hdbh->__prot_lock_oid());
    memcpy(xdbh + DbHeader_shmfile_OFF, hdbh->shmfile(), L_FILENAME);
    *uint32_off(xdbh, DbHeader___nbobjs_OFF) = h2x_u32(hdbh->__nbobjs());
    *uint32_off(xdbh, DbHeader___ndat_OFF) = h2x_u32(hdbh->__ndat());
    for (int i = 0; i < ndat; i++) {
      DatafileDesc d = hdbh->dat(i);
      h2x_datafileDesc(xdbh + DbHeader_dat_OFF(i), &d);
    }
    *uint32_off(xdbh, DbHeader___ndsp_OFF) = h2x_u32(hdbh->__ndsp());
    for (int i = 0; i < ndsp; i++) {
      DataspaceDesc d = hdbh->dsp(i);
      h2x_dataspaceDesc(xdbh + DbHeader_dsp_OFF(i), &d);
    }
    *int16_off(xdbh, DbHeader___def_dspid_OFF) = h2x_16(hdbh->__def_dspid());
    *uint32_off(xdbh, DbHeader___lastidxbusy_OFF) = h2x_u32(hdbh->__lastidxbusy());
    *uint32_off(xdbh, DbHeader___curidxbusy_OFF) = h2x_u32(hdbh->__curidxbusy());
    *uint32_off(xdbh, DbHeader___lastidxblkalloc_OFF) = h2x_u32(hdbh->__lastidxblkalloc());
    for (int i = 0; i < ndat; i++)
      *uint32_off(xdbh, DbHeader___lastnsblkalloc_OFF(i)) = h2x_u32(hdbh->__lastnsblkalloc(i));
  }
#endif

#if 0
  void
  x2h_protoids(Oid *prot_lock_oid, Oid *prot_list_oid,
	       Oid *prot_uid_oid, unsigned char *xdbh)
  {
    /*
    x2h_oid(prot_uid_oid, xdbh + DbHeader___prot_uid_oid_OFF);
    x2h_oid(prot_list_oid, xdbh + DbHeader___prot_list_oid_OFF);
    x2h_oid(prot_lock_oid, xdbh + DbHeader___prot_lock_oid_OFF);
    */

    x2h_oid(prot_list_oid, &dbh->__prot_list_oid);
    x2h_oid(prot_lock_oid, &dbh->__prot_lock_oid);
    x2h_oid(prot_uid_oid, &dbh->__prot_uid_oid);
  }

  void
  h2x_protoids(const Oid *prot_lock_oid, const Oid *prot_list_oid,
	       const Oid *prot_uid_oid, unsigned char *xdbh)
  {
    /*
    h2x_oid(xdbh + DbHeader___prot_uid_oid_OFF, prot_uid_oid);
    h2x_oid(xdbh + DbHeader___prot_list_oid_OFF, prot_list_oid);
    h2x_oid(xdbh + DbHeader___prot_lock_oid_OFF, prot_lock_oid);
    */

    h2x_oid(&dbh->__prot_list_oid, prot_list_oid);
    h2x_oid(&dbh->__prot_lock_oid, prot_lock_oid);
    h2x_oid(&dbh->__prot_uid_oid, prot_uid_oid);
  }
#endif

  // --------------------------

  void x2h_mapHeader(MapHeader *hmap, const MapHeader *_xmap)
  {
#ifdef USE_STRICT_XDR
    x2h_mapHeader(hmap, (const unsigned char *)xmap);
#else

#ifdef EYEDBLIB_BIG_ENDIAN
    if (hmap != xmap)
      memcpy(hmap, xmap, sizeof(*hmap));
#else
    MapHeader *xmap = const_cast<MapHeader *>(_xmap);
    hmap->mtype() = x2h_16(xmap->mtype());
    hmap->sizeslot() = x2h_u32(xmap->sizeslot());
    hmap->pow2() = x2h_u32(xmap->pow2());
    hmap->nslots() = x2h_u32(xmap->nslots());
    hmap->nbobjs() = x2h_u32(xmap->nbobjs());
    hmap->mstat_mtype() = x2h_16(xmap->mstat_mtype());

    if (hmap->mtype() == BitmapType) {
      hmap->u_bmh_slot_cur() = x2h_u32(xmap->u_bmh_slot_cur());
      hmap->u_bmh_slot_lastbusy() = x2h_u32(xmap->u_bmh_slot_lastbusy());
      hmap->u_bmh_retry() = x2h_u16(xmap->u_bmh_retry());

      hmap->mstat_u_bmstat_obj_count() = x2h_u32(xmap->mstat_u_bmstat_obj_count());
      hmap->mstat_u_bmstat_busy_slots() = x2h_u32(xmap->mstat_u_bmstat_busy_slots());
      hmap->mstat_u_bmstat_busy_size() = x2h_u32(xmap->mstat_u_bmstat_busy_size());
      hmap->mstat_u_bmstat_hole_size() = x2h_u32(xmap->mstat_u_bmstat_hole_size());
    }
    else if (hmap->mtype() == LinkmapType) {
      abort();
    }
    else
      abort();
#endif
#endif
  }

  void h2x_mapHeader(MapHeader *xmap, const MapHeader *_hmap)
  {
#ifdef USE_STRICT_XDR
    h2x_mapHeader((unsigned char *)xmap, hmap);
#else

#ifdef EYEDBLIB_BIG_ENDIAN
    if (hmap != xmap)
      memcpy(xmap, hmap, sizeof(*hmap));
#else
    MapHeader *hmap = const_cast<MapHeader *>(_hmap);
    unsigned short mtype = hmap->mtype();
    xmap->mtype() = h2x_16(hmap->mtype());
    xmap->sizeslot() = h2x_u32(hmap->sizeslot());
    xmap->pow2() = h2x_u32(hmap->pow2());
    xmap->nslots() = h2x_u32(hmap->nslots());
    xmap->nbobjs() = h2x_u32(hmap->nbobjs());
    xmap->mstat_mtype() = h2x_16(hmap->mstat_mtype());

    if (mtype == BitmapType) {
      xmap->u_bmh_slot_cur() = h2x_u32(hmap->u_bmh_slot_cur());
      xmap->u_bmh_slot_lastbusy() = h2x_u32(hmap->u_bmh_slot_lastbusy());
      xmap->u_bmh_retry() = h2x_u16(hmap->u_bmh_retry());

      xmap->mstat_u_bmstat_obj_count() = h2x_u32(hmap->mstat_u_bmstat_obj_count());
      xmap->mstat_u_bmstat_busy_slots() = h2x_u32(hmap->mstat_u_bmstat_busy_slots());
      xmap->mstat_u_bmstat_busy_size() = h2x_u32(hmap->mstat_u_bmstat_busy_size());
      xmap->mstat_u_bmstat_hole_size() = h2x_u32(hmap->mstat_u_bmstat_hole_size());
    }
    else if (mtype == LinkmapType) {
      abort();
    }
    else
      abort();
#endif
#endif
  }

  void x2h_datafileDesc(DatafileDesc *hdat,
			const DatafileDesc *_xdat)
  {
#ifdef EYEDBLIB_BIG_ENDIAN
    if (hdat != xdat)
      memcpy(hdat, xdat, sizeof(*xdat));
#else
    DatafileDesc *xdat = const_cast<DatafileDesc *>(_xdat);
    hdat->__maxsize() = x2h_u32(xdat->__maxsize());
    x2h_mapHeader(hdat->mp(), xdat->mp());
    hdat->__lastslot() = x2h_u32(xdat->__lastslot());
    unsigned short __dspid = xdat->__dspid();
    hdat->__dspid() = x2h_16(xdat->__dspid());

    memcpy(hdat->file(), xdat->file(), L_FILENAME);
    memcpy(hdat->name(), xdat->name(), L_NAME+1);
#endif
  }

  void h2x_datafileDesc(DatafileDesc *xdat,
			const DatafileDesc *_hdat)
  {
#ifdef EYEDBLIB_BIG_ENDIAN
    if (hdat != xdat)
      memcpy(xdat, hdat, sizeof(*xdat));
#else
    DatafileDesc *hdat = const_cast<DatafileDesc *>(_hdat);
    xdat->__maxsize() = h2x_u32(hdat->__maxsize());
    h2x_mapHeader(xdat->mp(), hdat->mp());
    xdat->__lastslot() = h2x_u32(hdat->__lastslot());
    unsigned short __dspid = hdat->__dspid();
    xdat->__dspid() = h2x_16(hdat->__dspid());

    memcpy(xdat->file(), hdat->file(), L_FILENAME);
    memcpy(xdat->name(), hdat->name(), L_NAME+1);
#endif
  }


  void x2h_dataspaceDesc(DataspaceDesc *hdsp,
			 const DataspaceDesc *_xdsp)
  {
#ifdef EYEDBLIB_BIG_ENDIAN
    if (hdsp != xdsp)
      memcpy(hdsp, xdsp, sizeof(*xdsp));
#else
    DataspaceDesc *xdsp = const_cast<DataspaceDesc *>(_xdsp);
    hdsp->__cur() = x2h_32(xdsp->__cur());
    hdsp->__ndat() = x2h_32(xdsp->__ndat());
    for (int i = 0; i < hdsp->__ndat(); i++)
      hdsp->__datid(i) = x2h_16(xdsp->__datid(i));

    memcpy(hdsp->name(), xdsp->name(), L_NAME+1);
#endif
  }

  void h2x_dataspaceDesc(DataspaceDesc *xdsp,
			 const DataspaceDesc *_hdsp)
  {
#ifdef EYEDBLIB_BIG_ENDIAN
    if (hdsp != xdsp)
      memcpy(xdsp, hdsp, sizeof(*hdsp));
#else
    DataspaceDesc *hdsp = const_cast<DataspaceDesc *>(_hdsp);
    unsigned int ndat = hdsp->__ndat();

    xdsp->__cur() = h2x_32(hdsp->__cur());
    xdsp->__ndat() = h2x_32(hdsp->__ndat());
    for (int i = 0; i < ndat; i++)
      xdsp->__datid(i) = h2x_16(hdsp->__datid(i));

    memcpy(xdsp->name(), hdsp->name(), L_NAME+1);
#endif
  }

  void x2h_dbHeader(DbHeader *hdbh, const DbHeader *_xdbh)
  {
#ifdef USE_STRICT_XDR
    x2h_dbHeader(hdbh, (const unsigned char *)xdbh);
#else

#ifdef EYEDBLIB_BIG_ENDIAN
    if (hdbh != xdbh)
      memcpy(hdbh, xdbh, sizeof(*hdbh));
#else
    DbHeader *xdbh = const_cast<DbHeader *>(_xdbh);
    hdbh->__magic() = x2h_u32(xdbh->__magic());
    hdbh->__dbid() = x2h_32(xdbh->__dbid());
    hdbh->__guest_uid() = x2h_32(xdbh->__guest_uid());
    hdbh->state() = xdbh->state();
    memcpy(hdbh->shmfile(), xdbh->shmfile(), L_FILENAME);

    Oid hoid, xoid;
    xoid = xdbh->__prot_uid_oid();
    x2h_oid(&hoid, &xoid);
    hdbh->__prot_uid_oid() = hoid;

    xoid = xdbh->__prot_list_oid();
    x2h_oid(&hoid, &xoid);
    hdbh->__prot_list_oid() = hoid;

    xoid = xdbh->__prot_lock_oid();
    x2h_oid(&hoid, &xoid);
    hdbh->__prot_lock_oid() = hoid;

    hdbh->__nbobjs() = x2h_u32(xdbh->__nbobjs());
    hdbh->__ndat() = x2h_u32(xdbh->__ndat());
    for (int i = 0; i < hdbh->__ndat(); i++) {
      DatafileDesc hd = hdbh->dat(i);
      DatafileDesc xd = xdbh->dat(i);
      x2h_datafileDesc(&hd, &xd);
    }
    hdbh->__ndsp() = x2h_u32(xdbh->__ndsp());
    for (int i = 0; i < hdbh->__ndsp(); i++) {
      DataspaceDesc hd = hdbh->dsp(i);
      DataspaceDesc xd = xdbh->dsp(i);
      x2h_dataspaceDesc(&hd, &xd);
    }
    hdbh->__def_dspid() = x2h_16(xdbh->__def_dspid());
    hdbh->__lastidxbusy() = x2h_u32(xdbh->__lastidxbusy());
    hdbh->__curidxbusy() = x2h_u32(xdbh->__curidxbusy());
    hdbh->__lastidxblkalloc() = x2h_u32(xdbh->__lastidxblkalloc());

    for (int i = 0; i < hdbh->__ndat(); i++)
      hdbh->__lastnsblkalloc(i) = x2h_u32(xdbh->__lastnsblkalloc(i));

    memcpy(hdbh->vre_addr(0), xdbh->vre_addr(0),
	   DbRootEntry_SIZE * MAX_ROOT_ENTRIES);

#endif
#endif
  }

  void h2x_dbHeader(DbHeader *xdbh, const DbHeader *_hdbh)
  {
#ifdef USE_STRICT_XDR
    h2x_dbHeader((unsigned char *)xdbh, hdbh);
#else

#ifdef EYEDBLIB_BIG_ENDIAN
    if (hdbh != xdbh)
      memcpy(xdbh, hdbh, sizeof(*hdbh));
#else
    DbHeader *hdbh = const_cast<DbHeader *>(_hdbh);
    unsigned int ndat = hdbh->__ndat();
    unsigned int ndsp = hdbh->__ndsp();

    xdbh->__magic() = h2x_u32(hdbh->__magic());
    xdbh->__dbid() = h2x_32(hdbh->__dbid());
    xdbh->__guest_uid() = h2x_32(hdbh->__guest_uid());
    xdbh->state() = hdbh->state();
    memcpy(xdbh->shmfile(), hdbh->shmfile(), L_FILENAME);
    
    Oid xoid, hoid;

    hoid = hdbh->__prot_uid_oid();
    h2x_oid(&xoid, &hoid);
    xdbh->__prot_uid_oid() = xoid;

    hoid = hdbh->__prot_list_oid();
    h2x_oid(&xoid, &hoid);
    xdbh->__prot_list_oid() = xoid;

    hoid = hdbh->__prot_lock_oid();
    h2x_oid(&xoid, &hoid);
    xdbh->__prot_lock_oid() = xoid;

    xdbh->__nbobjs() = h2x_u32(hdbh->__nbobjs());
    xdbh->__ndat() = h2x_u32(hdbh->__ndat());
    for (int i = 0; i < ndat; i++) {
      DatafileDesc hd = hdbh->dat(i);
      DatafileDesc xd = xdbh->dat(i);
      h2x_datafileDesc(&xd, &hd);
    }
    xdbh->__ndsp() = h2x_u32(hdbh->__ndsp());
    for (int i = 0; i < ndsp; i++) {
      DataspaceDesc hd = hdbh->dsp(i);
      DataspaceDesc xd = xdbh->dsp(i);
      h2x_dataspaceDesc(&xd, &hd);
    }
    xdbh->__def_dspid() = h2x_16(hdbh->__def_dspid());
    xdbh->__lastidxbusy() = h2x_u32(hdbh->__lastidxbusy());
    xdbh->__curidxbusy() = h2x_u32(hdbh->__curidxbusy());
    xdbh->__lastidxblkalloc() = h2x_u32(hdbh->__lastidxblkalloc());
    for (int i = 0; i < ndat; i++)
      xdbh->__lastnsblkalloc(i) = h2x_u32(hdbh->__lastnsblkalloc(i));

    memcpy(xdbh->vre_addr(0), hdbh->vre_addr(0),
	   DbRootEntry_SIZE * MAX_ROOT_ENTRIES);
#endif
#endif
  }

  void
  x2h_protoids(Oid *prot_lock_oid, Oid *prot_list_oid,
	       Oid *prot_uid_oid, DbHeader *dbh)
  {
    Oid xoid;

    xoid = dbh->__prot_list_oid();
    x2h_oid(prot_list_oid, &xoid);

    xoid = dbh->__prot_lock_oid();
    x2h_oid(prot_lock_oid, &xoid);

    xoid = dbh->__prot_uid_oid();
    x2h_oid(prot_uid_oid, &xoid);
  }

  void
  h2x_protoids(Oid *prot_lock_oid, Oid *prot_list_oid,
	       Oid *prot_uid_oid, DbHeader *dbh)
  {
    Oid xoid;

    h2x_oid(&xoid, prot_list_oid);
    dbh->__prot_list_oid() = xoid;

    h2x_oid(&xoid, prot_lock_oid);
    dbh->__prot_lock_oid() = xoid;

    h2x_oid(&xoid, prot_uid_oid);
    dbh->__prot_uid_oid() = xoid;
  }
}
