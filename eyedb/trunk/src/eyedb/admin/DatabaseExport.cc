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
   Author: Francois Dechelle <francois@dechelle.net>
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <eyedb/eyedb.h>
#include <eyedblib/xdr.h>
#include <eyedblib/rpc_lib.h>
#include "DatabaseExport.h"

using namespace eyedb;
using namespace std;

extern const std::string PROG_NAME;

static unsigned int magic = 0xde728eab;

#define LSEEK(FD, OFF, WH) \
   do {if (lseek(FD, OFF, WH) < 0) {perror("lseek"); return 1;}} while(0)

#define WRITE(FD, PTR, SZ) \
   do {if (write(FD, PTR, SZ) != (SZ)) {perror("write"); return 1;}} while(0)

#define READ(FD, PTR, SZ) \
   do {if (read(FD, PTR, SZ) != (SZ)) {perror("read"); return 1;}} while(0)

#define READ_XDR_16(FD, PTR, SZ) \
   do {assert(SZ == 2); \
       eyedblib::int16 i16; \
       READ(FD, &i16, SZ); \
       x2h_16_cpy(PTR, &i16);} while(0)

#define READ_XDR_16_ARR(FD, PTR2, NB) \
   do { for (int j = 0; j < NB; j++) { \
          READ_XDR_16(FD, (((short *)PTR2)+j), sizeof(*PTR2)); \
        } \
   } while(0)

#define READ_XDR_32(FD, PTR, SZ) \
   do {assert(SZ == 4); \
       eyedblib::int32 i32; \
       READ(FD, &i32, SZ); \
       x2h_32_cpy(PTR, &i32);} while(0)

#define READ_XDR_64(FD, PTR, SZ) \
   do {assert(SZ == 8); \
       eyedblib::int64 i64; \
       READ(FD, &i64, SZ); \
       x2h_64_cpy(PTR, &i64);} while(0)

#define WRITE_XDR_16(FD, PTR, SZ) \
   do {assert(SZ == 2); \
       eyedblib::int16 i16; \
       h2x_16_cpy(&i16, PTR); \
       WRITE(FD, &i16, SZ); } while(0)

#define WRITE_XDR_16_ARR(FD, PTR2, NB) \
   do { for (int j = 0; j < NB; j++) { \
          WRITE_XDR_16(FD, (((short *)PTR2)+j), sizeof(*PTR2)); \
       } \
   } while(0)

#define WRITE_XDR_32(FD, PTR, SZ) \
   do {assert(SZ == 4); \
       eyedblib::int32 i32; \
       h2x_32_cpy(&i32, PTR); \
       WRITE(FD, &i32, SZ); } while(0)

#define WRITE_XDR_64(FD, PTR, SZ) \
   do {assert(SZ == 8); \
       eyedblib::int64 i64; \
       h2x_64_cpy(&i64, PTR); \
       WRITE(FD, &i64, SZ); } while(0)


struct DBMethod {
  char extref[64];
  char file[256];
  int fd;
  unsigned long long cdsize, rsize;
};

struct DBDatafile {
  int datfd, dmpfd;
  char file[eyedbsm::L_FILENAME];
  char name[eyedbsm::L_NAME+1];
  unsigned long long maxsize;
  unsigned int slotsize;
  unsigned int dtype;
  short dspid;
  unsigned long long dat_cdsize, dat_rsize;
  unsigned long long dmp_cdsize, dmp_rsize;
};

struct DBDataspace {
  char name[eyedbsm::L_NAME+1];
  unsigned int ndat;
  short datid[eyedbsm::MAX_DAT_PER_DSP];
};

struct DBInfo {
  char dbname[256];
  off_t offset;
  unsigned long long size;
  eyedbsm::Oid::NX nbobjs;
  unsigned int dbid;
  unsigned int ndat;
  unsigned int ndsp;
  unsigned int nmths;
  unsigned long long db_cdsize, db_rsize;
  unsigned long long omp_cdsize, omp_rsize;
  DBDatafile *datafiles;
  DBDataspace *dataspaces;
  DBMethod *mths;
};

static const char *
get_ompfile(const char *dbfile)
{
  static char ompfile[256];
  strcpy(ompfile, dbfile);
  char *p = strrchr(ompfile, '.');
  if (p)
    *p = 0;

  strcat(ompfile, ".omp");
  return ompfile;
}

static int
code_string(int fd, const char *s)
{
  int len = strlen(s);
  WRITE_XDR_32(fd, &len, sizeof(len));
  WRITE(fd, s, len+1);
  return 0;
}

