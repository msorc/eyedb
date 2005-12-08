
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

#include "person.h"

int
main(int argc, char *argv[])
{
  eyedb::init(argc, argv);
  person::init();

  if (argc != 3)
    {
      fprintf(stderr, "usage: %s <dbname> <query>\n", argv[0]);
      return 1;
    }

  eyedb::Exception::setMode(eyedb::Exception::ExceptionMode);

  try {
    eyedb::Connection conn;
    // connecting to the eyedb server
    conn.open();

    // opening database argv[1] using 'personDatabase' class
    personDatabase db(argv[1]);
    db.open(&conn, Database::DBRW);

    // beginning a transaction
    db.transactionBegin();

    // performing the OQL query argv[2]
    eyedb::OQL q(&db, argv[2]);

    eyedb::ObjectArray arr;
    q.execute(arr);

    // for each Person returned in the query, display its name and age,
    // its address, its spouse name and age and its cars
    for (int i = 0; i < arr.getCount(); i++)
      {
	Person *p = Person_c(arr[i]);
	if (p)
	  {
	    cout << "name:    " << p->getName() << endl;
	    cout << "age:     " << p->getAge() << endl;

	    if (*p->getAddr()->getStreet())
	      cout << "street:  " << p->getAddr()->getStreet() << endl;

	    if (*p->getAddr()->getTown())
	      cout << "town:    " << p->getAddr()->getTown() << endl;

	    if (p->getSpouse())
	      {
		cout << "spouse_name: " << p->getSpouse()->getName() << endl;
		cout << "spouse_age:  " << p->getSpouse()->getAge() << endl;
	      }

#if 0
	    for (int i = 0; i < p->getCarsCount(); i++)
	      cout << "car: #" << i << ": " <<
		p->getCarsAt(i)->getMark() << ";" <<
		p->getCarsAt(i)->getNum() << endl;
#endif
	  }
      }

    // committing the transaction
    db.transactionCommit();
  }

  catch(eyedb::Exception &e) {
    cerr << argv[0] << ": " << e;
    eyedb::release();
    return 1;
  }

  eyedb::release();

  return 0;
}
