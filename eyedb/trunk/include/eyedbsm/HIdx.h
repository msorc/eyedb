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


#ifndef	_eyedbsm_HIdx_
#define	_eyedbsm_HIdx_

#include <stdio.h>
#include <eyedblib/machtypes.h>
#include <eyedbsm/Idx.h>

namespace eyedblib {
  class ThreadPool;
  class ThreadPerformer;
}

namespace eyedbsm {

  /**
     @addtogroup eyedbsm
     @{
  */

  class HIdxCursor;

  static const int HIdxImplHintsCount = 8;

  /**
     Not yet documented.
  */
  class HIdx : public Idx {

    friend class HIdxCursor;

  public: /* conceptually private */
    class Overhead;
    class Header;
    class Hat;

  public:
    enum HIdxKeyType {
      stringType = 1,
      dataType
    };


    struct _Idx {
      unsigned int idxtype;
      unsigned int object_count;
      unsigned int mag_order;
      unsigned int key_count;
      short dspid;
      unsigned int keytype;
      unsigned int keysz;
      unsigned int datasz;
      unsigned int offset;
      int impl_hints[HIdxImplHintsCount];
      void trace(FILE *fd = stdout) const;
      std::string toString() const;
    };

  public: //@@@@ A.W just try to cure
    struct _Idx hidx;
  private:
    unsigned int mask;
    Boolean pow2;
    Boolean uextend;
    Oid treeoid;
    Status stat;
    DbHandle *dbh;
    int bsize;
    hash_key_t hash_key;
    void *hash_data;
    int version;
    Boolean nocopy;

    Status dumpMemoryMap(const Hat &, const char *msg = "", FILE *fd = stdout);

    Status readOverhead(int off, const Oid &, Overhead &) const;
    Status writeOverhead(int off, const Oid &, const Overhead &) const;
    Status readHeader(const Oid &, Header &) const;
    Status writeHeader(const Oid &, const Header &) const;
    Status readHat(int k, Hat &hat) const;
    Status writeHat(int k, const Hat &hat) const;
    Status readHats(Hat *&hats) const;
    Status writeHats(const Hat *hats) const;

    Status insertCell(int offset, unsigned int size, Header &h,
		      const Oid &koid) const;
    Status suppressCell(int offset, Header &h, const Oid &koid) const;
    Status get_key(int &, const void *, unsigned int * = 0) const;
    int cmp(const void *, const void *, unsigned char) const;
    Status count_manage(DbHandle *dbh, int inc);
    Status getCell(unsigned int size, Hat &hat, int hat_k,
		   Oid &koid, Header &h, int &offset, Overhead &o);
    Status insert_realize(Hat &hat, int hat_k, const void *key,
			  unsigned int size, const void *xdata,
			  const Oid &koid, Header &h, int offset,
			  Overhead &o);
    Status remove_realize(Hat *hat, int hat_key,
			  const char *, const char *, const char *,
			  const Overhead *, const Oid *);
    Status getObjectToExtend(unsigned int size, Hat &hat, int hat_k,
			     Oid &koid, Header &h, int &offset,
			     Overhead &o, Boolean &found);
    Status extendObject(Hat &hat, int hat_k, const Oid &koid,
			Header &h, Boolean &extended);
    Status extendObject(unsigned int size, Hat &hat, int hat_k, Oid &koid,
			Header &h, int &offset, Overhead &o,
			Boolean &extended);
    Boolean candidateForExtension(const Header &);
    Status makeObject(Hat &hat, int hat_k, Oid &koid, int &offset,
		      Header &h, Overhead &o, unsigned int objsize);
    static bool inFreeList(const Header &h, const Hat &hat, const Oid &koid);
    Status insertObjectInFreeList(Hat &hat, int hat_k, Header &h,
				  const Oid &koid);
    Status suppressObjectFromFreeList(Hat &hat, int hat_k, Header &h,
				      const Oid &koid);
    Status suppressObjectFromList(Hat &hat, int hat_k, Header &h,
				  const Oid &koid);
    Status replaceObjectInList(Hat &hat, int hat_k, Header &h,
			       const Oid &koid, const Oid &nkoid);
    Status modifyObjectSize(int osize, int nsize, const Oid &koid, 
			    Oid &nkoid);
    Status headPrint(FILE *, int, Oid *, int&) const;
    Status getEntryCount(Oid *, int& ) const;
    Status getHashObjectBusySize(const Oid *koid, unsigned int &busysize,
				 unsigned int size = 0) const;
    static Status get_def_string_hash_key(const void *key, unsigned int len,
					  void *, int &);
    static Status get_def_nstring_hash_key(const void *key, unsigned int len,
					   void *, int &);
    static Status get_def_rawdata_hash_key(const void *key, unsigned int len,
					   void *, int &);
    static Status get_def_int16data_hash_key(const void *key,
					     unsigned int len, void *, int &);
    static Status get_def_int32data_hash_key(const void *key,
					     unsigned int len, void *, int &);
    static Status get_def_int64data_hash_key(const void *key,
					     unsigned int len, void *, int &);
    static Status get_def_oiddata_hash_key(const void *key, unsigned int len,
					   void *, int &);
    static Status get_def_float32data_hash_key(const void *key,
					       unsigned int len,
					       void *, int &);
    static Status get_def_float64data_hash_key(const void *key,
					       unsigned int len,
					       void *, int &);