#define min(x,y) ((x)<(y)?(x):(y))

static int
copy(int fdsrc, int fddest, unsigned long long &size, int import = 0,
     unsigned long long rsize = 0,
     int info = 0, Bool lseek_ok = True)
{
  char buf[512];
  static char const zero[sizeof buf] = { 0 };
  int n;
  off_t zeros = 0;
  int cnt;
  unsigned long long cdsize;

  if (import)
    {
      cdsize = size;
      cnt = min(sizeof(buf), cdsize);
    }
  else
    cnt = sizeof(buf);

  size = 0;

  while ((n = read(fdsrc, buf, cnt)) > 0)
    {
      size += n;

      if (memcmp(buf, zero, n) == 0)
	zeros += n;
      else if (!info)
	{
	  if (zeros)
	    {
	      if (lseek_ok)
		LSEEK(fddest, zeros, SEEK_CUR);
	      else
		while (zeros--)
		  WRITE(fddest, zero, 1);

	      zeros = 0;
	    }

	  WRITE(fddest, buf, n);
	}
      else
	zeros = 0;

      if (import)
	{
	  cnt = min(sizeof(buf), cdsize-size);
	  if (size >= cdsize)
	    break;
	}
    }

  if (n < 0)
    return 1;

  size -= zeros;

  if (import && rsize > 0)
    {
      char zero = 0;
      LSEEK(fddest, rsize-1, SEEK_SET);
      WRITE(fddest, &zero, 1);
    }

  return 0;
}

static int
code_file(int fd, int file_fd, unsigned long long &totsize, Bool info, Bool lseek_ok)
{
  struct stat st = {0};
  if (file_fd != -1) {
    if (fstat(file_fd, &st) < 0)
      {
	perror("stat");
	return 1;
      }
  }

  unsigned long long size;
  if (info)
    {
      WRITE_XDR_64(fd, &st.st_size, sizeof(st.st_size)); // real size
      if (file_fd != -1) {
	if (copy(file_fd, fd, size, 0, 0, 1))
	  return 1;
      }
      else
	size = 0;
      WRITE_XDR_64(fd, &size, sizeof(size)); // sparsify size
    }
  else
    {
      WRITE_XDR_32(fd, &magic, sizeof(magic));
      if (file_fd != -1)
	if (copy(file_fd, fd, size, 0, 0, 0, lseek_ok)) return 1;
    }

  totsize += size;
  return 0;
}

static const char *
dmpfileGet(const char *datafile)
{
  static std::string str;
  char *s = strdup(datafile);
  char *p;
  if (p = strrchr(s, '.'))
    *p = 0;
  strcat(s, ".dmp");
  str = s;
  free(s);
  return str.c_str();
}

