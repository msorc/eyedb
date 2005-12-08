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

//#define CACHE_FOR_LOCK

//#define TRUSS1_GC
//#define TRUSS2_GC
#define OPT_FREELIST

#include <eyedbsm/eyedbsm.h>
#include <eyedbsm/HIdx.h>
#include "IdxP.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <eyedblib/performer.h>
#include <eyedblib/strutils.h>
#include <eyedbsm/xdr.h>
#include <eyedblib/log.h>

namespace eyedbsm {
  class MapHeader;
  class DbHeader;
  class DatafileDesc;
  class DataspaceDesc;

#define OPTIM_LARGE_OBJECTS

#define NEW_WAIT
//#define MULTI_LIST

//#define TRACK_MAP
//#define TRACK_MAP_2

#define HIDX_XDR
#define HIDX_XDR_OID
#define HIDX_XDR_OVERHEAD

#define NO_EXTEND

#define NullOffset (-1)

//#define TRACE_HIDX
//#define ESM_HIDX_REGISTER

#define NEW_HASH_KEY
#define NEW_HASH_KEY_VERSION 206004

#define STRTYPE(IDX) \
    ((IDX)->hidx.keytype == Idx::tString)

#define IDXSZ(V) (sizeof(HIdx::_Idx))

  extern Boolean backend_interrupt;

static void
dump_keytype(const char *msg, const Idx::KeyType &keytype,
	     const HIdx::_Idx &hidx)
{
  printf("%s:\n"
	 "    type=%s count=%d offset=%d\n", msg,
	 Idx::typeString(keytype.type), keytype.count, keytype.offset);
  printf("    hidx.keytype=%s hidx.keysz=%d hidx.offset=%d\n",
	 Idx::typeString((Idx::Type)hidx.keytype), hidx.keysz, hidx.offset);
}

#define mcp(D, S, N) \
do { \
  int __n__ = (N); \
  char *__d__ = (char *)(D), *__s__ = (char *)(S); \
  while(__n__--) \
    *__d__++ = *__s__++; \
} while(0)

#define mset(D, V, N) \
do { \
  int __n__ = (N); \
  char *__d__ = (char *)(D); \
  while(__n__--) \
    *__d__++ = V; \
} while(0)

static void x2h_idx(HIdx::_Idx *idx);
static void h2x_idx(HIdx::_Idx *idx, const HIdx::_Idx *);
static void x2h_hat(HIdx::Hat *);
static void h2x_hat(HIdx::Hat *, const HIdx::Hat *);
static void x2h_header(HIdx::Header *);
static void h2x_header(HIdx::Header *, const HIdx::Header *);
static void x2h_overhead(HIdx::Overhead *);
static void h2x_overhead(HIdx::Overhead *, const HIdx::Overhead *);

#define KEY_OFF (3)

#define OFFSET(T, X) (unsigned long)(&((T *)0)->X)

const char HIdxCursor::defaultSKey[] = "";

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

static const int IniSize_Default = 4096;
//static const int IniObjCnt_Default = 256;

static const unsigned int SmallThreshold_Default = 4;

static const unsigned int IniSizeSmall_Default = 256;
static const unsigned int IniObjCntSmall_Default = 8;

static const int XCoef_Default = 1;

static int
get_iniobjcnt_def(const HIdx::_Idx &hidx)
{
  if (hidx.keysz == HIdx::VarSize)
    return 0;

  return hidx.key_count <= SmallThreshold_Default ? IniObjCntSmall_Default :
    (hidx.mag_order+1) / hidx.key_count;
}

static int
get_inisize_small_def(const HIdx::_Idx &hidx)
{
  assert(hidx.key_count <= SmallThreshold_Default);

  if (hidx.keysz != HIdx::VarSize) {
    assert(hidx.impl_hints[HIdx::IniObjCnt_Hints]);
    return (sizeof(HIdx::Overhead) + hidx.keysz + hidx.datasz) * 
      hidx.impl_hints[HIdx::IniObjCnt_Hints];
    // - sizeof(HIdx::Overhead); necessary ?
  }

  return IniSizeSmall_Default;
}

static int
get_inisize_def(const HIdx::_Idx &hidx)
{
  if (hidx.key_count <= SmallThreshold_Default)
    return get_inisize_small_def(hidx);

  if (hidx.keysz != HIdx::VarSize) {
    assert(hidx.impl_hints[HIdx::IniObjCnt_Hints]);
    return (sizeof(HIdx::Overhead) + hidx.keysz + hidx.datasz) * 
      hidx.impl_hints[HIdx::IniObjCnt_Hints];
    // - sizeof(HIdx::Overhead); necessary ?
  }

  return IniSize_Default;
}

static int
get_xcoef_def(const HIdx::_Idx &)
{
  return XCoef_Default;
}

static int
get_sizemax_def(const HIdx::_Idx &hidx)
{
  assert(hidx.impl_hints[HIdx::XCoef_Hints] &&
	 hidx.impl_hints[HIdx::IniSize_Hints]);
	 
  int xcoef = hidx.impl_hints[HIdx::XCoef_Hints];
  return xcoef * xcoef * hidx.impl_hints[HIdx::IniSize_Hints];
}

static unsigned int
key_offset(int x)
{
  return x + KEY_OFF;
}

//#define IDX_DBG

Boolean trace_idx;
FILE *trace_idx_fd = stdout;
Boolean trace_idx_sync;

/*extern "C" */ unsigned int getDbVersion(void *);
/*extern "C" */ Boolean isWholeMapped(void *);

static Boolean
isPower2(int x)
{
  int n = 0;
  while(x) {
      if ((x & 1) && ++n > 1)
	return False;

      x >>= 1;
    }

  return True;
}

const unsigned int HIdx::MaxKeys = 0x800000;
const unsigned int HIdx::VarSize = 0xffffffff;
const unsigned int HIdx::MagorderKeycountCoef = 64;

#define get_gkey(V) key_offset

unsigned int
computeKeyCount(unsigned int key_count, int &mag_order,
		unsigned int maxkeys, Boolean &pow2)
{
  if (key_count) {
    key_count = (key_count > maxkeys ? maxkeys : key_count);
    pow2 = isPower2(key_count);
  }
  else {
    unsigned int ncells = (mag_order/HIdx::MagorderKeycountCoef)+1;
    if (ncells >= maxkeys-1)
      key_count = maxkeys;
    else {
      key_count = 1;
      
      for (;;) {
	if (key_count >= ncells)
	  break;
	key_count <<= 1;
      }

      pow2 = True;
    }
  }

  mag_order = key_count * HIdx::MagorderKeycountCoef-1;
  return key_count;
}

static unsigned int
get_keyTypeCount(const HIdx::_Idx &hidx)
{
  if (hidx.keysz == HIdx::VarSize)
    return HIdx::VarSize; // changed the 29/01/02: was return 0

  unsigned tsz = Idx::typeSize((Idx::Type)hidx.keytype);
  unsigned int count = (hidx.keysz - hidx.offset) / tsz;
  assert(count * tsz == (hidx.keysz - hidx.offset));

  return count;
}

//#define FORCE_COPY

void
HIdx::init(DbHandle *_dbh, unsigned int _keytype,
	      unsigned int keysz, unsigned int offset,
	      unsigned int datasz, short _dspid, int mag_order,
	      int key_count,
	      const int *impl_hints,
	      unsigned int impl_hints_cnt)
{
  dbh = _dbh;
  version = getDbVersion(dbh);
#ifdef FORCE_COPY
  nocopy = False;
#else
  nocopy = isWholeMapped(dbh);
#endif
  memset(&hidx, 0, sizeof(hidx));

  hidx.key_count = computeKeyCount(key_count, mag_order, MaxKeys, pow2);

  hidx.idxtype = HashType;
  hidx.mag_order = mag_order;
  hidx.object_count = 0;
  hidx.dspid = _dspid;
  hidx.keytype = _keytype;

  hidx.keysz = keysz;
  hidx.datasz = datasz;
  hidx.offset = offset;

  memset(&treeoid, 0, sizeof(treeoid));
#ifdef NEW_HASH_TRACE
  keytype.type = (Type)hidx.keytype;
  keytype.count = get_keyTypeCount(hidx);
  dump_keytype("creating hash index", keytype, hidx);;
#endif

  hash_data = 0;
  hash_key = 0;
#ifdef NEW_HASH_KEY
  set_hash_key();
#endif

  if (impl_hints)
    memcpy(hidx.impl_hints, impl_hints, impl_hints_cnt*sizeof(impl_hints[0]));

  if (hidx.keysz == HIdx::VarSize)
    hidx.impl_hints[IniObjCnt_Hints] = 0; // ignored
  else if (!hidx.impl_hints[IniObjCnt_Hints])
    hidx.impl_hints[IniObjCnt_Hints] = get_iniobjcnt_def(hidx);

  if (!hidx.impl_hints[IniSize_Hints])
    hidx.impl_hints[IniSize_Hints] = get_inisize_def(hidx);

  if (!hidx.impl_hints[XCoef_Hints])
    hidx.impl_hints[XCoef_Hints] = get_xcoef_def(hidx);

  if (!hidx.impl_hints[SzMax_Hints])
    hidx.impl_hints[SzMax_Hints] = get_sizemax_def(hidx);

  mask = hidx.key_count - 1;
  unsigned int (*gkey)(int) = get_gkey(version);

  int len = gkey(hidx.key_count);
  IDB_LOG(IDB_LOG_IDX_CREATE,
	  ("Creating Hash Index: magorder=%u, entries=%u "
	   "keysz=%u, datasz=%u, "
	   "size=%u [%d objects of size %u + 1 object of size %u]\n",
	   hidx.mag_order,
	   hidx.key_count,
	   hidx.keysz,
	   hidx.datasz,
	   sizeof(Hat)*hidx.key_count+len*sizeof(Oid),
	   hidx.key_count,
	   sizeof(Hat),
	   len*sizeof(Oid)));

  Hat *hats;
  hats = new Hat[len];

  memset(hats, 0, sizeof(Hat) * len);
  _Idx xidx;
  h2x_idx(&xidx, &hidx);
  mcp(hats, &xidx, sizeof(xidx));
  assert(sizeof(xidx) <= KEY_OFF * sizeof(Hat));

  bsize = hidx.impl_hints[IniSize_Hints];
  /*
    printf("creating index header size=%d, magorder=%d\n", sizeof(Oid) * len,
    hidx.mag_order);
  */
#ifdef TRACE_HIDX
  printf("creating index %d\n",  sizeof(Oid) * len);
#endif

  Hat *thats = new Hat[len];
  memcpy(thats, hats, sizeof(Hat) * KEY_OFF);
  for (int i = KEY_OFF; i < len; i++)
    h2x_hat(&thats[i], &hats[i]);
  stat = objectCreate(dbh, thats, sizeof(Hat) * len, hidx.dspid,
			 &treeoid);
  delete [] thats;

  if (!stat)
    IDB_LOG(IDB_LOG_IDX_CREATE,
	    ("Have Created Hash Index: treeoid=%s\n",
	     getOidString(&treeoid)));
  delete [] hats;
  uextend = (hidx.impl_hints[XCoef_Hints] > 1) ? True : False;
}

HIdx::HIdx(DbHandle *_dbh, KeyType _keytype,
	   unsigned int datasz, short _dspid,
	   int _mag_order, int key_count,
	   const int *impl_hints,
	   unsigned int impl_hints_cnt)
  : Idx(False)
{
  keytype = _keytype;
  init(_dbh, (unsigned int)keytype.type,
       ((unsigned int)keytype.count == VarSize ? VarSize :
	(typeSize(keytype.type) * keytype.count) + keytype.offset),
       keytype.offset,
       datasz, _dspid, _mag_order, key_count,
       impl_hints, impl_hints_cnt);
}

unsigned int
HIdx::getMagOrder(unsigned int keycount)
{
  return keycount * HIdx::MagorderKeycountCoef;
}

unsigned int
HIdx::getKeyCount(unsigned int magorder)
{
  unsigned int keycount = magorder / MagorderKeycountCoef;

  if (keycount >= MaxKeys)
    return MaxKeys;

  return keycount ? keycount : 1;
}

HIdx::HIdx(DbHandle *_dbh, const Oid *_oid,
		 hash_key_t _hash_key,
		 void *_hash_data,
		 Boolean (*precmp)(void const * p, void const * q,
				      KeyType const * type, int & r))
  : Idx(True, precmp), dbh(_dbh)
{
  version = getDbVersion(_dbh);
  stat = objectRead(dbh, 0, IDXSZ(version), &hidx, DefaultLock, 0, 0, _oid);

  /*
    printf("version = %d %d [%d vs. %d] status %d\n",
    version, IDXSZ(version), sizeof(HIdx::_Idx),
    sizeof(OIdx), stat);
  */
    
  if (stat)
    return;

  x2h_idx(&hidx);

  keytype.type = (Type)hidx.keytype;
  keytype.count = get_keyTypeCount(hidx);

  hash_key = _hash_key;
  hash_data = _hash_data;
  treeoid = *_oid;

#ifdef NEW_HASH_KEY
  set_hash_key();
#endif

  keytype.offset = hidx.offset;

  //dump_keytype("Idx::HIdx", keytype, hidx);;

  //unsigned int (*gkey)(int) = get_gkey(version);
  //int len = gkey(hidx.key_count);
  mask = hidx.key_count - 1;
  pow2 = isPower2(hidx.key_count);
  bsize = hidx.impl_hints[IniSize_Hints];
  uextend = (hidx.impl_hints[XCoef_Hints] > 1) ? True : False;
}

#ifdef NEW_HASH_KEY

//  mcp(&t, (unsigned char *)(key)+((HIdx::_Idx *)xhidx)->offset, sizeof(T));

#define MK_DEF_HASH_KEY(T, F) \
Status \
HIdx::F(const void *key, unsigned int len, void *xhidx, int &x) \
{ \
  T t; \
  mcp(&t, (unsigned char *)key, sizeof(T)); \
  x = (int)t; \
  return Success; \
}

Status
HIdx::get_def_oiddata_hash_key(const void *key, unsigned int len,
				  void *xhidx, int &x)
{
  Oid oid;
  mcp(&oid, (unsigned char *)(key), sizeof(oid));
  x = oid.getNX();
  return Success;
}

MK_DEF_HASH_KEY(eyedblib::int16, get_def_int16data_hash_key);
MK_DEF_HASH_KEY(eyedblib::int32, get_def_int32data_hash_key);
MK_DEF_HASH_KEY(eyedblib::int64, get_def_int64data_hash_key);
MK_DEF_HASH_KEY(eyedblib::float32, get_def_float32data_hash_key);
MK_DEF_HASH_KEY(eyedblib::float64, get_def_float64data_hash_key);

void
HIdx::set_hash_key()
{
#ifdef NEW_HASH_TRACE
  printf("hash_key for index %s [hash_key=%p]\n",
	 getOidString(&treeoid), hash_key);
  dump_keytype("hash_key : ", keytype, hidx);;
#endif
  if (hash_key) return;

  hash_data = 0;
  /*
  if (hidx.keytype == tString) {
    hash_key = get_def_string_hash_key;
    return;
  }
  */

  if (version >= NEW_HASH_KEY_VERSION) {
    hash_data = &hidx;

    switch(hidx.keytype) {
    case tString:
      hash_key = get_def_nstring_hash_key;
      return;

    case tInt16:
    case tUnsignedInt16:
      hash_key = get_def_int16data_hash_key;
      return;

    case tInt32:
    case tUnsignedInt32:
      hash_key = get_def_int32data_hash_key;
      return;

    case tInt64:
    case tUnsignedInt64:
      hash_key = get_def_int64data_hash_key;
      return;

    case tOid:
      hash_key = get_def_oiddata_hash_key;
      return;

    case tFloat32:
      hash_key = get_def_float32data_hash_key;
      return;

    case tFloat64:
      hash_key = get_def_float64data_hash_key;
      return;
    }

    hash_data = 0;
  }

  if (hidx.keytype == tString) {
    hash_key = get_def_string_hash_key;
    return;
  }

  hash_key = get_def_rawdata_hash_key;
}
#endif

void
HIdx::open(hash_key_t _hash_key,
	      void *_hash_data,
	      Boolean (*_precmp)(void const * p, void const * q,
				    KeyType const * type, int & r))
{
  keytype.type = (Type)hidx.keytype;
  keytype.count = get_keyTypeCount(hidx);
  keytype.offset = hidx.offset;

  hash_key = _hash_key;
  hash_data = _hash_data;
#ifdef NEW_HASH_KEY
  set_hash_key();
  assert(hash_key);
#endif
  precmp = _precmp;
  opened = True;
}

HIdx::~HIdx()
{
}

Boolean trace_it;

#define first_char ' '
#define last_char  '~'
#define asc(x) ((unsigned int)((x) - first_char))
#define asc_len ((unsigned int)(last_char - first_char + 1))

#define NEW_HASH_KEY2

Status
HIdx::get_def_string_hash_key(const void *key, unsigned int len, void *, int &x)
{
#ifdef NEW_HASH_KEY2
  unsigned char *k = (unsigned char *)key;
  x = 1;
  for (unsigned int i = 0; i < len; i++) {
    x *= *k++;
    x ^= x >> 8;
  }
#else
  x = 0;
  char *k = (char *)key;
  for (unsigned int i = 0; i < len; i++)
    x += *k++ * (i+1); //x += *k++;

#endif
  return Success;
}

Status
HIdx::get_def_nstring_hash_key(const void *key, unsigned int len, void *, int &x)
{
  unsigned char *k = (unsigned char *)key;
  if (len > 12)
    len = 12;
#ifdef NEW_HASH_KEY2
  x = 1;
  for (unsigned int i = 0; i < len; i++) {
    x *= *k++;
    x ^= x >> 8;
  }
#else
  int coef = 1;
  eyedblib::int64 r = 0;
  for (unsigned int n = 0; n < len; n++, k++) {
    r += coef * asc(*k);
    coef *= asc_len;
  }

  x = r;
#endif
  return Success;
}

Status
HIdx::get_string_hash_key(const void *key, unsigned int len, int &x) const
{
#ifdef NEW_HASH_KEY
  assert(hash_key);
#endif
  if (hash_key)
    return hash_key(key, len, hash_data, x);

  abort();
  return get_def_string_hash_key(key, len, 0, x);
}

Status
HIdx::get_def_rawdata_hash_key(const void *key, unsigned int len, void *, int &x)
{
  x = 0;
  unsigned char *k = (unsigned char *)key;
  for (unsigned int i = 0; i < len; i++)
    x += *k++;

  return Success;
}

Status
HIdx::get_rawdata_hash_key(const void *key, unsigned int len, int &x) const
{
#ifdef NEW_HASH_KEY
  assert(hash_key);
#endif
  if (hash_key)
    return hash_key(key, len, hash_data, x);

  return get_def_rawdata_hash_key(key, len, 0, x);
}

static inline void const *
fpos_(void const * p, int offset)
{
  return (char const *)p + offset;
}

inline Status
HIdx::get_key(int &n, const void *key, unsigned int *size) const
{
  key = fpos_(key, keytype.offset);
  Status s;
  int x;

  if (STRTYPE(this)) {
    int len = strlen((char *)key);
    s = get_string_hash_key(key, len, x);
    if (s) return s;
    
    if (size) {
      if (hidx.keysz == VarSize)
	*size = hidx.datasz + len + 1;
      else
	*size = hidx.datasz + hidx.keysz;
    }
    
    n = (pow2 ? (x & mask) : (x % mask));
    return Success;
  }

  s = get_rawdata_hash_key(key, hidx.keysz - keytype.offset, x);
  if (s) return s;
  if (size)
    *size = hidx.datasz + hidx.keysz;
  
  n = (pow2 ? (x & mask) : (x % mask));
  return Success;
}

Status
HIdx::suppressObjectFromFreeList(Hat &hat, int hat_k, Header &h,
				    const Oid &koid)
{
#ifdef TRACK_MAP
  printf("suppressObjectFromFreeList(%s)\n", getOidString(&koid));
#endif  
  Status s;

  if (h.free_prev.getNX()) {
    Oid xoid;
    h2x_oid(&xoid, &h.free_next);
    s = objectWrite(dbh, OFFSET(Header, free_next), sizeof(Oid),
		       &xoid, &h.free_prev);
    if (s) return s;
  }
  
  if (h.free_next.getNX()) {
    Oid xoid;
    h2x_oid(&xoid, &h.free_prev);
    s = objectWrite(dbh, OFFSET(Header, free_prev), sizeof(Oid),
		       &xoid, &h.free_next);
    if (s) return s;
  }
  
  if (hat.free_first.getNX() == koid.getNX()) {
    hat.free_first = h.free_next;
    s = writeHat(hat_k, hat);
    if (s) return s;
  }

  mset(&h.free_prev, 0, sizeof(h.free_prev));
  mset(&h.free_next, 0, sizeof(h.free_next));

  return Success;
}

Status
HIdx::suppressObjectFromList(Hat &hat, int hat_k, Header &h,
				const Oid &koid)
{
#ifdef TRACK_MAP
  printf("suppressObjectFromList(%s)\n", getOidString(&koid));
#endif  
  Status s;

  if (h.prev.getNX()) {
    Oid xoid;
    h2x_oid(&xoid, &h.next);
    s = objectWrite(dbh, OFFSET(Header, next), sizeof(Oid),
		       &xoid, &h.prev);
    if (s) return s;
  }
  
  if (h.next.getNX()) {
    Oid xoid;
    h2x_oid(&xoid, &h.prev);
    s = objectWrite(dbh, OFFSET(Header, prev), sizeof(Oid),
		       &xoid, &h.next);
    if (s) return s;
  }
  
  Boolean write_hat = False;
  if (hat.first.getNX() == koid.getNX()) {
    hat.first = h.next;
    write_hat = True;
  }

  if (hat.last.getNX() == koid.getNX()) {
    hat.last = h.prev;
    write_hat = True;
  }

  if (write_hat) {
    s = writeHat(hat_k, hat);
    if (s) return s;
  }

  return objectDelete(dbh, &koid);
}

Status
HIdx::modifyObjectSize(int osize, int nsize, const Oid &koid, 
		       Oid &nkoid)
{
  // ATTENTION:
  // 1. il ne faut utiliser cette fonction qui si koid est un oid physique
  //    test: if (isPhysicalOid(dbh, &koid))
  // 2. il faut creer le nouvel objet sur le meme dataspace !!
  //    => objectRead() permet de recuperer le datid mais objectCreate()
  //       necessite le dspid. Comment fait-on ?
  //       créer une fonction: datGetDspid(dbh, short datid, short *dspid)
  //printf("Modify Object Size from %d to %d\n", osize, nsize);

  unsigned char *data = new unsigned char[osize];
  short datid;
  Status s = objectRead(dbh, 0, osize, data, DefaultLock, &datid,
			      0, &koid);
  if (s) {
    delete [] data;
    return s;
  }

  short dspid;
  s = datGetDspid(dbh, datid, &dspid);

#ifdef OPTIM_LARGE_OBJECTS
  if (!s)
    s = objectCreate(dbh, ObjectNone, nsize, dspid, &nkoid);
#else
  if (!s)
    s = objectCreate(dbh, 0, nsize, dspid, &nkoid);
#endif

  if (s) {
    delete[] data;
    return s;
  }

  s = objectWrite(dbh, 0, osize, data, &nkoid);
  delete [] data;
  if (s) {
    (void)objectDelete(dbh, &nkoid);
    return s;
  }

  s = objectDelete(dbh, &koid);
  if (s)
    (void)objectDelete(dbh, &nkoid);

  return s;
}

Status
HIdx::replaceObjectInList(Hat &hat, int hat_k, Header &h,
			     const Oid &koid, const Oid &nkoid)
{
#ifdef TRACK_MAP
  printf("replaceObjectFromList(%s, %s)\n", getOidString(&koid),
	 getOidString(&nkoid));
  printf("HAT first=%s last=%s\n", getOidString(&hat.first), getOidString(&hat.last));
  printf("HAT2 free_first=%s\n",
	 getOidString(&hat.free_first));
  printf("HEADER prev=%s, next=%s, free_prev=%s, free_next=%s\n",
	 getOidString(&h.prev),
	 getOidString(&h.next),
	 getOidString(&h.free_prev),
	 getOidString(&h.free_next));
#endif  
  Status s;

  Oid xoid;
  h2x_oid(&xoid, &nkoid);

  if (h.prev.getNX()) {
    s = objectWrite(dbh, OFFSET(Header, next), sizeof(Oid),
		       &xoid, &h.prev);
    if (s) return s;
  }
  
  if (h.next.getNX()) {
    s = objectWrite(dbh, OFFSET(Header, prev), sizeof(Oid),
		       &xoid, &h.next);
    if (s) return s;
  }
  
  if (h.free_prev.getNX()) {
    s = objectWrite(dbh, OFFSET(Header, free_next), sizeof(Oid),
		       &xoid, &h.free_prev);
    if (s) return s;
  }
  
  if (h.free_next.getNX()) {
    s = objectWrite(dbh, OFFSET(Header, free_prev), sizeof(Oid),
		       &xoid, &h.free_next);
    if (s) return s;
  }
  
  Boolean write_hat = False;
  if (hat.first.getNX() == koid.getNX()) {
    hat.first = xoid;
    write_hat = True;
  }

  if (hat.last.getNX() == koid.getNX()) {
    hat.last = xoid;
    write_hat = True;
  }

  if (hat.free_first.getNX() == koid.getNX()) {
    hat.free_first = xoid;
    write_hat = True;
  }

  if (write_hat) {
    s = writeHat(hat_k, hat);
    if (s) return s;
  }

  return Success;
}

//#define BUG_DATA_STORE
#define BUG_DATA_STORE2

Status
HIdx::insert_realize(Hat &hat, int hat_k, const void *key,
		     unsigned int size, const void *xdata,
		     const Oid &koid,
		     Header &h, int offset, Overhead &o)
{
  int osize = o.size, onext = o.free_next;
  int ovsize = size + sizeof(Overhead);
  Status s;

#ifdef TRACK_MAP
  printf("insert_realize(%d vs. %d", osize, size);
  if (hidx.datasz >= sizeof(Oid)) {
    Oid xdata_oid;
    memcpy(&xdata_oid, xdata, sizeof(Oid));
    printf(". [%s]", getOidString(&xdata_oid));
  }
  printf("\n");
#endif

#ifdef BUG_DATA_STORE
  if (osize > size + sizeof(Overhead))
    o.size = size;
  else
    ovsize += osize - size;
#endif

  char *data = (char *)malloc(ovsize);
  if (STRTYPE(this))
    memcpy(data + sizeof(Overhead), key, strlen((char *)key)+1);
  else if (hidx.keytype == tUnsignedChar || hidx.keytype == tChar ||
	   hidx.keytype == tSignedChar)
    memcpy(data + sizeof(Overhead), key, size - hidx.datasz);
  else {
    char xkey[Idx_max_type_size];
    assert(size - hidx.datasz <= Idx_max_type_size);
    h2x(xkey, key, keytype);
    memcpy(data + sizeof(Overhead), xkey, size - hidx.datasz);
  }

  if (h.free_first == offset) {
    if (o.free_next != NullOffset) {
      Overhead no;
      s = readOverhead(o.free_next, koid, no);
      if (s) return s;
      no.free_prev = NullOffset;
      s = writeOverhead(o.free_next, koid, no);
      if (s) return s;
    }
    h.free_first = o.free_next;
  }

#ifndef BUG_DATA_STORE
  if (osize > size + sizeof(Overhead))
    o.size = size;
#endif

  o.free = 0;
  o.free_next = NullOffset;
  o.free_prev = NullOffset;
  Overhead to;
  h2x_overhead(&to, &o);
  mcp(data, &to, sizeof(to));
  memcpy(data + ovsize - hidx.datasz, xdata, hidx.datasz);
  //printf("writing OBJECT %s of size %d offset = %d total = %d\n", getOidString(&koid), ovsize, offset, offset+ovsize);
  s = objectWrite(dbh, offset, ovsize, data, &koid);
  free(data);
  if (s) return s;  

  h.free_whole -= osize;
  if (osize == size) {
#ifdef TRACK_MAP
    printf("exact size %d\n", size);
#endif
  }
  else if (osize > size + sizeof(Overhead)) {
    /* generation d'une nouvelle cellule */
#ifdef TRACK_MAP
    printf("split cell %d %d\n", size, osize);
#endif
    int noffset = offset + size + sizeof(Overhead);
    s = insertCell(noffset, osize - size - sizeof(Overhead), h, koid);
    if (s) return s;
  }
  #ifdef TRACK_MAP
  else {
    printf("special case: we are loosing some place %s ?\n", getOidString(&koid));
  }
#endif

  h.free_cnt--;
  h.alloc_cnt++;

  /*
  printf("insert: ovsize=%d, free_whole=%d, free_cnt=%d, alloc_cnt=%d\n",
	 ovsize, h.free_whole, h.free_cnt, h.alloc_cnt);
  */

  // changed this test the 29/06/02 to allow to get rid of unuseful objects
#ifdef OPT_FREELIST
  if (!h.free_cnt || (STRTYPE(this) && h.free_whole <= sizeof(Overhead)+8)) {
#else
  if (!h.free_cnt) {
#endif
#ifdef TRACK_MAP
    printf("making chain for new object\n");
#endif
    Status s;
    // ASTUCE : si la taille n'est pas au maximum, alors ne pas supprimer
    // cet objet de la free list mais, au contraire, l'étendre en utilisant
    // hints[XCoef_Hints].
    if (!uextend || !candidateForExtension(h)) {
#ifdef TRUSS2_GC
      if (h.free_cnt) {
	printf("get rid of this fucking object %d %s\n", h.free_whole, getOidString(&koid));
      }
#endif
      s = suppressObjectFromFreeList(hat, hat_k, h, koid);
      if (s) return s;
    }
  }

  s = writeHeader(koid, h);

  if (!s)
    s = count_manage(dbh, 1);

  return s;
}

Status
HIdx::count_manage(DbHandle *_dbh, int inc)
{
  unsigned int count;
  Status s = objectRead(_dbh, sizeof(unsigned int),
			      sizeof(unsigned int), &count,
			      DefaultLock, 0, 0, &treeoid);
  if (s) return s;
  count = x2h_u32(count);
  unsigned int o_count = hidx.object_count;

  hidx.object_count = count + inc;
  count = h2x_u32(hidx.object_count);

  s = objectWrite(_dbh, sizeof(unsigned int),
		     sizeof(unsigned int), &count, &treeoid);

  if (s) hidx.object_count = o_count;
  return s;
}

Status
HIdx::readHat(int k, Hat &hat) const
{
  Status s;
  unsigned int (*gkey)(int) = get_gkey(version);

  s = objectRead(dbh, gkey(k) * sizeof(Hat), sizeof(Hat), &hat,
		    DefaultLock, 0, 0, &treeoid);
  if (s) return s;
  x2h_hat(&hat);
  return Success;
}

Status
HIdx::readHeader(const Oid &koid, Header &h) const
{
  Status s;
  s = objectRead(dbh, 0, sizeof(Header), &h, DefaultLock,
		    0, 0, &koid);
  if (s) return s;
  x2h_header(&h);
  return Success;
}

Status
HIdx::writeHeader(const Oid &koid, const Header &h) const
{
  Header th;
  h2x_header(&th, &h);
  return objectWrite(dbh, 0, sizeof(Header), &th, &koid);
}

Status
HIdx::readOverhead(int offset, const Oid &koid, Overhead &o) const
{
  Status s;
  s = objectRead(dbh, offset, sizeof(Overhead), &o, DefaultLock,
		    0, 0, &koid);
  if (s) return s;
  x2h_overhead(&o);
  return Success;
}

Status
HIdx::writeOverhead(int offset, const Oid &koid,
		       const Overhead &o) const
{
  Overhead to;
  h2x_overhead(&to, &o);
  return objectWrite(dbh, offset, sizeof(Overhead), &to, &koid);
}

Status
HIdx::readHats(Hat *&hats) const
{
  Status s;
  int len = get_gkey(version)(hidx.key_count);
  hats = new Hat[len];

  s = objectRead(dbh, 0, len * sizeof(Hat), hats,
		    DefaultLock, 0, 0, &treeoid);
  if (s) return s;
  for (int i = KEY_OFF; i < len; i++)
    x2h_hat(&hats[i]);
  return Success;
}

Status
HIdx::writeHats(const Hat *hats) const
{
  int len = get_gkey(version)(hidx.key_count);
  Hat *nhats = new Hat[len];
  memcpy(nhats, hats, sizeof(Hat) * KEY_OFF);
  for (int i = KEY_OFF; i < len; i++)
    h2x_hat(&nhats[i], &hats[i]);

  Status s = objectWrite(dbh, 0, len * sizeof(Hat), nhats,
			       &treeoid);
  delete [] nhats;
  return s;
}

Status
HIdx::writeHat(int k, const Hat &hat) const
{
  Status s;
  unsigned int (*gkey)(int) = get_gkey(version);

  Hat that;
  h2x_hat(&that, &hat);
  s = objectWrite(dbh, gkey(k) * sizeof(Hat), sizeof(Hat), &that,
		     &treeoid);
  if (s) return s;
  return Success;
}

Status
HIdx::dumpMemoryMap(const Hat &hat, const char *msg, FILE *fd)
{
  fprintf(fd, "%sFREE MEMORY MAP {\n", msg);
  Oid prev;
  Oid koid = hat.free_first;
  memset(&prev, 0, sizeof(prev));

  int cnt = 0;
  while (koid.getNX()) {
    Status s;
    Header h;
    s = readHeader(koid, h);
    if (s) return s;
    fprintf(fd, "\tObject %s -> Free Whole: %d, Free Count: %d\n",
	    getOidString(&koid), h.free_whole, h.free_cnt);
    assert(!memcmp(&h.free_prev, &prev, sizeof(prev)));
    prev = koid;
    koid = h.free_next;
    cnt++;
  }
  fprintf(fd, "} -> %d cells in FREE MAP\n\n", cnt);

  cnt = 0;
  memset(&prev, 0, sizeof(prev));
  koid = hat.first;

  fprintf(fd, "%sMEMORY MAP {\n", msg);
  fprintf(fd, "\tFirst Free %s\n", getOidString(&hat.free_first));
  while (koid.getNX()) {
    Status s;
    Header h;
    s = readHeader(koid, h);
    if (s) return s;
    unsigned int sz = 0;
    s = objectSizeGet(dbh, &sz, DefaultLock, &koid);
    if (s) return s;
    int cur = sizeof(Header);
    fprintf(fd, "\tObject %s {\n\t  First Free: %d\n\t  Free Whole: %d\n\t  "
	    "Free Count: %d\n\t  Alloc Count: %d\n\t  Size: %d\n\t  "
	    "Free Prev: %s\n\t  Free Next: %s\n",
	    getOidString(&koid), h.free_first,
	    h.free_whole, h.free_cnt, h.alloc_cnt, sz,
	    getOidString(&h.free_prev), getOidString(&h.free_next));
    assert(!memcmp(&h.prev, &prev, sizeof(prev)));

    int busy_cnt = 0;
    int free_cnt = 0;
    while (cur + sizeof(Overhead) <= sz) {
      Overhead to;
      s = readOverhead(cur, koid, to);
      if (s) return s;
      fprintf(fd, "\t  #%d size %d %s", cur,
	      to.size, (to.free ? "free" : "busy"));

      if (to.free_prev != NullOffset)
	fprintf(fd, " free_prev %d", to.free_prev);

      if (to.free_next != NullOffset)
	fprintf(fd, " free_next %d", to.free_next);

      fprintf(fd, "\n");
      if (to.free) free_cnt++;
      else busy_cnt++;
      cur += to.size + sizeof(Overhead);
    }

    fprintf(fd, "\t}\n");
    // now checking
    assert(free_cnt == h.free_cnt);
    assert(busy_cnt == h.alloc_cnt);
    int free_cur = h.free_first;
    int free_prev = NullOffset;
    int free_size = 0;
    while (free_cur != NullOffset) {
      Overhead to;
      s = readOverhead(free_cur, koid, to);
      if (s) return s;
      if (!to.free || to.free_prev != free_prev) {
	fprintf(fd, "#%d free, free_prev %d %d\n", free_cur, to.free_prev,
		free_prev);
	assert(0);
      }
      assert(to.free);
      assert(to.free_prev == free_prev);
      free_size += to.size;
      free_prev = free_cur;
      free_cur = to.free_next;
    }

    assert(free_size == h.free_whole);
    prev = koid;
    koid = h.next;
    cnt++;
  }
  fprintf(fd, "} -> %d cells in MAP\n", cnt);

  return Success;
}

Status
HIdx::makeObject(Hat &hat, int hat_k, Oid &koid, int &offset,
		    Header &h, Overhead &o, unsigned int objsize)
{
#ifdef TRACK_MAP
  printf("making object %s\n", !hat.first.nx ? "FIRST" : "not FIRST");
#endif
  unsigned int bsz = bsize; // changed the 30/01/02
  objsize += sizeof(Overhead); // added the 30/01/02
  int utsize = (bsz > objsize ? bsz : objsize);
  //int size = sizeof(Header) + sizeof(Overhead) + utsize;
  unsigned int size = sizeof(Header) + utsize;

#ifdef OPTIM_LARGE_OBJECTS
  int alloc_size = sizeof(Header) + sizeof(Overhead);
  char *d = (char *)malloc(alloc_size);
#else
  char *d = (char *)malloc(size);
#endif

  offset = sizeof(Header);
  h.size = size;
  h.free_cnt = 1;
  h.alloc_cnt = 0;
  h.free_whole = utsize - sizeof(Overhead);
  h.free_first = sizeof(Header);
  h.prev = hat.last;
  mset(&h.next, 0, sizeof(h.next));
  mset(&h.free_prev, 0, sizeof(h.free_prev));

  // changed the 20/05/02
  //mset(&h.free_next, 0, sizeof(h.free_next));
  h.free_next = hat.free_first;

  o.free = 1;
  o.size = utsize - sizeof(Overhead); //  + sizeof(Overhead); // 26/12/01: added '+ sizeof(Overhead)'
  o.free_next = NullOffset;
  o.free_prev = NullOffset;
  Header xh;
  h2x_header(&xh, &h);
  mcp(d, &xh, sizeof(Header));
  Overhead xo;
  h2x_overhead(&xo, &o);
  mcp(d + sizeof(Header), &xo, sizeof(Overhead));
  
#ifdef OPTIM_LARGE_OBJECTS
  Status s = objectCreate(dbh, ObjectNone, size, hidx.dspid, &koid);
  if (s) {free (d); return s;}
  s = objectWrite(dbh, 0, alloc_size, d, &koid);
#else
  Status s = objectCreate(dbh, d, size, hidx.dspid, &koid);
#endif
  free(d);

  if (s) return s;

  if (!hat.first.getNX())
    hat.first = koid;
  else {
    Oid xoid;
    h2x_oid(&xoid, &koid);
    s = objectWrite(dbh, OFFSET(Header, next), sizeof(Oid), &xoid,
		       &hat.last);
    if (s) return s;
  }

  hat.last = koid;
  // changed the 20/05/02
  return insertObjectInFreeList(hat, hat_k, h, koid);
}

inline bool
HIdx::inFreeList(const Header &h, const Hat &hat, const Oid &koid)
{
  return h.free_prev.getNX() || h.free_next.getNX() || hat.free_first.getNX() == koid.getNX();
}

Status
HIdx::insertObjectInFreeList(Hat &hat, int hat_k, Header &h,
				const Oid &koid)
{
#ifdef TRACK_MAP
  printf("hidx: insertion of a new cell!\n");
#endif
  Status s;
  if (hat.free_first.getNX()) {
    Oid xoid;
    h2x_oid(&xoid, &koid);
    s = objectWrite(dbh, OFFSET(Header, free_prev),
		       sizeof(Oid), &xoid, &hat.free_first);
    if (s) return s;
  }

  h.free_next = hat.free_first;
  hat.free_first = koid;

  return writeHat(hat_k, hat);
}

Boolean
HIdx::candidateForExtension(const Header &h)
{
  unsigned int size_n = hidx.impl_hints[XCoef_Hints] * h.size;
  return (size_n <= (unsigned int)hidx.impl_hints[SzMax_Hints] ? True : False);
}

Status
HIdx::extendObject(unsigned int size, Hat &hat, int hat_k, Oid &koid,
		      Header &h, int &offset, Overhead &o,
		      Boolean &extended)
{
  unsigned int size_n = hidx.impl_hints[XCoef_Hints] * h.size;
  unsigned int size_inc = size_n - h.size;

  /*
  printf("extendObject %s [%d > %d && %d > %d] ?\n", getOidString(&koid),
	 size_n, hidx.impl_hints[SzMax_Hints],
	 size_inc + h.free_whole, size);
  */
  if (size_n > (unsigned int)hidx.impl_hints[SzMax_Hints] &&
      (!size || size_inc + h.free_whole >= size)) {
    extended = False;
    return Success;
  }
    
  Status s;
  memset(&o, 0, sizeof(Overhead));
  offset = h.free_first;
  int lastoffset = NullOffset;
  while (offset != NullOffset) {
    s = readOverhead(offset, koid, o);
    if (s) return s;
    lastoffset = offset;
    offset = o.free_next;
  }

  Oid nkoid;
  if (o.free) {
#ifdef TRACK_MAP	
    printf("object %s can be extended from %d to %d -> extend overhead\n",
	   getOidString(&koid), h.size, size_n);
#endif
    o.size += size_inc;
    offset = lastoffset;
    s = writeOverhead(offset, koid, o);
    if (s) return s;
    extended = True;
    h.free_whole += size_inc;
    int osize = h.size;
    h.size = size_n;
    s = writeHeader(koid, h);
    if (s) return s;
    if (isPhysicalOid(dbh, &koid)) {
      s = modifyObjectSize(osize, size_n, koid, nkoid);
      if (s) return s;
      s = replaceObjectInList(hat, hat_k, h, koid, nkoid);
      if (s) return s;
      koid = nkoid;
      return Success;
    }
    return objectSizeModify(dbh, size_n, True, &koid);
  }

  if (!size || size_inc >= size) {
    // must done a new overhead AND must chain with last overhead !
#ifdef TRACK_MAP	
    printf("object %s can be extended from %d to %d (diff=%d) -> new overhead\n",
	   getOidString(&koid), h.size, size_n, size_inc - size);
#endif
    if (isPhysicalOid(dbh, &koid)) {
      s = modifyObjectSize(h.size, size_n, koid, nkoid);
      if (s) return s;
      s = replaceObjectInList(hat, hat_k, h, koid, nkoid);
      koid = nkoid;
    }
    else
      s = objectSizeModify(dbh, size_n, True, &koid);
    if (s) return s;
    offset = h.size;
    if (lastoffset != NullOffset) {
      o.free_next = offset;
      s = writeOverhead(lastoffset, koid, o);
      if (s) return s;
    }
    else
      h.free_first = offset;

    o.size = size_inc - sizeof(Overhead);
    o.free = 1;
    o.free_prev = lastoffset;
    o.free_next = NullOffset;
    extended = True;
    h.free_cnt++;
    h.free_whole += size_inc - sizeof(Overhead);
    h.size = size_n;
    if (!inFreeList(h, hat, koid)) {
      //      printf("not in free list ?\n");
      s = insertObjectInFreeList(hat, hat_k, h, koid);
      if (s) return s;
    }
    s = writeHeader(koid, h);
    if (s) return s;
    return writeOverhead(offset, koid, o);
  }
  //printf("not extended\n");
  return Success;
}

#ifndef NO_EXTEND
Status
HIdx::getObjectToExtend(unsigned int size, Hat &hat, int hat_k,
			   Oid &koid, Header &h, int &offset, Overhead &o,
			   Boolean &found)
{
  found = False;
  Status s;
  koid = hat.first;
  while (koid.getNX()) {
    s = readHeader(koid, h);
    if (s) return s;
    /*
    printf("KOID %s h.prev %s h.next %s\n", getOidString(&koid),
	   getOidString(&h.prev),
	   getOidString(&h.next));
    */
    unsigned int size_n = hidx.impl_hints[XCoef_Hints] * h.size;
    unsigned int size_inc = size_n - h.size;
    if (size_n <= hidx.impl_hints[SzMax_Hints] &&
	size_inc + h.free_whole >= size) {
      memset(&o, 0, sizeof(Overhead));
      offset = h.free_first;
      int lastoffset = NullOffset;
      while (offset != NullOffset) {
	s = readOverhead(offset, koid, o);
	if (s) return s;
	lastoffset = offset;
	offset = o.free_next;
      }

      Oid nkoid;
      if (o.free) {
#ifdef TRACK_MAP	
	printf("object %s can be extended from %d to %d -> extend overhead\n",
	       getOidString(&koid), h.size, size_n);
#endif
	o.size += size_inc;
	offset = lastoffset;
	s = writeOverhead(offset, koid, o);
	if (s) return s;
	found = True;
	h.free_whole += size_inc;
	int osize = h.size;
	h.size = size_n;
	s = writeHeader(koid, h);
	if (s) return s;
	if (isPhysicalOid(dbh, &koid)) {
	  s = modifyObjectSize(osize, size_n, koid, nkoid);
	  if (s) return s;
	  s = replaceObjectInList(hat, hat_k, h, koid, nkoid);
	  if (s) return s;
	  koid = nkoid;
	  return Success;
	}
	return objectSizeModify(dbh, size_n, True, &koid);
      }
      else if (size_inc >= size) {
	// must done a new overhead AND must chain with last overhead !
#ifdef TRACK_MAP	
	printf("object %s can be extended from %d to %d -> new overhead\n",
	       getOidString(&koid), h.size, size_n);
#endif
	if (isPhysicalOid(dbh, &koid)) {
	  s = modifyObjectSize(h.size, size_n, koid, nkoid);
	  if (s) return s;
	  s = replaceObjectInList(hat, hat_k, h, koid, nkoid);
	  koid = nkoid;
	}
	else
	  s = objectSizeModify(dbh, size_n, True, &koid);
	if (s) return s;
	offset = h.size;
	if (lastoffset != NullOffset) {
	  o.free_next = offset;
	  s = writeOverhead(lastoffset, koid, o);
	  if (s) return s;
	}
	else
	  h.free_first = offset;

	o.size = size_inc - sizeof(Overhead);
	o.free = 1;
	o.free_prev = lastoffset;
	o.free_next = NullOffset;
	found = True;
	h.free_cnt++;
	h.free_whole += size_inc - sizeof(Overhead);
	h.size = size_n;
	if (!inFreeList(h, hat, koid)) {
	  s = insertObjectInFreeList(hat, hat_k, h, koid);
	  if (s) return s;
	}
	s = writeHeader(koid, h);
	if (s) return s;
	return writeOverhead(offset, koid, o);
      }
    }
    koid = h.next;
  }

#ifdef TRACK_MAP
  printf("no candidates for extension\n");
#endif
  return Success;
}
#endif

int hidx_gccnt;

Status
HIdx::getCell(unsigned int size, Hat &hat, int hat_k,
		 Oid &koid, Header &h, int &offset, Overhead &o)
{
  Status s;
  koid = hat.free_first;
  hidx_gccnt = 0;
#ifdef TRUSS1_GC
  unsigned int total_whole = 0, min_whole = ~0, max_whole = 0;
#endif
#ifdef TRUSS2_GC
  printf("getcell size %d\n", size);
#endif

  while (koid.getNX()) {
    s = readHeader(koid, h);
    if (s) return s;

    Boolean extended;
    if (uextend) {
      if (h.free_whole < size) {
	s = extendObject(size, hat, hat_k, koid, h, offset, o, extended);
	if (s) return s;
      }
    }

#ifdef TRUSS1_GC
    total_whole += h.free_whole;
    if (h.free_whole > max_whole)
      max_whole = h.free_whole;
    if (h.free_whole < min_whole)
      min_whole = h.free_whole;
#endif
#ifdef TRUSS2_GC
    printf("getcell free_whole %d #%d\n", h.free_whole, hidx_gccnt);
#endif
    if (h.free_whole >= size) { // optimisation added 29/01/02
      offset = h.free_first;
      while (offset != NullOffset) {
	s = readOverhead(offset, koid, o);
	if (s) return s;
	if (o.free && o.size >= size)
	  return Success;

	if (uextend) {
	  s = extendObject(size, hat, hat_k, koid, h, offset, o, extended);
	  if (s) return s;
	  if (extended && o.free && o.size >= size)
	    return Success;
	}

	offset = o.free_next;
      }
    }
    koid = h.free_next;
#ifdef TRUSS1_GC
    if (hidx_gccnt == 10000) {
      printf("hidx getcell size=%d, avg_whole=%f, min_whole=%d, "
	     "max_whole=%d\n",
	     size, (float)total_whole/hidx_gccnt, min_whole, max_whole);
      fflush(stdout);
    }
#endif
#ifdef OPT_FREELIST
    if (hidx.keysz != VarSize)
      break;
    // WARNING : 29/06/02 this test is to avoid too many search in free list
    // but this can make holes in the free list !
    if (hidx_gccnt > 4) {
      break;
    }
#endif
    hidx_gccnt++;
  }

#ifndef NO_EXTEND
  Boolean found;
  s = getObjectToExtend(size, hat, hat_k, koid, h, offset, o, found);
  if (s || found) return s;
#endif

  // not found
  return makeObject(hat, hat_k, koid, offset, h, o, size);
}

#ifdef CI_STORE
static char *
lowstring(const char *key)
{
  char *q = new char[strlen(key)+1];
  char c, *p;

  for (p = q; c = *key++; p++) {
    if (c >= 'A' && c <= 'Z')
      *p = c + 'a' - 'A';
    else
      *p = c;
  }

  *p = 0;
  return q;
}
#endif

void stop_imm1() { }

Status
HIdx::insert(const void *key, const void *xdata)
{ 
  Status s;

  if (stat)
    return stat;

  if (s = checkOpened())
    return s;

#ifdef CACHE_FOR_LOCK
  // oplist.insertObjectLast(new INSLink(this, key));
  // return Success;
#endif

  int x;
  unsigned int size;
  unsigned int (*gkey)(int) = get_gkey(version);
  s = get_key(x, key, &size);
  if (s) return s;

  IdxLock lockx(dbh, treeoid);
  s = lockx.lock();
  if (s) return s;

  Hat hat;
  s = readHat(x, hat);
  if (s) return s;

#ifdef TRACK_MAP
  printf("\nINSERT at #%d\n", x);
#endif
  Overhead o;
  Header h;
  Oid koid;
  int offset = 0;
  s = getCell(size, hat, x, koid, h, offset, o);
  if (s) return s;

#ifdef TRACK_MAP
  printf("GETTING CELL offset=%d, koid=%s\n", offset,
	 getOidString(&koid));
#endif

#ifdef TRACK_MAP_2
  (void)dumpMemoryMap(hat, "before inserting ");
#endif
  s = insert_realize(hat, x, key, size, xdata, koid, h, offset, o);
#ifdef TRACK_MAP_2
  (void)dumpMemoryMap(hat, "after inserting ");
#endif
  return s;
}

Status
HIdx::suppressCell(int offset, Header &h, const Oid &koid) const
{
  Status s;
  Overhead o;
  s = readOverhead(offset, koid, o);
  if (s) return s;
#ifdef TRACK_MAP
  printf("suppressing cell at #%d size %d free_first is %d free_prev %d "
	 "free_next %d\n", offset, o.size, h.free_first, o.free_prev,
	 o.free_next);
#endif
  Overhead po, no;
  if (o.free_prev != NullOffset) {
    s = readOverhead(o.free_prev, koid, po);
    if (s) return s;
    po.free_next = o.free_next;
    s = writeOverhead(o.free_prev, koid, po);
    if (s) return s;
  }
  else
    h.free_first = o.free_next;

  if (o.free_next != NullOffset) {
    s = readOverhead(o.free_next, koid, no);
    if (s) return s;
    no.free_prev = o.free_prev;
    s = writeOverhead(o.free_next, koid, no);
    if (s) return s;
  }

#ifdef TRACK_MAP
  printf("now free_first %d\n", h.free_first);
#endif
  h.free_cnt--;
  h.free_whole -= o.size;
  o.free_next = NullOffset;
  o.free_prev = NullOffset;
  o.free = 0;
  return writeOverhead(offset, koid, o);
}

Status
HIdx::insertCell(int offset, unsigned int size, Header &h,
		    const Oid &koid) const
{
#ifdef TRACK_MAP
  printf("inserting cell at #%d size %d [free_first %d]\n", offset, size,
	 h.free_first);
#endif
  Overhead o;
  o.size = size;
  o.free = 1;
  o.free_next = h.free_first;
  o.free_prev = NullOffset;

  if (h.free_first >= 0) {
    Overhead po;
    Status s = readOverhead(h.free_first, koid, po);
    if (s) return s;
#ifdef TRACK_MAP
    printf("making prev link for #%d -> #%d\n", h.free_first, offset);
#endif
    po.free_prev = offset;
    s = writeOverhead(h.free_first, koid, po);
    if (s) return s;
#ifdef TRACK_MAP
    s = readOverhead(h.free_first, koid, po);
    if (s) return s;
    printf("PO.offset = %d\n", po.free_prev);
#endif
  }

  h.free_first = offset;
  h.free_cnt++;
  h.free_whole += o.size;
  return writeOverhead(offset, koid, o);
}

//#define SIMPLE_REMOVE
#ifdef SIMPLE_REMOVE
  h.free_whole += o->size;
  Overhead co;
  co.free = 1;
  co.size = o->size;
  co.next = h.free_first;
  int offset = (eyedblib::int32)(curcell - start);
  h.free_first = offset;
  h.free_cnt++;
  h.alloc_cnt--;
#endif

Status
HIdx::remove_realize(Hat *hat, int hat_k,
			const char *curcell, const char *prevcell,
			const char *start, const Overhead *o,
			const Oid *koid)
{
  Header h;

  mcp(&h, start, sizeof(Header));
  x2h_header(&h);

  Overhead no, po;
  const char *nextcell = curcell + sizeof(Overhead) + o->size;
  if ((eyedblib::int32)(nextcell - start) < h.size) {
    mcp(&no, nextcell, sizeof(Overhead));
    x2h_overhead(&no);
  }
  else
    no.free = 0;

  if (prevcell) {
    mcp(&po, prevcell, sizeof(Overhead));
    x2h_overhead(&po);
  }
  else
    po.free = 0;

#ifdef TRACK_MAP
  printf("co = #%d, no = #%d, po = #%d\n", curcell-start,
	 (curcell-start) + sizeof(Overhead) + o->size,
	 (prevcell ? prevcell-start : 0));
  printf("fo = #%d", h.free_first);
#endif
  Overhead fo;
  if (h.free_first >= 0) {
    mcp(&fo, h.free_first + start, sizeof(Overhead));
    x2h_overhead(&fo);
#ifdef TRACK_MAP
    printf(", fo.free = %d, fo.size = %d, fo.next = %d",
	   fo.free, fo.size, fo.free_next);
#endif
  }

#ifdef TRACK_MAP
  printf("\n");
#endif

  if (no.free && po.free) {
      suppressCell(prevcell-start, h, *koid);
      suppressCell(nextcell-start, h, *koid);
      insertCell(prevcell-start,
		 o->size + po.size + no.size + 2 * sizeof(Overhead),
		 h, *koid);

      //h.free_whole += o->size + 2 * sizeof(Overhead);
#ifdef TRACK_MAP
      printf("case no.free && po.free\n");
#endif
    }
  else if (no.free) {
      suppressCell(nextcell-start, h, *koid);
      insertCell(curcell-start,
		 o->size + no.size + sizeof(Overhead),
		 h, *koid);
      //h.free_whole += o->size + sizeof(Overhead);
#ifdef TRACK_MAP
      printf("case no.free\n");
#endif
    }
  else if (po.free) {
      suppressCell(prevcell-start, h, *koid);
      insertCell(prevcell-start,
		 o->size + po.size + sizeof(Overhead),
		 h, *koid);
      //h.free_whole += o->size + sizeof(Overhead);
#ifdef TRACK_MAP
      printf("case po.free\n");
#endif
    }
  else {
      insertCell(curcell-start, o->size, h, *koid);
      //h.free_whole += o->size;
#ifdef TRACK_MAP
      printf("case *no* free\n");
#endif
    }
  
  h.alloc_cnt--;

  Status s;
  Boolean rmobj = False;
  if (!h.alloc_cnt) {
    s = suppressObjectFromFreeList(*hat, hat_k, h, *koid);
    if (s) return s;
    s = suppressObjectFromList(*hat, hat_k, h, *koid);
    if (s) return s;
    rmobj = True;
  }
  else if (!inFreeList(h, *hat, *koid)) {
    s = insertObjectInFreeList(*hat, hat_k, h, *koid);
    if (s) return s;
  }

  if (!rmobj) {
    s = writeHeader(*koid, h);
    if (s) return s;
  }

#ifdef TRACK_MAP
  Header xh;
  h2x_header(&xh, &h);
  memcpy((void *)start, &xh, sizeof(xh));
#endif

  return count_manage(dbh, -1);
}

#define get_off(X, THIS) \
 ((THIS)->hidx.keysz != HIdx::VarSize ? (THIS)->hidx.keysz : strlen(X) + 1)

Status
HIdx::remove(const void *key, const void *xdata, Boolean *found)
{
  Status s;
  
  if (stat)
    return stat;

  if (s = checkOpened())
    return s;

#ifdef CACHE_FOR_LOCK
  // remove(const void *key, const void *xdata, found?)
  // oplist.insertObjectLast(new RMVLink(this, key, xdata, ?found));
  // return Success;

  // TRES IMPORTANT : il faut changer la stratégie relative au found, par
  // exemple :
  // remove(const void *key, const void *xdata,
  //        Status (*if_not_found)(void *arg) = 0, void *arg = 0)
  // oplist.insertObjectLast(new RMVLink(this, key, xdata, if_not_found, arg));
  // return Success;
  // Mais ce système risque (1) d'être contraignant pour l'utilisateur
  // de cette méthode, (2) de poser un pb de memory leak si arg est un
  // pointeur d'une spécialement allouée pour if_not_found : il faudrait alors
  // une autre fonction releaarg(void *) ou bien :
  // struct IfNotFound {
  //   private : void *arg;
  //   public: Status ifnot();
  //   public: Status release();
  // }; et remove(...., IfNotFound *if_not_found) en dernier argument !
  // autre solution : on supprime l'argument pour found et le client
  // devra faire Idx::search avant s'il veut savoir si la key/data existe
  // ou non
  // à noter que cela est utilisé uniquement dans : attr.cc et idbkern.cc
#endif
  if (found)
    *found = False;

  int x;
  unsigned int (*gkey)(int) = get_gkey(version);
  s = get_key(x, key);
  if (s) return s;

#ifdef TRACK_MAP
  printf("\nREMOVE at #%d\n", x);
#endif
  IdxLock lockx(dbh, treeoid);
  s = lockx.lock();
  if (s) return s;

  Hat hat;
  s = readHat(x, hat);
  if (s) return s;

#ifdef IDX_DBG
  if (STRTYPE(this)) {
      printf("hidx: removing '%s' '%s'\n", key, getOidString((const Oid *)xdata));
    }
#endif
  Oid koid = hat.first;
  while (koid.getNX() > 0) {
    unsigned int size;
      s = objectSizeGet(dbh, &size, DefaultLock, &koid);
      if (s)
	statusPrint(s, "HIdx::remove() treeoid %s", getOidString(&treeoid));

      if (s) return s;

      char *start = (char *)malloc(size);
      s = objectRead(dbh, 0, size, start, DefaultLock, 0, 0, &koid);
      
      if (s) {
	  free(start);
	  return s;
	}
      
      char *curcell, *end = start + size, *prevcell = 0;
      for (curcell = start + sizeof(Header); curcell < end; ) {
	  Overhead o;
	  mcp(&o, curcell, sizeof(Overhead));
	  x2h_overhead(&o);
	  curcell += sizeof(Overhead);
	  
	  if (!o.free && !cmp(key, curcell, OP2_SWAP)) {
#ifdef BUG_DATA_STORE2
	    int r = memcmp(xdata, curcell + get_off(curcell, this),
			   hidx.datasz);
#else
	    int r = memcmp(xdata, curcell + o.size - hidx.datasz, hidx.datasz);
#endif
	    if (!r) {
#ifdef TRACK_MAP
	      Header h;
	      memcpy(&h, start, sizeof(h));
	      x2h_header(&h);
	      dumpMemoryMap(hat, "before remove_realize ");
#endif
	      s = remove_realize(&hat, x, curcell - sizeof(Overhead),
				 prevcell, start, &o, &koid);

#ifdef TRACK_MAP
	      memcpy(&h, start, sizeof(h));
	      x2h_header(&h);
	      dumpMemoryMap(hat, "after remove_realize ");
#endif

	      if (!s && found)
		*found = True;
	      free(start);
	      return s;
	    }
	  }
	  
	  prevcell = curcell - sizeof(Overhead);
	  curcell += o.size;
	}

      Header h;
      mcp(&h, start, sizeof(Header));
      x2h_header(&h);
      //koid = h.free_next;
      koid = h.next;
      free(start);
    }

  return Success;
}

unsigned int
HIdx::getCount() const
{
  return hidx.object_count;
}

short
HIdx::getDefaultDspid() const
{
  return hidx.dspid;

}

void
HIdx::setDefaultDspid(short dspid)
{
  hidx.dspid = dspid;
  dspid = h2x_16(dspid);
  objectWrite(dbh, 4*sizeof(unsigned int), sizeof(short),
		 &dspid, &treeoid);
}

static void
add(Oid *&oids, unsigned int &cnt, unsigned int &alloc_cnt,
    const Oid &oid)
{
  if (cnt >= alloc_cnt) {
    alloc_cnt = cnt + 32;
    oids = (Oid *)realloc(oids, alloc_cnt * sizeof(Oid));
  }

  oids[cnt++] = oid;
}

static const char *
implHintsStr(int hints)
{
  if (hints == HIdx::IniSize_Hints)
    return "Initial Size";

  if (hints == HIdx::IniObjCnt_Hints)
    return "Initial Object Count";

  if (hints == HIdx::XCoef_Hints)
    return "Extends Coeficient";

  if (hints == HIdx::SzMax_Hints)
    return "Maximal Hash Object Size";

  return "<unimplemented>";
}

std::string
HIdx::_Idx::toString() const
{
  std::string s;
  s = std::string("Key Count: ") + str_convert((long)key_count);
  s += std::string("Magnitude Order: ") + str_convert((long)mag_order);
  s += std::string("Object Count: ") + str_convert((long)object_count);
  s += std::string("Dataspace ID: ") + str_convert((long)(dspid == DefaultDspid ? -1 : dspid));
  s += std::string("Key Type: ") + typeString((Idx::Type)keytype);
  s += std::string("Key Size: ") + str_convert((long)keysz);
  s += std::string("Data Size: ") + str_convert((long)datasz);
  s += std::string("Data Offset: ") + str_convert((long)offset);
  s += "Implementation Hint:\n";
  for (int i = 0; i < HIdxImplHintsCount; i++)
    s += std::string("  ") + implHintsStr(i) + ": " + str_convert((long)impl_hints[i]);
  return s;
}

void
HIdx::_Idx::trace(FILE *fd) const
{
  fprintf(fd, "Key Count: %d\n", key_count);
  fprintf(fd, "Magnitude Order: %d\n", mag_order);
  fprintf(fd, "Object Count: %d\n", object_count);
  fprintf(fd, "Dataspace ID: %d\n", (dspid == DefaultDspid ? -1 : dspid));
  fprintf(fd, "Key Type: %s\n", typeString((Idx::Type)keytype));
  fprintf(fd, "Key Size: %d\n", keysz);
  fprintf(fd, "Data Size: %d\n", datasz);
  fprintf(fd, "Data Offset: %d\n", offset);
  fprintf(fd, "Implementation Hint:\n");
  for (int i = 0; i < HIdxImplHintsCount; i++)
    fprintf(fd, "  %s: %d\n", implHintsStr(i), impl_hints[i]);
}

Status
HIdx::getObjects(Oid *&oids, unsigned int &cnt) const
{
  unsigned int alloc_cnt = 0;
  cnt = 0;
  oids = 0;
  unsigned int (*gkey)(int) = get_gkey(version);

  for (int i = 0; i < hidx.key_count; i++) {
    HIdx::Hat hat;
    Status s = readHat(i, hat);
    if (s) return s;

    Oid koid = hat.first;
    while (koid.getNX()) {
      add(oids, cnt, alloc_cnt, koid);
      HIdx::Header h;
      s = objectRead(dbh, 0, sizeof(HIdx::Header), &h,
			DefaultLock, 0, 0, &koid);
      if (s) return s;
      x2h_header(&h);
      koid = h.next;
    }
  }

  return Success;
}

int
HIdx::cmp(const void *key, const void *d, unsigned char bswap) const
{
  return compare(key, d, &keytype, bswap);
}

Status HIdx::search(const void *key, Boolean *found, void *xdata)
{
  Status s;
  
  if (stat)
    return stat;

  if (s = checkOpened())
    return s;

  int x;

  *found = False;

  unsigned int (*gkey)(int) = get_gkey(version);
  s = get_key(x, key);
  if (s) return s;

  Hat hat;
  s = readHat(x, hat);
  if (s) return s;

  Oid koid = hat.first;
  while (koid.getNX() > 0) {
    if (backend_interrupt)
      return statusMake(BACKEND_INTERRUPTED, "");
    unsigned int size;
    Status s = objectSizeGet(dbh, &size, DefaultLock, &koid);
    if (s) return s;
    char *data = (char *)malloc(size);
    s = objectRead(dbh, 0, size, data, DefaultLock, 0, 0, &koid);
      
    if (s) {
      free(data);
      return s;
    }
      
    char *d, *edata = data + size;
    for (d = data + sizeof(Header); d < edata; ) {
      Overhead o;
      mcp(&o, d, sizeof(Overhead));
      x2h_overhead(&o);
      d += sizeof(Overhead);

      if (!o.free && !cmp(key, d, OP2_SWAP)) {
	*found = True;
	if (xdata) {
#ifdef BUG_DATA_STORE2
	  memcpy(xdata, d + get_off(d, this), hidx.datasz);
#else
	  memcpy(xdata, d + o.size - hidx.datasz, hidx.datasz);
#endif
	}
	break;
      }
	  
      d += o.size;
    }
      
    Header h;
    mcp(&h, data, sizeof(Header));
    x2h_header(&h);
    //koid = h.free_next;
    koid = h.next;
    free(data);
  }

  return Success;
}

Status HIdx::destroy()
{
  Status s = destroy_r();
  if (s) return s;
  return objectDelete(dbh, &treeoid);
}

Status HIdx::destroy_r()
{
  for (int n = 0; n < hidx.key_count; n++) {
    Hat hat;
    Status s = readHat(n, hat);
    if (s) return s;

    Oid koid = hat.first;

    while (koid.getNX()) {
      Header h;
      Status s = readHeader(koid, h);
      if (s) return s;

      s = objectDelete(dbh, &koid);
      if (s) return s;

      koid = h.next;
    }
  }

  return Success;
}

Status
HIdx::headPrint(FILE *fd, int n, Oid *koid, int &count) const
{
  Header h;
  Status s = objectRead(dbh, 0, sizeof(Header), &h, DefaultLock, 0,
			      0, koid);

  if (s) return s;
  x2h_header(&h);

  count = h.alloc_cnt;
  fprintf(fd, "\tsubcell[%d] %s {\n", n, getOidString(koid));
  fprintf(fd, "\t\tsize       = %d;\n", h.size);
  fprintf(fd, "\t\tnfree      = %d;\n", h.free_cnt);
  fprintf(fd, "\t\tnalloc     = %d;\n", h.alloc_cnt);
  fprintf(fd, "\t\tfree_whole = %d;\n", h.free_whole);
  fprintf(fd, "\t\tfirstfree  = %d;\n", h.free_first);
  fprintf(fd, "\t\tprev       = %s;\n", getOidString(&h.prev));
  fprintf(fd, "\t\tnext       = %s;\n", getOidString(&h.next));
  fprintf(fd, "\t\tfree_prev  = %s;\n", getOidString(&h.free_prev));
  fprintf(fd, "\t\tfree_next  = %s;\n", getOidString(&h.free_next));
  fprintf(fd, "\t};\n");
  *koid = h.next;

  return Success;
}

Status
HIdx::getHashObjectBusySize(const Oid *koid, unsigned int &osize, unsigned int size) const
{
  if (!STRTYPE(this)) {
    Header h;
    Status s = objectRead(dbh, 0, sizeof(Header), &h, DefaultLock, 0,
				0, koid);
    
    if (s) return s;
    x2h_header(&h);
  
    osize = h.alloc_cnt * (sizeof(Overhead) + hidx.keysz) + sizeof(Header);
    return Success;
  }

  Status s;
  if (!size) {
    s = objectSizeGet(dbh, &size, DefaultLock, koid);
    if (s) return s;
  }

  osize = sizeof(Header);

  char *data;
  if (nocopy) {
    s = objectReadNoCopy(dbh, 0, size, &data, DefaultLock, 0, 0, koid);
  }
  else {
    data = (char *)malloc(size);
    s = objectRead(dbh, 0, size, data, DefaultLock, 0, 0, koid);
  }

  if (s) {if (!nocopy) free(data); return s;}

  int cur = sizeof(Header);
  while (cur + sizeof(Overhead) <= size) {
    Overhead to;
    s = readOverhead(cur, *koid, to);
    if (s) {if (!nocopy) free(data); return s;}
    cur += sizeof(Overhead);
    osize += sizeof(Overhead) + strlen(data+cur)+1;
    cur += to.size;
  }

  if (!nocopy)
    free(data);
  return Success;
}

Status
HIdx::getEntryCount(Oid *koid, int &count) const
{
  if (koid->getNX() == 0) {
    count = 0;
    return Success;
  }
  Header h;
  Status s = objectRead(dbh, 0, sizeof(Header), &h, DefaultLock, 0,
			      0, koid);

  if (s) return s;
  x2h_header(&h);

  count = h.alloc_cnt;
  *koid = h.next;

  return Success;
}

//#define ALL_STATS

Status
HIdx::dumpMemoryMap(FILE *fd)
{
  for (int n = 0; n < hidx.key_count; n++) {
    Hat hat;
    Status s = readHat(n, hat);
    if (s) return s;
    Oid koid = hat.first;
    if (!koid.getNX()) continue;
    if (s) return s;
    s = dumpMemoryMap(hat, (std::string("Entry #") + str_convert((long)n) + " ").c_str(),
		      fd);
    if (s) return s;
  }

  return Success;
}

Status HIdx::printStat(FILE *fd) const
{
  //const Oid *koid;
  int n, total;

  if (!fd)
    fd = stdout;

#ifdef ALL_STATS
  fprintf(fd, "index bsize = 0x%x\n", hidx.impl_hints[IniSize_Hints]);
#endif
  fprintf(fd, "\tkey_count = %d\n", hidx.key_count);
  fflush(fd);

  total = 0;

  for (int n = 0; n < hidx.key_count; n++) {
    Hat hat;
    Status s = readHat(n, hat);
    if (s) return s;

#ifdef ALL_STATS
    fprintf(fd, "cell[%d] = {\n", n);
    fprintf(fd, "\tfirst      = %s;\n", getOidString(&hat.first));
    fprintf(fd, "\tlast       = %s;\n", getOidString(&hat.last));
    fprintf(fd, "\tfree_first = %s;\n", getOidString(&hat.free_first));
#endif

    Oid toid;
    toid = hat.first;
    int cell_count = 0;

    while (toid.getNX() > 0) {
      int count;
      if (backend_interrupt) {
	/*
	fprintf(fd, "Interrupted!\n");
	fprintf(fd, "\tpartial total count = %d\n", total);
	fflush(fd);
	*/
	return statusMake(BACKEND_INTERRUPTED, "");
      }

      s = getEntryCount(&toid, count);
      if (s) {
	statusPrint(s, "");
	fflush(fd);
	return s;
      }
      cell_count += count;
    }
#ifdef ALL_STATS
#ifndef PRINT_DETAILS
    fprintf(fd, "\tcount      = %d;\n", cell_count);
#endif
#else
    if (cell_count) {
      fprintf(fd, "\tcell[%d] -> %d\n", n, cell_count);
      fflush(fd);
    }
#endif
    total += cell_count;
#ifdef ALL_STATS
    fprintf(fd, "};\n\n");
#endif
  }

  fprintf(fd, "\ttotal count = %d [%d]\n", total, getCount());
  fflush(fd);
  return Success;
}

#define ONE_LIST

struct Link {

  Link(unsigned int sz) {
    data = new unsigned char[sz];
    next = 0;
  }

  ~Link() {
    delete [] data;
  }

  Idx::Key key;
  unsigned char *data;
  Link *next;

private:
  // forbidden
  Link(const Link &);
  Link &operator=(const Link &);
};

class HIdxCursor::LinkList {

public:
  LinkList() {first = last = 0; cnt = 0;}

  void insert(Link *l) {
    eyedblib::MutexLocker _(mt);
    if (last)
      last->next = l;
    if (!first)
      first = l;
    last = l;
    cnt++;
  }

  Link *peek() {
    eyedblib::MutexLocker _(mt);
    if (first) {
      Link *l = first;
      first = first->next;
      if (!first) last = 0;
      cnt--;
      return l;
    }
    return 0;
  }

  int getCount() const {
    eyedblib::MutexLocker _(mt);
    return cnt;
  }

  ~LinkList() {
    eyedblib::MutexLocker _(mt);
    Link *l = first;
    while (l) {
      Link *next = l->next;
      delete l;
      l = next;
    }
  }

private:
  mutable eyedblib::Mutex mt;
  Link *first, *last;
  int cnt;
};

extern eyedblib::ThreadPool *getThreadPool();

#define MIN_OBJCNT_FOR_PARALLEL 10
//#define MIN_KEYCNT_FOR_PARALLEL 10

static eyedblib::ThreadPerformerArg
cursor_perform_wrapper(eyedblib::ThreadPerformerArg xarg)
{
  HIdxCursor *cur = (HIdxCursor *)xarg.data;
  for (;;) {
    Boolean found;
    Status s = cur->next(&found);
    if (s)
      return eyedblib::ThreadPerformerArg((void *)s);
    if (!found)
      return 0;
  }
  return 0;
}

static int user_cmp_mod;

static Boolean 
user_cmp(const void *, void *)
{
  static eyedblib::Mutex mt;
  //  eyedblib::MutexLocker _(mt);
  static int cnt;
  for (int n = 0; n < 50; n++) ;
  if (!(cnt++ % user_cmp_mod)) return True;
  return False;
}

Boolean
HIdxCursor::parallelInit(int thread_cnt)
{
  /*
  if ((skey && ekey) || !user_cmp ||
      idx->hidx.object_count <= MIN_OBJCNT_FOR_PARALLEL)
    return False;
  */

  if (thread_cnt < 2 || idx->hidx.object_count <= MIN_OBJCNT_FOR_PARALLEL ||
      (skey && ekey && !idx->cmp(skey, ekey, OPS_NOSWAP))) /*(skey && ekey) || */ // WARNING : 24/02/03 !!
    return False;

  thrpool = getThreadPool();
  if (!thrpool)
    return False;

  perf_cnt = thrpool->getThreadCount();

  if (perf_cnt > thread_cnt)
    perf_cnt = thread_cnt;

  if (perf_cnt < 1)
    return False;

  unsigned int itv = idx->hidx.key_count / perf_cnt;

  if (itv < 1)
    return False;

  /*
  printf("Hash Cursor %s candidate for parallelization: perf_cnt=%d, itv=%d, "
	 "max_threads=%d\n",
	 getOidString(&idx->treeoid),
	 perf_cnt, itv,  thrpool->getThreadCount());
  */

  master = True;
#ifdef MULTI_LIST
  lists = new LinkList*[perf_cnt];
  list = 0;
#else
  list = new LinkList();
  lists = 0;
#endif
  perf_curs = new HIdxCursor*[perf_cnt];

  /*
  const char *s = getenv("ESM_STD_USER_CMP");
  if (s) {
    user_cmp = user_cmp;
    user_cmp_mod = atoi(s);
  }
  */

  for (int n = 0; n < perf_cnt; n++) {
#ifdef MULTI_LIST
    lists[n] = new LinkList();
    perf_curs[n] = new HIdxCursor
      (idx,
       n*itv,
       (n == perf_cnt-1 ? idx->hidx.key_count : (n+1)*itv),
       skey, ekey,
       sexcl, eexcl,
       user_cmp, cmp_arg,
       lists[n]);
#else
    perf_curs[n] = new HIdxCursor
      (idx,
       n*itv,
       (n == perf_cnt-1 ? idx->hidx.key_count : (n+1)*itv),
       skey, ekey,
       sexcl, eexcl,
       user_cmp, cmp_arg,
       list);
#endif
  }

  /*
  s = getenv("ESM_MAX_PERF_CNT");
  if (s) {
    unsigned int cnt = atoi(getenv("ESM_MAX_PERF_CNT"));
    if (perf_cnt > cnt) {
      perf_cnt = cnt;
      printf("limited perf_cnt to %d\n", perf_cnt);
    }
  }
  */

  thrpool->reset();
  perfs = new eyedblib::ThreadPerformer*[perf_cnt];

  for (int n = 0; n < perf_cnt; n++) {
    perfs[n] = thrpool->start(cursor_perform_wrapper, perf_curs[n]);
  }

#ifdef NEW_WAIT
  perf_end_cnt = 0;
#endif
  return True;
}

void
HIdxCursor::init(DbHandle *dbh)
{
  state = True;
  master = False;
  slave = False;

  sdata = 0;
  memset(&koid, 0, sizeof(koid));
  skey = ekey = 0;
  sdata = 0;

  perf_cnt = 0;
  perf_curs = 0;
  perfs = 0;
#ifdef FORCE_COPY
  nocopy = False;
#else
  nocopy = isWholeMapped(dbh);
#endif
  data_tofree = False;
}

HIdxCursor::HIdxCursor(const HIdx *_idx,
			     unsigned int _k_cur,
			     unsigned int _k_end,
			     const void *_skey, const void *_ekey,
			     Boolean _sexcl, Boolean _eexcl,
			     Boolean (*_user_cmp)(const void *, void *),
			     void *_cmp_arg,
			     LinkList *_list) :
  idx(_idx),
  user_cmp(_user_cmp), cmp_arg(_cmp_arg),
  k_cur(_k_cur), k_end(_k_end), list(_list),
  sexcl(_sexcl), eexcl(_eexcl)
{
  init(idx->dbh);
  slave = True;
  equal = False;
  Boolean isstr = (STRTYPE(idx) ? True : False);
  skey = copy_key(_skey, idx->hidx.keysz, isstr);
  ekey = copy_key((_ekey == defaultSKey) ? _skey : _ekey, idx->hidx.keysz, isstr);
  //printf("k_cur %d -> %d : %d\n", k_cur, k_end, idx->hidx.key_count);
}

HIdxCursor::HIdxCursor(const HIdx *_idx,
			     const void *_skey, const void *_ekey,
			     Boolean _sexcl, Boolean _eexcl,
			     Boolean (*_user_cmp)(const void *, void *),
			     void *_cmp_arg,
			     int thread_cnt) :
  idx(_idx), sexcl(_sexcl),
  eexcl(_eexcl),
  user_cmp(_user_cmp),
  cmp_arg(_cmp_arg)
{
  assert(!idx->status());
  assert(idx->isOpened());

  init(idx->dbh);
  list = 0;
  lists = 0;

  Status s;
  Boolean isstr = (STRTYPE(idx) ? True : False);
  skey = copy_key(_skey, idx->hidx.keysz, isstr);
  ekey = copy_key((_ekey == defaultSKey) ? _skey : _ekey, idx->hidx.keysz, isstr);

  if (!state)
    return;

  if (parallelInit(thread_cnt))
    return;

  if (skey && ekey && !idx->cmp(skey, ekey, OPS_NOSWAP)) {
    equal = True;
    s = idx->get_key(k_cur, skey);
    if (s) state = False;
  }
  else {
    k_cur = 0;
    equal = False;
  }
    
  k_end = idx->hidx.key_count;
}

#define NO_COPY

Status
HIdxCursor::read(Boolean &eox)
{
  unsigned int size = 0;
  Status s;
  int n;

  HIdx::Header h;

  unsigned int (*gkey)(int) = get_gkey(idx->version);
  //  printf("enter HIdxCursor::read()\n");
  for (n = 0; ; n++) {
    /*
    printf("@%d koid = %s k_cur=%d, k_end=%d\n", pthread_self(),
	   getOidString(&koid), k_cur, k_end);
    */
    if (backend_interrupt)
      return statusMake(BACKEND_INTERRUPTED, "");

    if (koid.getNX() == 0) {
      if (k_cur >= k_end) {
	eox = True;
	return Success;
      }

      HIdx::Hat hat;
      s = idx->readHat(k_cur, hat);
      if (s) return s;
      koid = hat.first;

      if (equal) {
	if (koid.getNX() == 0) {
	  eox = True;
	  return Success;
	}
      }
      else
	k_cur++;
      
      if (koid.getNX() == 0)
	continue;
    }	  

    s = objectRead(idx->dbh, 0, sizeof(HIdx::Header), &h,
		   DefaultLock, 0, &size, &koid);
    if (s) return s;
    x2h_header(&h);
    
    if (h.alloc_cnt)
      break;

    koid = h.next;
    
    if (equal && koid.getNX() == 0) {
      eox = True;
      return Success;
    }
  }      

  eox = False;

  Boolean nocopy_failed = False;

  if (data_tofree)
    free(sdata);

  if (nocopy) {
    s =  objectReadNoCopy(idx->dbh, 0, size, &sdata, DefaultLock, 0, 0,
			     &koid);
    if (!s) {
      edata = sdata + size;
      cur = sdata + sizeof(HIdx::Header);
      data_tofree = False;
    }
    else {
      //printf("nocopy failed for %s\n", getOidString(&koid));
      nocopy_failed = True;
    }
  }

  if (!nocopy || nocopy_failed) {
    sdata = (char *)malloc(size);
    data_tofree = True;
    edata = sdata + size;
    cur = sdata + sizeof(HIdx::Header);
    
    s =  objectRead(idx->dbh, 0, size, sdata, DefaultLock, 0, 0, &koid);
  }

  koid = h.next;

  return s;
}

void *
HIdxCursor::copy_key(const void *key, unsigned int keysz, Boolean isstr)
{
  if (!key)
    return 0;

  if (keysz == HIdx::VarSize)
    return strdup((char *)key);

  char *k = (char *)malloc(keysz);
  assert((long)k > 0);

  if (isstr) {
    int len = strlen((char *)key)+1;

    if (len > keysz) // was if (len >= keysz)
      state = False;
    else {
      memcpy(k, key, len);
      memset(k+len, 0, keysz-len);
    }
  }
  else
    memcpy(k, key, keysz);

  return k;
}

inline int
HIdxCursor::cmp_realize(const void *xkey, const void *ykey,
			   Boolean excl, unsigned char bswap) const
{
  int c = idx->cmp(xkey, ykey, bswap);

  if (c == 0) {
    if (excl)
      return 1;
    return 0;
  }
  
  return (c > 0);
}

inline int
HIdxCursor::cmp(const void *key) const
{
  if (equal)
    return idx->cmp(skey, key, OP2_SWAP);

  if (!skey && !ekey)
    return 0;

  if (skey && ekey)
    return cmp_realize(skey, key, sexcl, OP2_SWAP) ||
      cmp_realize(key, ekey, eexcl, OP1_SWAP);

  if (skey)
    return cmp_realize(skey, key, sexcl, OP2_SWAP);

  if (ekey)
    return cmp_realize(key, ekey, eexcl, OP1_SWAP);

  abort();
  return 1;
}

Status
HIdxCursor::next(Boolean *found, void *data, Idx::Key *key)
{
  if (!state) {
    *found = False;
    return Success;
  }

  if (master) {
    for (;;) {
      LinkList *tlist;
#ifdef MULTI_LIST
      for (int n = 0; n < perf_cnt; n++) {
	tlist = lists[n];
#else
	tlist = list;
#endif
	Link *l = tlist->peek();
	if (l) {
	  if (data)
	    memcpy(data, l->data, idx->hidx.datasz);

	  if (key)
	    key->setKey(l->key.getKey(), l->key.getSize(), idx->keytype);

	  delete l;
	  *found = True;
	  return Success;
	}
#ifdef MULTI_LIST
      }
#endif

#ifdef NEW_WAIT
      if (perf_end_cnt == perf_cnt) {
	thrpool->waitAll();
	*found = False;
	return Success;
      }

      eyedblib::ThreadPerformer *perf;
      eyedblib::ThreadPerformerArg arg = thrpool->wait(perf);
      if (arg.data) {
	thrpool->waitAll(); // should advert thread to stop immediately !
	return (Status)arg.data;
      }

      perf_end_cnt++;
#else
      int n;
      for (n = 0; n < perf_cnt; n++) {
	if (!perfs[n]->isIdle())
	  break;
      }

      bool cond = (n == perf_cnt);
      if (!cond) { // at least one is still running
	eyedblib::ThreadPerformer *perf;
	eyedblib::ThreadPerformerArg arg = thrpool->wait(perf);
	if (arg.data) {
	  thrpool->waitAll(); // should advert thread to stop immediately !
	  return (Status)arg.data;
	}
      }
      else if (cond) { // all performers have finished
	thrpool->waitAll();
	*found = False;
	return Success;
      }
#endif
    }
  }

  if (!sdata) {
    Boolean eox;
    Status s = read(eox);
    if (s) return s;
    
    if (eox) {
      *found = False;
      return Success;
    }
  }

  for (;;) {
    // changed the 8/12/99
    // for (; cur < edata; )
    for (; cur+sizeof(HIdx::Overhead) <= edata; ) {
      if (backend_interrupt)
	return statusMake(BACKEND_INTERRUPTED, "");
      HIdx::Overhead o;
      mcp(&o, cur, sizeof(HIdx::Overhead));
      x2h_overhead(&o);
      cur += sizeof(HIdx::Overhead);
	  
      if (!o.free && !cmp(cur)) {
	if (idx->precmp) {
	  int r;
	  void const * sk = skey ? skey : ekey;
	  if (sk && idx->precmp(sk, cur, &idx->keytype, r)) {
	    //printf("skipping entry\n");
	    if (!skey) {
	      cur += o.size; // 23/12/01: was not test with this statement
	      continue;
	    }
	    *found = False;
	    return Success;
	  }
	}
	
	if (user_cmp && !user_cmp(cur, cmp_arg)) {
	  cur += o.size;
	  continue;
	}

	*found = True;
	int off = get_off(cur, idx);

	Link *l;
	if (slave) {
	  l = new Link(idx->hidx.datasz);
	  data = l->data;
	  key = &l->key;
	}
	else
	  l = 0;

	if (data) {
#ifdef BUG_DATA_STORE2
	  memcpy(data, cur + off, idx->hidx.datasz);
#else
	  memcpy(data, cur + o.size - idx->hidx.datasz, idx->hidx.datasz);
#endif
	}
	
	if (key)
	  key->setKey(cur, (idx->hidx.keysz != HIdx::VarSize ?
			    idx->hidx.keysz : strlen(cur) + 1),
		      idx->keytype);
	
	if (slave)
	  list->insert(l);

	cur += o.size;
	return Success;
      }
	  
      cur += o.size;
    }
      
    if (equal && !koid.getNX()) {
      *found = False;
      state = False;
      return Success;
    }

    Boolean eox;
    Status s = read(eox);
	  
    if (s) return s;

    if (eox) {
      *found = False;
      state = False;
      return Success;
    }
  }
}

HIdxCursor::~HIdxCursor()
{
  free(skey);
  free(ekey);
  if (data_tofree)
    free(sdata);
  delete [] perf_curs;
  delete [] perfs;
#ifdef MULTI_LIST
  if (lists) {
    for (int n = 0; n < perf_cnt; n++)
      delete lists[n];
    delete lists;
  }
#else
  delete list;
#endif
}

Status
HIdx::getStats(std::string &stats) const
{
  unsigned int total;
  int n;

  stats  = std::string("  Index type: 'hash'\n");

  stats += std::string("  Key count: ") + str_convert((long)hidx.key_count) + "\n";
  stats += std::string("  Magnitude Order: ") + str_convert((int)hidx.mag_order) + "\n";
  stats += std::string("  Key Type: ") + typeString((Type)hidx.keytype) + "\n";
  stats += std::string("  Implementation Hints:\n");
  for (int i = 0; i < HIdxImplHintsCount; i++)
    stats += std::string("    ") + implHintsStr(i) + ": " +
      str_convert((int)hidx.impl_hints[i]) + "\n";
  stats += std::string("  Dataspace ID: ") + str_convert((int)(hidx.dspid == DefaultDspid ? -1 : hidx.dspid)) + "\n";
  stats += std::string("  Data Size: ") + str_convert((long)hidx.datasz) + "\n";
  stats += std::string("  Key Size: ") + str_convert((int)hidx.keysz) + "\n";
  stats += std::string("  Magnitude Order: ") + str_convert((long)hidx.mag_order) + "\n";

  total = 0;
  unsigned int total_objs = 0;
  unsigned int max = 0;
  unsigned int min = ~0;
  unsigned int free = 0, busy = 0;

  for (n = 0; n < hidx.key_count; n++) {
    Hat hat;
    Status s = readHat(n, hat);
    if (s) return s;

    Oid toid;
    toid = hat.first;
    unsigned int cell_count = 0;
    unsigned int nobjs = 0;
    while (toid.getNX() > 0) {
      int count;
      if (backend_interrupt)
	return statusMake(BACKEND_INTERRUPTED, "");

      s = getEntryCount(&toid, count);
      if (s) return s;

      cell_count += count;
      nobjs++;
    }

    total_objs += nobjs;

    if (cell_count > max)
      max = cell_count;
    if (cell_count < min)
      min = cell_count;

    if (cell_count) {
      busy++;
      stats += std::string("  Cell #") + str_convert(n) + ": " + str_convert((long)cell_count) + " objects, " + str_convert((long)nobjs) + " hash object" + (nobjs != 1 ? "s" : "") + "\n";
      total += cell_count;
    }
    else
      free++;
      
  }

  stats += std::string("  Total object count: ") + str_convert((long)total) +
    " {computed: " + str_convert((long)getCount()) + "}\n";
  stats += std::string("  Min object per entry: ") + str_convert((long)min) + "\n";
  stats += std::string("  Max entries per entry: ") + str_convert((long)max) + "\n";
  stats += std::string("  Free entry count: ") + str_convert((long)free) + "\n";
  stats += std::string("  Busy entry count: ") + str_convert((long)busy) + "\n";
  stats += std::string("  Total hash object count: 1+") +
    str_convert((long)total_objs) + "\n";
  return Success;
}

Status
HIdx::getStats(HIdx::Stats &stats) const
{
  memset(&stats, 0, sizeof(stats));
  memcpy(&stats.idx, &hidx, sizeof(hidx));

  unsigned int (*gkey)(int) = get_gkey(version);
  // the tree object:
  stats.total_hash_object_count = 1;
  stats.total_hash_object_size = sizeof(Hat) * gkey(hidx.key_count);

  stats.entries = new Stats::Entry[hidx.key_count];
  memset(stats.entries, 0, sizeof(Stats::Entry) * hidx.key_count);
  stats.min_objects_per_entry = ~0;

  Stats::Entry *entry = stats.entries;
  for (int n = 0; n < hidx.key_count; n++, entry++) {
    Hat hat;
    Status s = readHat(n, hat);
    if (s) return s;

    Oid koid;
    koid = hat.first;
    while (koid.getNX() > 0) {
      int count;
      if (backend_interrupt)
	return statusMake(BACKEND_INTERRUPTED, "");
      
      unsigned int size, busysize;
      s = objectSizeGet(dbh, &size, DefaultLock, &koid);
      if (s) return s;

      s = getHashObjectBusySize(&koid, busysize, size);
      if (s) return s;

      s = getEntryCount(&koid, count);
      if (s) return s;
      
      entry->object_count += count;
      entry->hash_object_busy_size += busysize;
      entry->hash_object_count++;
      entry->hash_object_size += size;
    }

    if (entry->object_count > stats.max_objects_per_entry)
      stats.max_objects_per_entry = entry->object_count;
    if (entry->object_count < stats.min_objects_per_entry)
      stats.min_objects_per_entry = entry->object_count;

    stats.total_hash_object_count += entry->hash_object_count;
    stats.total_hash_object_busy_size += entry->hash_object_busy_size;
    stats.total_object_count += entry->object_count;
    stats.total_hash_object_size += entry->hash_object_size;

    if (entry->object_count)
      stats.busy_key_count++;
    else
      stats.free_key_count++;
  }

  return Success;
}

HIdx::Stats::Stats()
{
  entries = 0;
}

std::string
HIdx::Stats::toString(Boolean full) const
{
  std::string s = idx.toString();
  Entry *entry = entries;

  if (full) {
    for (int i = 0; i < idx.key_count; i++, entry++)
      if (entry->object_count) {
	s += std::string("Entry #") + str_convert((long)i) + " {\n";
	s += std::string("\tObject count: ") + str_convert((long)entry->object_count) +
	  "\n";
	s += std::string("\tObject size: ")  + 
	  str_convert((long)entry->hash_object_busy_size) + "\n";
	s += std::string("\tHash object count: ") +
	  str_convert((long)entry->hash_object_count) + "\n";
	s += std::string("\tHash object size: ")  + 
	  str_convert((long)entry->hash_object_size) + "b\n}\n";
      }

    s += "\n";
  }

  s += std::string("Min objects per entry: ") +
    str_convert((long)min_objects_per_entry) + "\n";
  s += std::string("Max objects per entry: ") +
    str_convert((long)max_objects_per_entry) + "\n";
  s += std::string("Total object count: ") +
    str_convert((long)total_object_count) + "\n";
  s += std::string("Total object size: ") +
    str_convert((long)total_hash_object_busy_size) + "b\n";
  s += std::string("Total hash object count: ") + 
    str_convert((long)total_hash_object_count) + "\n";
  s += std::string("Total hash object size: ") +
    str_convert((long)total_hash_object_size) + "b\n";
  s += std::string("Busy entry count: ") +
    str_convert((long)busy_key_count) + "\n";
  s += std::string("Free entry count: ") +
    str_convert((long)free_key_count) + "\n";
  return s;
}

void
HIdx::Stats::trace(Boolean full, FILE *fd) const
{
  idx.trace(fd);
  Entry *entry = entries;

  if (full) {
    for (int i = 0; i < idx.key_count; i++, entry++)
      if (entry->object_count) {
	fprintf(fd, "Entry #%d {\n", i);
	fprintf(fd, "\tObject count: %d\n", entry->object_count);
	fprintf(fd, "\tHash object count: %d\n", entry->hash_object_count);
	fprintf(fd, "\tHash object size: %db\n", entry->hash_object_size);
	fprintf(fd, "\tHash object busy size: %db\n", entry->hash_object_busy_size);
	fprintf(fd, "}\n");
      }
    fprintf(fd, "\n");
  }

  fprintf(fd, "Min objects per entry: %d\n", min_objects_per_entry);
  fprintf(fd, "Max objects per entry: %d\n", max_objects_per_entry);
  fprintf(fd, "Total object count: %d\n", total_object_count);
  fprintf(fd, "Total hash object count: %d\n", total_hash_object_count);
  fprintf(fd, "Total hash object size: %db\n", total_hash_object_size);
  fprintf(fd, "Total hash object busy size: %db\n", total_hash_object_busy_size);
  fprintf(fd, "Busy entry count: %d\n", busy_key_count);
  fprintf(fd, "Free entry count: %d\n", free_key_count);
}

HIdx::Stats::~Stats()
{
  delete [] entries;
}

Status
HIdx::copy(HIdx *&idx_n, int key_count, int mag_order, short dspid,
	      const int *impl_hints, unsigned int impl_hints_cnt,
	      hash_key_t _hash_key,
	      void *_hash_data, KeyType *ktype) const
{
  key_count = (key_count > MaxKeys ? MaxKeys : key_count);

  idx_n = new HIdx(dbh, (ktype ? *ktype : getKeyType()), hidx.datasz,
		      (dspid == DefaultDspid ? hidx.dspid : dspid),
		      (mag_order == 0 ? hidx.mag_order : mag_order),
		      key_count,
		      (impl_hints == 0 ? hidx.impl_hints : impl_hints),
		      (impl_hints == 0 ? HIdxImplHintsCount : impl_hints_cnt));

  if (idx_n->status())
    return idx_n->status();

  idx_n->open(_hash_key, _hash_data);
  
  return copyRealize(idx_n);
}

Status
HIdx::copyRealize(Idx *idx) const
{
  Status s = Success;
  HIdxCursor curs(this);
  unsigned char *data = new unsigned char[hidx.datasz];

  for (;;) {
    Boolean found;
    Idx::Key key;

    s = curs.next(&found, data, &key);
    if (s) goto error;

    if (!found) break;

    s = idx->insert(key.getKey(), data);
    if (s) goto error;
  }

 error:
  delete [] data;
  return s;
}

Status
HIdx::reimplementToBTree(Oid &newoid, int degree, short dspid)
{
  BIdx bidx(dbh, hidx.datasz, &keytype,
	       (dspid == DefaultDspid ? hidx.dspid : dspid),
	       degree, 1);

  if (bidx.status())
    return bidx.status();

  bidx.open();

  Status s = copyRealize(&bidx);
  if (s) return s;
  s = destroy();
  if (s) return s;

  newoid = bidx.oid();
  return Success;
}

Status
HIdx::reimplementToHash(Oid &newoid, int key_count, int mag_order,
			   short dspid, const int *impl_hints,
			   unsigned int impl_hints_cnt,
			   hash_key_t _hash_key, void *_hash_data,
			   KeyType *ktype)
{
  IdxLock lockx(dbh, treeoid);
  Status s = lockx.lock();
  if (s) return s;

  HIdx *idx_n = 0;
  s = copy(idx_n, key_count, mag_order, dspid, impl_hints,
	   impl_hints_cnt, _hash_key, _hash_data, ktype);
  if (s) return s;

  s = destroy();
  if (s) return s;
  newoid = idx_n->oid();
  delete idx_n;
  return Success;
}

Status
HIdx::simulate(Stats &stats, int key_count, int mag_order,
		  const int *impl_hints, unsigned int impl_hints_cnt,
		  hash_key_t _hash_key,
		  void *_hash_data) const
{
  HIdx *idx_n;
  Status s = copy(idx_n, key_count, mag_order, hidx.dspid, impl_hints,
		     impl_hints_cnt, _hash_key, _hash_data, 0);
  if (s) return s;

  s = idx_n->getStats(stats);
  idx_n->destroy();
  delete idx_n;
  return s;
}

static void x2h_idx(HIdx::_Idx *idx)
{
#ifdef HIDX_XDR
  idx->idxtype = x2h_u32(idx->idxtype);
  idx->object_count = x2h_u32(idx->object_count);
  idx->mag_order = x2h_u32(idx->mag_order);
  idx->key_count = x2h_u32(idx->key_count);
  idx->dspid = x2h_16(idx->dspid);
  idx->keytype = x2h_u32(idx->keytype);
  idx->keysz = x2h_u32(idx->keysz);
  idx->datasz = x2h_u32(idx->datasz);
  idx->offset = x2h_u32(idx->offset);
  for (int i = 0; i < HIdxImplHintsCount; i++)
    idx->impl_hints[i] = x2h_u32(idx->impl_hints[i]);
#endif
}

static void h2x_idx(HIdx::_Idx *xidx, const HIdx::_Idx *hidx)
{
#ifdef HIDX_XDR
  xidx->idxtype = h2x_u32(hidx->idxtype);
  xidx->object_count = h2x_u32(hidx->object_count);
  xidx->mag_order = h2x_u32(hidx->mag_order);
  xidx->key_count = h2x_u32(hidx->key_count);
  xidx->dspid = h2x_16(hidx->dspid);
  xidx->keytype = h2x_u32(hidx->keytype);
  xidx->keysz = h2x_u32(hidx->keysz);
  xidx->datasz = h2x_u32(hidx->datasz);
  xidx->offset = h2x_u32(hidx->offset);
  for (int i = 0; i < HIdxImplHintsCount; i++)
    xidx->impl_hints[i] = h2x_u32(hidx->impl_hints[i]);
#else
  if (xidx != hidx)
    memcpy(xidx, hidx, sizeof(*xidx));
#endif
}

static void x2h_hat(HIdx::Hat *hat)
{
#ifdef HIDX_XDR
  x2h_oid(&hat->first, &hat->first);
  x2h_oid(&hat->last, &hat->last);
  x2h_oid(&hat->free_first, &hat->free_first);
#endif
}

static void h2x_hat(HIdx::Hat *xhat, const HIdx::Hat *hhat)
{
#ifdef HIDX_XDR
  h2x_oid(&xhat->first, &hhat->first);
  h2x_oid(&xhat->last, &hhat->last);
  h2x_oid(&xhat->free_first, &hhat->free_first);
#else
  if (xhat != hhat)
    memcpy(xhat, hhat, sizeof(*xhat));
#endif
}

static void x2h_header(HIdx::Header *h)
{
#ifdef HIDX_XDR
  h->size = x2h_32(h->size);
  h->free_cnt = x2h_u16(h->free_cnt);
  h->alloc_cnt = x2h_u16(h->alloc_cnt);
  h->free_whole = x2h_u32(h->free_whole);
  h->free_first = x2h_u32(h->free_first);
  x2h_oid(&h->free_prev, &h->free_prev);
  x2h_oid(&h->free_next, &h->free_next);
  x2h_oid(&h->prev, &h->prev);
  x2h_oid(&h->next, &h->next);
#endif
}

static void h2x_header(HIdx::Header *xh, const HIdx::Header *hh)
{
#ifdef HIDX_XDR
  xh->size = h2x_32(hh->size);
  xh->free_cnt = h2x_u16(hh->free_cnt);
  xh->alloc_cnt = h2x_u16(hh->alloc_cnt);
  xh->free_whole = h2x_u32(hh->free_whole);
  xh->free_first = h2x_32(hh->free_first);
  h2x_oid(&xh->free_prev, &hh->free_prev);
  h2x_oid(&xh->free_next, &hh->free_next);
  h2x_oid(&xh->prev, &hh->prev);
  h2x_oid(&xh->next, &hh->next);
#else
  if (xh != hh)
    memcpy(xh, hh, sizeof(*xh));
#endif
}

static void x2h_overhead(HIdx::Overhead *o)
{
#ifdef HIDX_XDR_OVERHEAD
  unsigned int x;
  mcp(&x, o, sizeof(x));
  x = x2h_u32(x);
  o->free = x >> 31;
  o->size = x & 0xefffffff;
  o->free_next = x2h_32(o->free_next);
  o->free_prev = x2h_32(o->free_prev);
#endif
}

static void h2x_overhead(HIdx::Overhead *xo, const HIdx::Overhead *ho)
{
#ifdef HIDX_XDR_OVERHEAD
  unsigned int x = h2x_u32((ho->free << 31) | ho->size);
  mcp(xo, &x, sizeof(x));
  xo->free_next = h2x_32(ho->free_next);
  xo->free_prev = h2x_32(ho->free_prev);
#else
  if (xo != ho)
    memcpy(xo, ho, sizeof(*xo));
#endif
}

}
