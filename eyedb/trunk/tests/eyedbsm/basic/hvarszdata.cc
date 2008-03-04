
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


#include "olib.h"

static const unsigned int DATASZ = sizeof(eyedbsm::Oid);

static void create_hash(void *x)
{
  eyedbsm::Idx::KeyType kt = o_get_idxKeyType("string:-1");

  eyedbsm::HIdx hidx(o_dbh, kt, DATASZ, o_dspid, o_magorder,
		     o_keycount, o_impl_hints, eyedbsm::HIdxImplHintsCount);

  eyedbsm::Status s = hidx.status();

  if (s) {
    eyedbsm::statusPrint(s, "creating hash index");
  }

  o_oids[0] = hidx.oid();

  printf("has created hash index %s [keytype: %s]\n", o_getOidString(&o_oids[0]), o_keytype);
}

static void o_insert()
{
  eyedbsm::HIdx hidx(o_dbh, &o_oids[0]);

  for (int n = 0; n < 120; n++) {
    char key[64];
    sprintf(key, "key-%d", n);
    char data[DATASZ];
    memset(data, 0, sizeof(data));
    sprintf(data, "#%d", n, 2*n);
    eyedbsm::Status s = hidx.insert(key, data);
    if (s) {
      eyedbsm::statusPrint(s, "inserting");
      break;
    }
  }
}

static void o_read()
{
  eyedbsm::HIdx hidx(o_dbh, &o_oids[0]);

  eyedbsm::HIdxCursor c(&hidx);
  unsigned int count;

  for (count = 0; ; count++) {
    eyedbsm::Idx::Key key;
    eyedbsm::Boolean found;
    char data[DATASZ];

    eyedbsm::Status s = c.next(&found, data, &key);
    if (s) {
      eyedbsm::statusPrint(s, "getting next");
      break;
    }

    if (!found)
      break;

    printf("[%s] -> [%s]\n", key.getKey(), data);
  }

  printf("count %u\n", count);
}

static int usage(const char *prog)
{
  std::cerr << "usage: " << prog << " DATABASE HOIDFILE\n";
  return 1;
}

int main(int argc, char *argv[])
{
  if (argc != 3) {
    return usage(argv[0]);
  }

  eyedbsm::init();

  o_dbfile = argv[1];

  if (o_dbopen()) {
    return 1;
  }

  o_oidfile = argv[2];

  if (o_fileopen(o_Create)) {
    return 1;
  }

  if (o_trsbegin())
    return 1;

  o_bench(create_hash, 0);

  if (o_trsend())
    return 1;

  o_write_oids(o_oids, 1); // not really necessary

  if (o_trsbegin())
    return 1;

  o_insert();

  if (o_trsend())
    return 1;

  if (o_trsbegin())
    return 1;

  o_read();

  if (o_trsend())
    return 1;

  return o_release();
}