static int
export_realize(int fd, const DbInfoDescription &dbdesc,
	       const char *dbname, unsigned int dbid,
	       eyedbsm::Oid::NX nbobjs, int dbfd, int ompfd,
	       DBDatafile datafiles[], unsigned int ndat, 
	       DBDataspace dataspaces[], unsigned int ndsp,
	       DBMethod *mths, unsigned int nmths)
{
  Bool lseek_ok = (lseek(fd, 0, SEEK_CUR) < 0 ? False : True);

  WRITE_XDR_32(fd, &magic, sizeof(magic));
  unsigned int version = getVersionNumber();
  WRITE_XDR_32(fd, &version, sizeof(version));

  if (code_string(fd, dbname))
    return 1;

  WRITE_XDR_32(fd, &nbobjs, sizeof(nbobjs));
  WRITE_XDR_32(fd, &dbid, sizeof(dbid));
  WRITE_XDR_32(fd, &ndat, sizeof(ndat));
  WRITE_XDR_32(fd, &ndsp, sizeof(ndsp));
  WRITE_XDR_32(fd, &nmths, sizeof(nmths));

  unsigned long long size = 0;

  fprintf(stderr, "Coding header...\n");

  if (code_file(fd, dbfd, size, True, lseek_ok))
    return 1;
  if (code_file(fd, ompfd, size, True, lseek_ok))
    return 1;

  int i, j;

  for (i = 0; i < ndat; i++) {
    if (code_string(fd, datafiles[i].file))
      return 1;
    if (code_string(fd, datafiles[i].name))
      return 1;
    WRITE_XDR_64(fd, &datafiles[i].maxsize, sizeof(datafiles[i].maxsize));
    WRITE_XDR_32(fd, &datafiles[i].slotsize, sizeof(datafiles[i].slotsize));
    WRITE_XDR_32(fd, &datafiles[i].dtype, sizeof(datafiles[i].dtype));
    WRITE_XDR_16(fd, &datafiles[i].dspid, sizeof(datafiles[i].dspid));
    if (code_file(fd, datafiles[i].datfd, size, True, lseek_ok))
      return 1;
    if (code_file(fd, datafiles[i].dmpfd, size, True, lseek_ok))
      return 1;
  }

  for (i = 0; i < ndsp; i++) {
    if (code_string(fd, dataspaces[i].name))
      return 1;
    WRITE_XDR_32(fd, &dataspaces[i].ndat, sizeof(dataspaces[i].ndat));
    //    WRITE_XDR_16_ARR(fd, dataspaces[i].datid, dataspaces[i].ndat * sizeof(dataspaces[i].datid[0]));
    WRITE_XDR_16_ARR(fd, dataspaces[i].datid, dataspaces[i].ndat);
  }

  for (j = 0; j < nmths; j++) {
    if (code_string(fd, mths[j].extref))
      return 1;
    if (code_string(fd, mths[j].file))
      return 1;
    if (code_file(fd, mths[j].fd, size, True, lseek_ok))
      return 1;
  }

  WRITE_XDR_64(fd, &size, sizeof(size));

  //fprintf(stderr, "TOTAL SIZE = %d [%d]\n", size, lseek(fd, 0, SEEK_CUR));

  LSEEK(dbfd, 0, SEEK_SET);
  fprintf(stderr, "Coding file %s...\n", dbdesc.dbfile);
  if (code_file(fd, dbfd, size, False, lseek_ok)) return 1;
  LSEEK(ompfd, 0, SEEK_SET);
  fprintf(stderr, "Coding file %s...\n", get_ompfile(dbdesc.dbfile));
  if (code_file(fd, ompfd, size, False, lseek_ok)) return 1;

  for (i = 0; i < ndat; i++) {
    if (datafiles[i].datfd != -1)
      LSEEK(datafiles[i].datfd, 0, SEEK_SET);
    fprintf(stderr, "Coding file %s...\n", datafiles[i].file);
    if (code_file(fd, datafiles[i].datfd, size, False, lseek_ok))
      return 1;
    fprintf(stderr, "Coding file %s...\n", dmpfileGet(datafiles[i].file));
    if (datafiles[i].dmpfd != -1)
      LSEEK(datafiles[i].dmpfd, 0, SEEK_SET);
    if (code_file(fd, datafiles[i].dmpfd, size, False, lseek_ok))
      return 1;
  }

  for (j = 0; j < nmths; j++) {
    LSEEK(mths[j].fd, 0, SEEK_SET);
    fprintf(stderr, "Coding file %s...\n", mths[j].file);
    if (code_file(fd, mths[j].fd, size, False, lseek_ok))
      return 1;
  }

  return 0;
}

static int
is_system_method(const char *name)
{
  return !strncmp(name, "etcmthbe", strlen("etcmthbe")) ||
    !strncmp(name, "etcmthfe", strlen("etcmthfe")) ||
    !strncmp(name, "oqlctbmthbe", strlen("oqlctbmthbe")) || 
    !strncmp(name, "oqlctbmthfe", strlen("oqlctbmthfe")) ||
    !strncmp(name, "utilsmthfe", strlen("utilsmthfe")) ||
    !strncmp(name, "utilsmthfe", strlen("utilsmthfe"));
}

static int
methods_manage(Database *db, DBMethod*& mths, unsigned int& nmths)
{
  db->transactionBegin();

  OQL q( db, "select method");

  ObjectArray obj_arr;
  q.execute(obj_arr);

  int err = 0;
  mths = 0;
  nmths = 0;
  int n = obj_arr.getCount();
  if (n) {
    mths = new DBMethod[n];
    memset(mths, 0, sizeof(mths[0])*n);
    for (int i = 0; i < n; i++) {
      Method *m = (Method *)obj_arr[i];
      if (m->asBEMethod_OQL())
	continue;
      const char *extref = m->getEx()->getExtrefBody().c_str();
      if (is_system_method(extref))
	continue;

      int j;
      for (j = 0; j < nmths; j++)
	if (!strcmp(mths[j].extref, extref))
	  break;

      if (j == nmths) {
	int r = nmths;
	const char *s = Executable::getSOFile(extref);

	if (!s) {
	  std::cerr << PROG_NAME << ": error: cannot find file for extref '" << extref << "'\n";
	  err = 1;
	  continue;
	}

	if ((mths[nmths].fd = open(s, O_RDONLY)) < 0) {
	  std::cerr << PROG_NAME << ": error: cannot open method file '" << (const char *)s << "' for reading\n";
	  err = 1;
	  continue;
	}
	      
	strcpy(mths[nmths].extref, extref);
	strcpy(mths[nmths].file, Executable::makeExtRef(extref));
	nmths++;

	if (nmths == r) {
	  std::cerr << PROG_NAME << ": error: no '" << Executable::makeExtRef(extref) << "' method file found.\n";
	  err = 1;
	  strcpy(mths[nmths++].extref, extref);
	}
      }
    }
  }

  db->transactionAbort();
  return err;
}

