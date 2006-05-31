import java.util.Random;
import quicktour.*;

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

    private static Teacher[] fillTeachers( quicktour.Database database, int nTeachers) throws org.eyedb.Exception
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

    private static Course[] fillCourses( quicktour.Database database, int nCourses, Teacher[] teachers) throws org.eyedb.Exception
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

    public void run()
    {
	int nStudents = getProperties().getIntProperty( "students");
	int nCourses = getProperties().getIntProperty( "courses");
	int nTeachers = getProperties().getIntProperty( "teachers");
	int nObjectsPerTransaction = getProperties().getIntProperty( "objects_per_transaction", 1000);

	try {
	    Teacher[] teachers = fillTeachers( database, nTeachers);
	    Course[] courses = fillCourses( database, nCourses, teachers);

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

		System.out.println( "Inserted " + count + " Students: " + student);

		database.transactionCommit();
	    }
	}
	catch( org.eyedb.Exception e) {
	    e.printStackTrace();
	    System.exit(1);
	}

	System.out.println( "org.eyedb.RPClib.read_ms = " + org.eyedb.RPClib.read_ms);
	System.out.println( "org.eyedb.RPClib.write_ms = " + org.eyedb.RPClib.write_ms);
	System.out.println( "org.eyedb.RPClib.write_async_ms = " + org.eyedb.RPClib.write_async_ms);
	System.out.println( "org.eyedb.RPClib.read_cnt = " + org.eyedb.RPClib.read_cnt);
	System.out.println( "org.eyedb.RPClib.read_async_cnt = " + org.eyedb.RPClib.read_async_cnt);
    }
}

