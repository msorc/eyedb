package org.eyedb.benchmark.quicktour.eyedb;

import org.eyedb.Database;
import org.eyedb.OQL;
import org.eyedb.ObjectArray;
import org.eyedb.Struct;
import org.eyedb.benchmark.quicktour.QuicktourBenchmark;
import org.eyedb.benchmark.quicktour.eyedb.quicktour.Course;
import org.eyedb.benchmark.quicktour.eyedb.quicktour.Student;
import org.eyedb.benchmark.quicktour.eyedb.quicktour.Teacher;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public class EyeDBQuicktourBenchmark extends QuicktourBenchmark {

	public String getImplementation()
	{
		return "EyeDB Java implementation";
	}

	protected Database getDatabase()
	{
		return database;
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

		database.transactionBegin();

		for ( int n = 0; n < nTeachers; n++) {
			teachers[n] = new Teacher( database);
			teachers[n].setFirstName( "Teacher_"+n+"_firstName");
			teachers[n].setLastName( "Teacher_"+n);
		}

		database.transactionCommit();

		return teachers;
	}

	private Course[] fillCourses( int nCourses, Teacher[] teachers) throws org.eyedb.Exception
	{
		Course courses[] = new Course[ nCourses];

		database.transactionBegin();

		for ( int n = 0; n < nCourses; n++) {
			courses[n] = new Course( database);
			courses[n].setTitle( "Course_"+n);
			courses[n].setDescription("Description of course "+n);
			courses[n].setTeacher( teachers[ getRandom().nextInt( teachers.length)]);
		}

		database.transactionCommit();

		return courses;
	}

	public void create( int nStudents, int nCourses, int nTeachers, int nObjectsPerTransaction)
	{
		try {
			Teacher[] teachers = fillTeachers( nTeachers);
			Course[] courses = fillCourses( nCourses, teachers);

			database.transactionBegin();

			for ( int n = 0; n < nStudents; n++) {
				Student student = new Student( database);
				student.setFirstName( "Student_"+n+"_firstName");
				student.setLastName( "Student_"+n);
				student.setBeginYear( (short)(getRandom().nextInt( 3) + 1));

				for ( int c = 0; c < courses.length; c++) {
				    int i = getRandom().nextInt( courses.length);
				    student.addToCoursesColl( courses[ i]);
				}

				student.store(org.eyedb.RecMode.FullRecurs);

				if (n % nObjectsPerTransaction == nObjectsPerTransaction - 1) {
					database.transactionCommit();
					database.transactionBegin();
				}
			}

			database.transactionCommit();
		}
		catch( org.eyedb.Exception e) {
			e.printStackTrace();
			System.exit(1);
		}
	}

	private void queryByClass( String className, int nSelects)
	{
		try {
			getDatabase().transactionBegin();

			for ( int n = 0;  n < nSelects; n++) {
				String q = "select x from " + className + " as x where x.lastName ~ \"" + n + "\"";
				OQL query = new OQL( getDatabase(), q);

				ObjectArray result = new ObjectArray();
				query.execute( result);

				int count = result.getCount();
				Object[] objects = result.getObjects();

				for (int i = 0; i < count; i++) {
					Struct x = (Struct)objects[i];
				}
			}

			getDatabase().transactionCommit();
		}
		catch( org.eyedb.Exception e) {
			e.printStackTrace();
		}
	}

	public void query( int nSelects)
	{
		queryByClass( "Teacher", nSelects);
		queryByClass( "Student", nSelects);
	}

	private void removeByClass( String className)
	{
		try {
			getDatabase().transactionBegin();

			String q = "select x from " + className + " as x";
			OQL query = new OQL( getDatabase(), q);

			ObjectArray result = new ObjectArray();
			query.execute( result);

			int count = result.getCount();
			Object[] objects = result.getObjects();

			for (int i = 0; i < count; i++) {
				Struct x = (Struct)objects[i];
				x.remove();
			}

			getDatabase().transactionCommit();
		}
		catch( org.eyedb.Exception e) {
			e.printStackTrace();
		}
	}

	public void remove()
	{
		removeByClass( "Teacher");
		removeByClass( "Course");
		removeByClass( "Student");
	}

	protected org.eyedb.Connection connection;
	protected org.eyedb.benchmark.quicktour.eyedb.quicktour.Database database;
}
