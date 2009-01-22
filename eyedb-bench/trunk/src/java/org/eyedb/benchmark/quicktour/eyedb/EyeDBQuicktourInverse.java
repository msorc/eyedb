package org.eyedb.benchmark.quicktour.eyedb;

import org.eyedb.Connection;
import org.eyedb.Database;
import org.eyedb.benchmark.quicktour.eyedb.inverse.Course;
import org.eyedb.benchmark.quicktour.eyedb.inverse.Student;
import org.eyedb.benchmark.quicktour.eyedb.inverse.Teacher;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public class EyeDBQuicktourInverse extends EyeDBQuicktour {

    public String getImplementation()
    {
	return "EyeDB Java implementation using referential integrity";
    }

    public void prepare()
    {
	String databaseName = getProperties().getProperty( "eyedb.database");
	int tcpPort = getProperties().getIntProperty( "eyedb.tcp_port");

	String[] args = new String[3];
	int i = 0;
	args[i++] = "--user=" + System.getProperty( "user.name");
	args[i++] = "--dbm=default";
	args[i++] = "--port=" + tcpPort;

	org.eyedb.Root.init( databaseName, args);

	try {
	    // Initialize the package
	    org.eyedb.benchmark.quicktour.eyedb.inverse.Database.init();

	    // Open the connection with the backend
	    connection = new org.eyedb.Connection();

	    // Open the database
	    database = new org.eyedb.benchmark.quicktour.eyedb.inverse.Database( databaseName);
	    database.open(connection, org.eyedb.Database.DBRW);
	}
	catch(org.eyedb.Exception e) {
	    e.printStackTrace();
	    System.exit(1);
	}
    }

    private Teacher[] fillTeachers( int nTeachers) throws org.eyedb.Exception
    {
	Teacher[] teachers = new Teacher[nTeachers];

	getDatabase().transactionBegin();

	for ( int n = 0; n < nTeachers; n++) {
	    teachers[n] = new Teacher( getDatabase());
	    teachers[n].setFirstName( "Teacher_"+n+"_firstName");
	    teachers[n].setLastName( "Teacher_"+n);

	    teachers[n].store(org.eyedb.RecMode.FullRecurs);
	}

	getDatabase().transactionCommit();

	return teachers;
    }

    private Course[] fillCourses( int nCourses, Teacher[] teachers) throws org.eyedb.Exception
    {
	Course courses[] = new Course[ nCourses];

	getDatabase().transactionBegin();

	for ( int n = 0; n < nCourses; n++) {
	    courses[n] = new Course( getDatabase());
	    courses[n].setTitle( "Course_"+n);
	    courses[n].setDescription("Description of course "+n);
	    courses[n].setTeacher( teachers[ getRandom().nextInt( teachers.length)]);

	    courses[n].store(org.eyedb.RecMode.FullRecurs);
	}

	getDatabase().transactionCommit();

	return courses;
    }

    public void create( int nStudents, int nCourses, int nTeachers, int nObjectsPerTransaction)
    {
	try {
	    Teacher[] teachers = fillTeachers( nTeachers);
	    Course[] courses = fillCourses( nCourses, teachers);

	    getDatabase().transactionBegin();

	    for ( int n = 0; n < nStudents; n++) {
		Student student = new Student( getDatabase());
		student.setFirstName( "Student_"+n+"_firstName");
		student.setLastName( "Student_"+n);
		student.setBeginYear( (short)(getRandom().nextInt( 3) + 1));

		int i = getRandom().nextInt( courses.length);
		for ( int c = 0; c < courses.length; c++) {
		    Course course = courses[ (i+c)%nCourses];
		    student.addToCoursesColl( course);
		}

		student.store(org.eyedb.RecMode.FullRecurs);

		if (n % nObjectsPerTransaction == nObjectsPerTransaction - 1) {
		    getDatabase().transactionCommit();
		    getDatabase().transactionBegin();
		}
	    }

	    getDatabase().transactionCommit();
	}
	catch( org.eyedb.Exception e) {
	    e.printStackTrace();
	    System.exit(1);
	}
    }
}
