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

	public void prepareClient()
	{
		String host = getProperties().getProperty("db4o.host");
		int port = getProperties().getIntProperty("db4o.port");
		String user = getProperties().getProperty("db4o.user");
		String passwd = getProperties().getProperty("db4o.passwd");

		try {
			container = Db4o.openClient( host, port, user, passwd);
		}
		catch( Exception e) {
			e.printStackTrace();
			System.exit( 1);
		}
	}

	public void prepare()
	{
		String filename = getProperties().getProperty( "db4o.filename");

		container = Db4o.openFile( filename);		
	}

	public void finish()
	{
		container.close();
	}

	private Teacher[] fillTeachers( int nTeachers)
	{
		Teacher[] teachers = new Teacher[nTeachers];

		for ( int n = 0; n < nTeachers; n++) {
			Teacher t = new Teacher();
			t.setFirstName( "Teacher_"+n+"_firstName");
			t.setLastName( "Teacher_"+n);
			teachers[n] = t;
		}

		return teachers;
	}

	private Course[] fillCourses( int nCourses, Teacher[] teachers)
	{
		Course courses[] = new Course[ nCourses];

		for ( int n = 0; n < nCourses; n++) {
			Course c = new Course();
			c.setTitle( "Course_"+n);
			c.setDescription( "Description of course "+n);
			courses[n].setTeacher( teachers[ getRandom().nextInt( teachers.length)]);
			courses[n] = c;
		}

		return courses;
	}


	public void create(int nStudents, int nCourses, int nTeachers, int nObjectsPerTransaction)
	{
		Teacher[] teachers = fillTeachers( nTeachers);
		Course[] courses = fillCourses( nCourses, teachers);

		for (int n = 0; n < nStudents; n++ ) {
			Student student = new Student();
			student.setFirstName("Student_"+n+"_firstName");
			student.setLastName("Student_"+n);
			//		    student.setBeginYear( (short)(random.nextInt( 3) + 1));

			for ( int c = 0; c < courses.length; c++) {
				int i = getRandom().nextInt( courses.length);
				student.getCourses().add( courses[i]);
			}

			container.store( student);

		}

	}

	public void query(int nSelects)
	{
	}

	public void remove()
	{
	}

	private ObjectContainer container;
}