int eyedb::databaseExport( Connection &conn, const char *dbname, const char *file)
{
  if (!eyedb::ServerConfig::getSValue("sopath")) {
    std::cerr << PROG_NAME << ": error: variable 'sopath' must be set in your configuration file.\n";
    return 1;
  }

  Database *db = new Database(dbname);

  db->open( &conn, Database::DBSRead);

  db->transactionBeginExclusive();

  unsigned int dbid = db->getDbid();
  DbCreateDescription dbdesc;
  db->getInfo( &conn, 0, 0, &dbdesc);

  int dbfd = open(dbdesc.dbfile, O_RDONLY);
  if (dbfd < 0) {
    std::cerr << PROG_NAME << ": error: cannot open dbfile '" << dbdesc.dbfile << "' for reading\n";
    return 1;
  }

  const char *ompfile = get_ompfile(dbdesc.dbfile);
  int ompfd = open(ompfile, O_RDONLY);
  if (ompfd < 0) {
    std::cerr << PROG_NAME << ": error: cannot open ompfile '" << ompfile << "' for reading\n";
    return 1;
  }

  const eyedbsm::DbCreateDescription *s = &dbdesc.sedbdesc;

  DBDatafile *datafiles = new DBDatafile[s->ndat];
  memset(datafiles, 0, sizeof(datafiles[0])*s->ndat);
  char *pathdir = strdup(dbdesc.dbfile);
  char *x = strrchr(pathdir, '/');
  if (x) *x = 0;
  
  for (int i = 0; i < s->ndat; i++) {
    const char *file = s->dat[i].file;
    const char *p = strrchr(file, '/');

    if (!*s->dat[i].file)
      *datafiles[i].file = 0;
    else if (p)
      strcpy(datafiles[i].file, p+1);
    else {
      strcpy(datafiles[i].file, file);
      file = strdup((std::string(pathdir) + "/" + std::string(s->dat[i].file)).c_str());
    }

    if (*datafiles[i].file) {
      if ((datafiles[i].datfd = open(file, O_RDONLY)) < 0) {
	std::cerr << PROG_NAME << ": error: cannot open datafile '" << file << "' for reading.\n";
	return 1;
      }

      if ((datafiles[i].dmpfd = open(dmpfileGet(file), O_RDONLY)) < 0) {
	std::cerr << PROG_NAME << ": error: cannot open dmpfile '" << dmpfileGet( file) << "' for reading.\n";
	return 1;
      }
    }
    else {
      datafiles[i].datfd = -1;
      datafiles[i].dmpfd = -1;
    }

    strcpy(datafiles[i].name, s->dat[i].name);
    datafiles[i].maxsize = s->dat[i].maxsize;
    datafiles[i].slotsize = s->dat[i].sizeslot;
    datafiles[i].dtype = s->dat[i].dtype;

    datafiles[i].dspid = s->dat[i].dspid;
  }

  DBDataspace *dataspaces = new DBDataspace[s->ndsp];

  for (int i = 0; i < s->ndsp; i++) {
    strcpy(dataspaces[i].name, s->dsp[i].name);
    dataspaces[i].ndat = s->dsp[i].ndat;
    memcpy(dataspaces[i].datid, s->dsp[i].datid,
	   s->dsp[i].ndat*sizeof(s->dsp[i].datid[0]));
  }

  int fd;

  if (!strcmp(file, "-"))
    fd = 1;
  else {
    fd = creat(file, 0666);

    if (fd < 0) {
      std::cerr << PROG_NAME << ": error: cannot create file '" << file << "'\n";
      return 1;
    }
  }

  unsigned int nmths;
  DBMethod *mths;

  if (methods_manage(db, mths, nmths))
    return 1;

  if (export_realize(fd, dbdesc, dbname, dbid, s->nbobjs, dbfd, ompfd,
		     datafiles, s->ndat,
		     dataspaces, s->ndsp,
		     mths, nmths)) {
    close(fd);
    return 1;
  }

  close(fd);

  db->transactionCommit();

  return 0;
}