    Status get_string_hash_key(const void *key, unsigned int len, int &) const;
    Status get_rawdata_hash_key(const void *key, unsigned int len, int &) const;
    void set_hash_key();
    Status destroy_r();
    Status copyRealize(Idx *) const;
    void init(DbHandle *, unsigned int keytype, unsigned int keysz,
	      unsigned int offset,
	      unsigned int datasz, short dspid,
	      int mag_order, int key_count,
	      const int *impl_hints,
	      unsigned int impl_hints_cnt);
    KeyType keytype;
  public:

    enum Hints {
      IniSize_Hints,
      IniObjCnt_Hints,
      XCoef_Hints,
      SzMax_Hints
    };

    static const unsigned int MaxKeys;
    static const unsigned int VarSize;
    static const unsigned int MagorderKeycountCoef;

    /**
       Not yet documented
       @param vh
       @param type
       @param datasz
       @param dspid
       @param magorder
       @param key_count
       @param impl_hints
       @param impl_hints_cnt
    */
    HIdx(DbHandle *vh, KeyType type,
	 unsigned int datasz, short dspid,
	 int magorder, int key_count = 0,
	 const int *impl_hints = 0,
	 unsigned int impl_hints_cnt = 0);

    /**
       Not yet documented
       @param vh
       @param poid
       @param hash_key
       @param hash_data
       @param precmp
    */
    HIdx(DbHandle *vh, const Oid *poid,
	 hash_key_t hash_key = 0,
	 void *hash_data = 0,
	 Boolean (*precmp)(void const * p, void const * q,
			   KeyType const * type, int & r) = 0);

    /**
       Not yet documented
       @param hash_key
       @param hash_data
       @param precmp
    */
    void open(hash_key_t hash_key = 0,
	      void *hash_data = 0,
	      Boolean (*precmp)(void const * p, void const * q,
				KeyType const * type, int & r) = 0);

    /**
       Not yet documented
       @return
    */
    unsigned int getCount() const;

    /**
       Not yet documented
       @return
    */
    short getDefaultDspid() const;

    /**
       Not yet documented
       @param dspid
    */
    void setDefaultDspid(short dspid);

    /**
       Not yet documented
       @param oids
       @param cnt
       @return
    */
    Status getObjects(Oid *&oids, unsigned int &cnt) const;

    /**
       Not yet documented
       @return
    */
    virtual HIdx *asHIdx() {return this;}

    /**
       Not yet documented
       @return
    */
    const KeyType & getKeyType() const {return keytype;}

    /**
       Not yet documented
       @return
    */
    const HIdx::_Idx & getIdx() const {return hidx;}

    ~HIdx();
  
    /**
       Not yet documented
       @param key
       @param xdata
       @return
    */
    Status insert(const void *key, const void *xdata);

    /**
       Not yet documented
       @param key
       @param xdata
       @param found
       @return
    */
    Status remove(const void *key, const void *xdata, Boolean *found = 0);

    /**
       Not yet documented
       @param key
       @param found
       @param xdata
       @return
    */
    Status search(const void *key, Boolean *found, void *xdata = 0);

    /**
       Not yet documented
       @return
    */
    Status destroy();

    /**
       Not yet documented
       @param fd
       @return
    */
    Status printStat(FILE *fd = stdout) const;

    /**
       Not yet documented
       @param fd
       @return
    */
    Status dumpMemoryMap(FILE *fd = stdout);

    /**
       Not yet documented
       @return
    */
    Oid const& oid() const {return treeoid;}

    /**
       Not yet documented
       @return
    */
    Status status() const {return stat;}

    /**
       Not yet documented
       @return
    */
    unsigned int getKeyCount() const {return hidx.key_count;}

    /**
       Not yet documented
       @param idx_n
       @param key_count
       @param mag_order
       @param dspid
       @param impl_hints
       @param impl_hints_cnt
       @param _hash_key
       @param _hash_data
       @param ktype
       @return
    */
    Status copy(HIdx *&idx_n, int key_count, int mag_order = 0,
		short dspid = DefaultDspid,
		const int *impl_hints = 0, unsigned int impl_hints_cnt = 0,
		hash_key_t _hash_key = 0,
		void *_hash_data = 0,
		KeyType *ktype = 0) const;

    /**
       Not yet documented
       @param keycount
       @return
    */
    static unsigned int getMagOrder(unsigned int keycount);

    /**
       Not yet documented
       @param magorder
       @return
    */
    static unsigned int getKeyCount(unsigned int magorder);

