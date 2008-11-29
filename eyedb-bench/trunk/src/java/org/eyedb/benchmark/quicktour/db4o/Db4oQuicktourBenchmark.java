package org.eyedb.benchmark.quicktour.db4o;

import org.eyedb.benchmark.quicktour.QuicktourBenchmark;

import com.db4o.Db4o;
import com.db4o.ObjectContainer;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public class Db4oQuicktourBenchmark extends QuicktourBenchmark {

	public String getImplementation()
	{
		return "db4o implementation";
	}

	public void prepare()
	{
		String host = getProperties().getProperty("host");
		int port = getProperties().getIntProperty("port");
		String user = getProperties().getProperty("user");
		String password = getProperties().getProperty("pass");

		try {
			client = Db4o.openClient( host, port, user, password);
		}
		catch( Exception e) {
			e.printStackTrace();
			System.exit( 1);
		}
	}

	public void finish()
	{
		client.close();
	}

	private Teacher[] fillTeachers( int nTeachers)
	{
		Teacher[] teachers = new Teacher[nTeachers];

		for ( int n = 0; n < nTeachers; n++)
		{
			teachers[n] = new Teacher( "Teacher_"+n+"_firstName", "Teacher_"+n);
		}

		return teachers;
	}

	private Course[] fillCourses( int nCourses, Teacher[] teachers)
	{
		Course courses[] = new Course[ nCourses];

		for ( int n = 0; n < nCourses; n++)
		{
			courses[n] = new Course( "Course_"+n, "Description of course "+n);
			courses[n].setTeacher( teachers[ getRandom().nextInt( teachers.length)]);
		}

		return courses;
	}


	public void create(int nStudents, int nCourses, int nTeachers, int nObjectsPerTransaction)
	{
		Teacher[] teachers = fillTeachers( nTeachers);
		Course[] courses = fillCourses( nCourses, teachers);

		for (long count = 0; count < nStudents; count += nObjectsPerTransaction) {
			Student student = null;

			for ( int n = 0; n < nObjectsPerTransaction; n++)
			{
				student = new Student( "Student_"+n+"_firstName", "Student_"+n);
				//		    student.setBeginYear( (short)(random.nextInt( 3) + 1));

				int courseIndex = getRandom().nextInt( courses.length);
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

	public void query(int nSelects)
	{
	}

	public void remove()
	{
	}

	protected ObjectContainer client;
}

