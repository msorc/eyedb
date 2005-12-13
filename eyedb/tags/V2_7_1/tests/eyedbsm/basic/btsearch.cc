
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


#include "olib.h"

static int
usage()
{
  o_usage(eyedbsm::False);
  fprintf(stderr, "[-remove] <key>\n");
  return 1;
}

static unsigned int count;
static int from;
static eyedbsm::Boolean rmv = eyedbsm::False;

static void
read_btree(void *x)
{
  eyedbsm::BIdx bidx(o_dbh, o_oids[0]);
  eyedbsm::Status s = bidx.status();
  if (s) {
    eyedbsm::statusPrint(s, "reading btree index");
    return;
  }

  unsigned nkeys;
  const eyedbsm::Idx::KeyType *kt = bidx.getKeyTypes(nkeys);
  if (nkeys != 1) {
    fprintf(stderr, "cannot manage btree with multiple keys\n");
    return;
  }

  int len;
  char *data = o_make_data(kt[0], 0, from, len, eyedbsm::False);
  char *key = new char[len];
  memcpy(key, data, len);
  data = o_make_data(kt[0], 0, from, len, eyedbsm::False);

  printf("searching ");
  o_trace_data(kt[0], key, eyedbsm::False);
  printf("\n");

  eyedbsm::Boolean found;
  eyedbsm::Oid data_oid;
  s = bidx.search(key, &found, &data_oid);
  if (s) {
    eyedbsm::statusPrint(s, "searching hash index");
    return;
  }

  if (found)
    printf("found -> %s\n", eyedbsm::getOidString(&data_oid));
  else
    printf("not found\n");

  delete [] key;
}

int
main(int argc, char *argv[])
{
  if (o_init(argc, argv, o_Read, usage, 1))
    return 1;

  if (o_trsbegin())
    return 1;

  if (argc != 1 && argc != 2)
    return usage();

  int start = 0;
  if (!strcmp(argv[0], "-remove")) {
    if (argc != 2)
      return usage();
    start = 1;
    rmv = eyedbsm::True;
  }
  else if (argc != 1)
    return usage();

  from = atoi(argv[start]);

  o_bench(read_btree, 0);

  if (o_trsend())
    return 1;

  return o_release();
}
