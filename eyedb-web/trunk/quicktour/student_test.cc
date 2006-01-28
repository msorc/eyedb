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

#include <iostream>
#include "student.h"

static void create(eyedb::Database *db)
{
  Course *perl = new Course(db);
  perl->setTitle("Perl");
  perl->setDescription("Perl Language");

  Course *python = new Course(db);
  python->setTitle("Python");
  python->setDescription("Python Language");

  Course *eyedb_ = new Course(db);
  eyedb_->setTitle("EyeDB");
  eyedb_->setDescription("EyeDB OODBMS");

  Student *henri_muller = new Student(db);
  henri_muller->setFirstname("Henri");
  henri_muller->setLastname("Muller");
  henri_muller->setBeginYear(2003);

  Student *jacques_martin = new Student(db);
  jacques_martin->setFirstname("Jacques");
  jacques_martin->setLastname("Martin");
  jacques_martin->setBeginYear(2003);
  
  Student *mary_kiss = new Student(db);
  mary_kiss->setFirstname("Mary");
  mary_kiss->setLastname("Kiss");
  mary_kiss->setBeginYear(2003);

  Teacher *max_first = new Teacher(db);
  max_first->setFirstname("Max");
  max_first->setLastname("First");

  Teacher *georges_shorter = new Teacher(db);
  georges_shorter->setFirstname("Georges");
  georges_shorter->setLastname("Shorter");

  perl->setTeacher(max_first);
  python->setTeacher(max_first);
  eyedb_->setTeacher(georges_shorter);

  henri_muller->addToCoursesColl(perl);
  henri_muller->addToCoursesColl(eyedb_);

  jacques_martin->addToCoursesColl(python);
  jacques_martin->addToCoursesColl(perl);
  jacques_martin->addToCoursesColl(eyedb_);

  mary_kiss->addToCoursesColl(python);

  // storing objects to database
  henri_muller->store(eyedb::RecMode::FullRecurs);
  jacques_martin->store(eyedb::RecMode::FullRecurs);
  mary_kiss->store(eyedb::RecMode::FullRecurs);
}

static void query_students(eyedb::Database *db)
{
  eyedb::OQL oql(db, "select Student");
  eyedb::ObjectArray obj_arr;
  oql.execute(obj_arr);
  unsigned int count = obj_arr.getCount();
  for (int n = 0; n < count; n++) {
    Student *s = Student_c(obj_arr[n]);
    if (s) {
      std::cout << s->getFirstname() << " " << s->getLastname() << std::endl;
    }
  }
}

static void query_courses(eyedb::Database *db,
			  const char *firstname, const char *lastname)
{
  eyedb::OQL oql(db, "select c from Course c where "
		 "c.teacher.lastname = \"%s\" && "
		 "c.teacher.firstname = \"%s\"", lastname, firstname);
  eyedb::ObjectArray obj_arr;
  oql.execute(obj_arr);
  unsigned int count = obj_arr.getCount();
  for (int n = 0; n < count; n++) {
    Course *c = Course_c(obj_arr[n]);
    if (c) {
      std::cout << firstname << " " << lastname << ": " <<
	c->getTitle() << " " << c->getDescription() << std::endl;
    }
  }
}

int
main(int argc, char *argv[])
{
  eyedb::init(argc, argv);
  student::init();

  if (argc != 2) {
    std::cerr << "usage: " << argv[0] << " <dbname>" << std::endl;
    return 1;
  }

  eyedb::Exception::setMode(eyedb::Exception::ExceptionMode);

  try {
    eyedb::Connection conn;

    conn.open();

    studentDatabase db(argv[1]);
    db.open(&conn, eyedb::Database::DBRW);

    db.transactionBegin();

    create(&db);
    query_students(&db);
    query_courses(&db, "Max", "First");

    db.transactionCommit();

    db.close();
  }
  catch(eyedb::Exception &e) {
    std::cerr << e << std::endl;
    return 1;
  }

  student::release();
  eyedb::release();

  return 0;
}
