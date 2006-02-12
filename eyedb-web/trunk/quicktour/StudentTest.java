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


import student.*;

class StudentTest {
    public static void main(String args[]) {

	// Initialize the eyedb package and parse the default eyedb options
	// on the command line
	String[] outargs = org.eyedb.Root.init("StudentTest", args);
     
	// Check that a database name is given on the command line
	int argc = outargs.length;
	if (argc != 1) {
	    System.err.println("usage: java StudentTest dbname");
	    System.exit(1);
	}

	try {
	    // Initialize the student package
	    student.Database.init();

	    // Open the connection with the server
	    org.eyedb.Connection conn = new org.eyedb.Connection();

	    // Open the database named outargs[0]
	    student.Database db = new student.Database(outargs[0]);
	    db.open(conn, org.eyedb.Database.DBRW);

	    db.transactionBegin();

	    create(db);
	    query_students(db);
	    query_courses(db, "Max", "First");

	    db.transactionCommit();
	}
	catch(org.eyedb.Exception e) { // Catch any eyedb exception
	    e.print();
	    System.exit(1);
	}
    }

    static void create(student.Database db) throws org.eyedb.Exception {
	Course perl = new Course(db);
	perl.setTitle("Perl");
	perl.setDescription("Perl Language");

	Course python = new Course(db);
	python.setTitle("Python");
	python.setDescription("Python Language");

	Course eyedb_ = new Course(db);
	eyedb_.setTitle("EyeDB");
	eyedb_.setDescription("EyeDB OODBMS");

	Student henri_muller = new Student(db);
	henri_muller.setFirstname("Henri");
	henri_muller.setLastname("Muller");
	henri_muller.setBeginYear((short)2003);

	Student jacques_martin = new Student(db);
	jacques_martin.setFirstname("Jacques");
	jacques_martin.setLastname("Martin");
	jacques_martin.setBeginYear((short)2003);
  
	Student mary_kiss = new Student(db);
	mary_kiss.setFirstname("Mary");
	mary_kiss.setLastname("Kiss");
	mary_kiss.setBeginYear((short)2003);

	Teacher max_first = new Teacher(db);
	max_first.setFirstname("Max");
	max_first.setLastname("First");

	Teacher georges_shorter = new Teacher(db);
	georges_shorter.setFirstname("Georges");
	georges_shorter.setLastname("Shorter");

	perl.setTeacher(max_first);
	python.setTeacher(max_first);
	eyedb_.setTeacher(georges_shorter);

	henri_muller.addToCoursesColl(perl);
	henri_muller.addToCoursesColl(eyedb_);

	jacques_martin.addToCoursesColl(python);
	jacques_martin.addToCoursesColl(perl);
	jacques_martin.addToCoursesColl(eyedb_);

	mary_kiss.addToCoursesColl(python);

	// storing objects to database
	henri_muller.store(org.eyedb.RecMode.FullRecurs);
	jacques_martin.store(org.eyedb.RecMode.FullRecurs);
	mary_kiss.store(org.eyedb.RecMode.FullRecurs);
    }

    static void query_students(org.eyedb.Database db)
	throws org.eyedb.Exception {
	org.eyedb.OQL oql = new org.eyedb.OQL(db, "select Student");
	org.eyedb.ObjectArray obj_arr = new org.eyedb.ObjectArray();
	oql.execute(obj_arr);
	int count = obj_arr.getCount();
	for (int n = 0; n < count; n++) {
	    Student s = (Student)obj_arr.getObject(n);
	    if (s != null) {
		System.out.println(s.getFirstname() + " " + s.getLastname());
	    }
	}
    }

    static void query_courses(org.eyedb.Database db,
			      String firstname, String lastname)
	throws org.eyedb.Exception
    {
	org.eyedb.OQL oql = new org.eyedb.OQL
	    (db, "select c from Course c where " +
	     "c.teacher.lastname = \"" + lastname + "\" && " +
	     "c.teacher.firstname = \"" + firstname + "\"");
	
	org.eyedb.ObjectArray obj_arr = new org.eyedb.ObjectArray();
	oql.execute(obj_arr);
	int count = obj_arr.getCount();
	for (int n = 0; n < count; n++) {
	    Course c = (Course)obj_arr.getObject(n);
	    if (c != null) {
		System.out.println(firstname + " " + lastname + ": " +
				   c.getTitle() + " " + c.getDescription());
	    }
	}
    }
}
