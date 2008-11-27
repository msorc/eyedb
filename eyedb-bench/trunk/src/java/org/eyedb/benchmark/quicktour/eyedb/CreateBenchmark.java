package org.eyedb.benchmark.quicktour.eyedb;

import java.util.Random;
import org.eyedb.benchmark.quicktour.eyedb.quicktour.*;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public class CreateBenchmark extends Benchmark {

    private static final Random random = new Random(System.currentTimeMillis());

    private static final String[] firstNames = {
	"Mike",
	"Oscar",
	"Joseph",
	"Jack",
	"Georges",
	"Suzan",
	"Sophie",
	"Margaret",
	"Paula",
	"Daniela"
    };

    private static String findFirstName()
    {
	return firstNames[ random.nextInt( firstNames.length)];
    }

    private Teacher[] fillTeachers( int nTeachers) throws org.eyedb.Exception
    {
	Teacher[] teachers = new Teacher[nTeachers];

	for ( int n = 0; n < nTeachers; n++)
	    {
		teachers[n] = new Teacher( database);
		teachers[n].setFirstName( CreateBenchmark.findFirstName());
		teachers[n].setLastName( "Teacher_"+n);
	    }

	return teachers;
    }

    private Course[] fillCourses( int nCourses, Teacher[] teachers) throws org.eyedb.Exception
    {
	Course courses[] = new Course[ nCourses];

	for ( int n = 0; n < nCourses; n++)
	    {
		courses[n] = new Course( database);
		courses[n].setTitle( "Course_"+n);
		courses[n].setDescription("Description of course "+n);
		courses[n].setTeacher( teachers[ random.nextInt( teachers.length)]);
	    }

	return courses;
    }

    public String getName()
    {
	return "Quicktour create";
    }

    public String getDescription()
    {
	return "Quicktour objects creation with one-to-many and many-to-many relations with referential integrity check";
    }

    public String getImplementation()
    {
	return "EyeDB Java implementation";
    }

    public void run()
    {
	int nStudents = getProperties().getIntProperty( "students");
	int nCourses = getProperties().getIntProperty( "courses");
	int nTeachers = getProperties().getIntProperty( "teachers");
	int nObjectsPerTransaction = getProperties().getIntProperty( "objects_per_transaction", 1000);

	getResult().addHeader( "objects");
	getResult().addHeader( "transaction");

	getResult().addValue( nStudents);
	getResult().addValue( nObjectsPerTransaction);

	try {
	    Teacher[] teachers = fillTeachers( nTeachers);
	    Course[] courses = fillCourses( nCourses, teachers);

	    for (long count = 0; count < nStudents; count += nObjectsPerTransaction) {
		database.transactionBegin();

		Student student = null;

		for ( int n = 0; n < nObjectsPerTransaction; n++)
		    {
			student = new Student( database);
			student.setFirstName( CreateBenchmark.findFirstName());
			student.setLastName( "Student_"+n);
			student.setBeginYear( (short)(random.nextInt( 3) + 1));

			int courseIndex = random.nextInt( courses.length);
			for ( int c = 0; c < courses.length/3; c++)
			    {
				student.addToCoursesColl( courses[ courseIndex]);
				courseIndex = (courseIndex + 719518367) % courses.length; // 719518367 is prime
			    }

			student.store(org.eyedb.RecMode.FullRecurs);
		    }

		database.transactionCommit();
	    }
	}
	catch( org.eyedb.Exception e) {
	    e.printStackTrace();
	    System.exit(1);
	}
	
	getStopWatch().lap( "total");

	getResult().addLaps( getStopWatch().getLaps());
	
	System.out.println( "org.eyedb.RPClib.read_ms = " + org.eyedb.RPClib.read_ms);
	System.out.println( "org.eyedb.RPClib.write_ms = " + org.eyedb.RPClib.write_ms);
	System.out.println( "org.eyedb.RPClib.write_async_ms = " + org.eyedb.RPClib.write_async_ms);
	System.out.println( "org.eyedb.RPClib.read_cnt = " + org.eyedb.RPClib.read_cnt);
	System.out.println( "org.eyedb.RPClib.read_async_cnt = " + org.eyedb.RPClib.read_async_cnt);
    }
}

