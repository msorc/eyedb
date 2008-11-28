package org.eyedb.benchmark.quicktour.eyedb;

import org.eyedb.benchmark.quicktour.QuicktourBenchmark;
import org.eyedb.benchmark.quicktour.eyedb.quicktour.Course;
import org.eyedb.benchmark.quicktour.eyedb.quicktour.Student;
import org.eyedb.benchmark.quicktour.eyedb.quicktour.Teacher;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public abstract class EyeDBQuicktourBenchmark extends QuicktourBenchmark {

	public String getImplementation()
	{
		return "EyeDB Java implementation";
	}

	public void prepare()
	{
		String databaseName = getProperties().getProperty( "database");

		String[] args = new String[3];
		int i = 0;
		args[i++] = "--user=" + System.getProperty( "user.name");
		args[i++] = "--dbm=default";
		args[i++] = "--port=" + getProperties().getProperty( "tcp_port");

		org.eyedb.Root.init(databaseName, args);

		try {
			// Initialize the package
			org.eyedb.benchmark.quicktour.eyedb.quicktour.Database.init();

			// Open the connection with the backend
			connection = new org.eyedb.Connection();

			// Open the database
			database = new org.eyedb.benchmark.quicktour.eyedb.quicktour.Database( databaseName);
			database.open(connection, org.eyedb.Database.DBRW);
		}
		catch(org.eyedb.Exception e) { // Catch any eyedb exception
			e.printStackTrace();
			System.exit(1);
		}
	}

	public void finish()
	{
/*		System.out.println( "org.eyedb.RPClib.read_ms = " + org.eyedb.RPClib.read_ms);
		System.out.println( "org.eyedb.RPClib.write_ms = " + org.eyedb.RPClib.write_ms);
		System.out.println( "org.eyedb.RPClib.write_async_ms = " + org.eyedb.RPClib.write_async_ms);
		System.out.println( "org.eyedb.RPClib.read_cnt = " + org.eyedb.RPClib.read_cnt);
		System.out.println( "org.eyedb.RPClib.read_async_cnt = " + org.eyedb.RPClib.read_async_cnt);
*/
		try {
			// Close the database
			database.close();

			// Close the connection
			connection.close();
		}
		catch(org.eyedb.Exception e) { // Catch any eyedb exception
			e.printStackTrace();
			System.exit(1);
		}
	}

	private Teacher[] fillTeachers( int nTeachers) throws org.eyedb.Exception
	{
		Teacher[] teachers = new Teacher[nTeachers];

		for ( int n = 0; n < nTeachers; n++)
		{
			teachers[n] = new Teacher( database);
			teachers[n].setFirstName( "Teacher_"+n+"_firstName");
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
			courses[n].setTeacher( teachers[ getRandom().nextInt( teachers.length)]);
		}

		return courses;
	}

	public void create( int nStudents, int nCourses, int nTeachers, int nObjectsPerTransaction)
	{

		try {
			Teacher[] teachers = fillTeachers( nTeachers);
			Course[] courses = fillCourses( nCourses, teachers);

			for (long count = 0; count < nStudents; count += nObjectsPerTransaction) {
				database.transactionBegin();

				Student student = null;

				for ( int n = 0; n < nObjectsPerTransaction; n++)
				{
					student = new Student( database);
					student.setFirstName( "Student_"+n+"_firstName");
					student.setLastName( "Student_"+n);
					student.setBeginYear( (short)(getRandom().nextInt( 3) + 1));

					int courseIndex = getRandom().nextInt( courses.length);
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
	}

	public void query( int nSelects)
	{
	}

	public void remove()
	{
	}

	protected org.eyedb.Connection connection;
	protected org.eyedb.benchmark.quicktour.eyedb.quicktour.Database database;
}