    // used for audit and scalibility
    Status getStats(std::string& stats) const;
    struct Stats {
      _Idx idx;
      struct Entry {
	unsigned int object_count;
	unsigned int hash_object_count;
	unsigned int hash_object_size;
	unsigned int hash_object_busy_size;
      } *entries;
      unsigned int min_objects_per_entry;
      unsigned int max_objects_per_entry;
      unsigned int total_object_count;
      unsigned int total_hash_object_count;
      unsigned int total_hash_object_size;
      unsigned int total_hash_object_busy_size;
      unsigned int busy_key_count;
      unsigned int free_key_count;

      Stats();
      ~Stats();
      void trace(Boolean full = True, FILE *fd = stdout) const;
      std::string toString(Boolean full = True) const;
    };

    /**
       Not yet documented
       @param stats
       @return
    */
    Status getStats(HIdx::Stats &stats) const;

    /**
       Not yet documented
       @param newoid
       @param key_count
       @param mag_order
       @param dspid
       @param impl_hints
       @param impl_hints_cnt
       @param hash_key
       @param hash_data
       @param ktype
       @return
    */
    Status reimplementToHash(Oid &newoid, int key_count, int mag_order = 0,
			     short dspid = DefaultDspid,
			     const int *impl_hints = 0,
			     unsigned int impl_hints_cnt = 0,
			     hash_key_t hash_key = 0,
			     void *hash_data = 0,
			     KeyType *ktype = 0);

    /**
       Not yet documented
       @param newoid
       @param degree
       @param dspid
       @return
    */
    Status reimplementToBTree(Oid &newoid, int degree = 0,
			      short dspid = DefaultDspid);

    /**
       Not yet documented
       @param stats
       @param key_count
       @param mag_order
       @param impl_hints
       @param impl_hints_cnt
       @param hash_key
       @param hash_data
       @return
    */
    Status simulate(HIdx::Stats &stats, int key_count,
		    int mag_order = 0,
		    const int *impl_hints = 0,
		    unsigned int impl_hints_cnt = 0,
		    hash_key_t hash_key = 0,
		    void *hash_data = 0) const;

  public: /* conceptually private */
    struct Overhead {
    public:
      eyedblib::uint32 free:1, size:31;
      eyedblib::int32 free_prev, free_next;
    };

    struct Header {
    public:
      unsigned int size;
      eyedblib::uint16 free_cnt, alloc_cnt;
      eyedblib::uint32 free_whole;
      eyedblib::int32 free_first;
      Oid free_prev, free_next;
      Oid prev, next;
    };
  
    struct Hat {
    public:
      Oid first, last;
      Oid free_first;
    };
  };

  /**
     Not yet documented.
  */
  class HIdxCursor : public IdxCursor {

    const HIdx *idx;
    void *skey, *ekey;
    Boolean sexcl, eexcl;
    static const char defaultSKey[];
    char *sdata, *edata, *cur;
    Boolean nocopy;
    Boolean data_tofree;
    Status read(Boolean& end);
    Boolean equal;
    int k_cur, k_end;
    int cmp_realize(const void *, const void *, Boolean, unsigned char) const;
    int cmp(const void *) const;
    Oid koid;
    void *copy_key(const void *, unsigned int, Boolean);
    Boolean state;
    Boolean (*user_cmp)(const void *key, void *cmp_arg);
    void *cmp_arg;

    void init(DbHandle *);

    // parallel management
    unsigned int perf_cnt, perf_end_cnt;
    Boolean master, slave;
    class LinkList;
    Boolean parallelInit(int thread_cnt);
    eyedblib::ThreadPerformer **perfs;
    HIdxCursor **perf_curs;
    LinkList *list;
    LinkList **lists;
    eyedblib::ThreadPool *thrpool;
    HIdxCursor(const HIdx *,
	       unsigned int k_start,
	       unsigned int k_end,
	       const void *,
	       const void *,
	       Boolean,
	       Boolean,
	       Boolean (*user_cmp)(const void *, void *),
	       void *cmp_arg,
	       LinkList *);

  public:
    /**
       Not yet documented
       @param idx
       @param skey
       @param ekey
       @param sexcl
       @param eexcl
       @param user_cmp
       @param cmp_arg
       @param thread_cnt
    */
    HIdxCursor(const HIdx *idx,
	       const void *skey = 0,
	       const void *ekey = defaultSKey,
	       Boolean sexcl = False,
	       Boolean eexcl = False,
	       Boolean (*user_cmp)(const void *key, void *cmp_arg) = 0,
	       void *cmp_arg = 0,
	       int thread_cnt = 0);

    /**
       Not yet documented
       @param found
       @param data
       @param key
       @return
    */
    Status next(Boolean *found, void *data = 0, Idx::Key *key = 0);

    ~HIdxCursor();
  };

  /**
     @}
  */

};

#endif
