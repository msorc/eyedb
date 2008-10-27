package org.eyedb.benchmark.quicktour.db4o;

import com.db4o.*;
import com.db4o.messaging.*;
import org.eyedb.benchmark.*;
import java.io.*;
import java.util.Random;

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

    private Teacher[] fillTeachers( int nTeachers)
    {
	Teacher[] teachers = new Teacher[nTeachers];

	for ( int n = 0; n < nTeachers; n++)
	    {
		teachers[n] = new Teacher( CreateBenchmark.findFirstName(), "Teacher_"+n);
	    }

	return teachers;
    }

    private Course[] fillCourses( int nCourses, Teacher[] teachers)
    {
	Course courses[] = new Course[ nCourses];

	for ( int n = 0; n < nCourses; n++)
	    {
		courses[n] = new Course( "Course_"+n, "Description of course "+n);
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

    public String getRunDescription()
    {
	return "db4o implementation";
    }

    public void run()
    {
	int nStudents = getProperties().getIntProperty( "students");
	int nCourses = getProperties().getIntProperty( "courses");
	int nTeachers = getProperties().getIntProperty( "teachers");
	int nObjectsPerTransaction = getProperties().getIntProperty( "objects_per_transaction", 1000);

	Teacher[] teachers = fillTeachers( nTeachers);
	Course[] courses = fillCourses( nCourses, teachers);

	for (long count = 0; count < nStudents; count += nObjectsPerTransaction) {
	    Student student = null;

	    for ( int n = 0; n < nObjectsPerTransaction; n++)
		{
		    student = new Student( CreateBenchmark.findFirstName(), "Student_"+n);
		    //		    student.setBeginYear( (short)(random.nextInt( 3) + 1));

		    int courseIndex = random.nextInt( courses.length);
		    for ( int c = 0; c < courses.length/3; c++)
			{
			    student.addCourse( courses[ courseIndex]);
			    courseIndex = (courseIndex + 719518367) % courses.length; // 719518367 is prime
			}

		    client.set( student);
		}

	    client.commit();
	}
    }
}

