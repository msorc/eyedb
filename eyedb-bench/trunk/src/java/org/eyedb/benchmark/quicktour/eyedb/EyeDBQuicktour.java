package org.eyedb.benchmark.quicktour.eyedb;

import org.eyedb.Database;
import org.eyedb.OQL;
import org.eyedb.ObjectArray;
import org.eyedb.Struct;
import org.eyedb.benchmark.quicktour.Quicktour;
import org.eyedb.benchmark.quicktour.eyedb.quicktour.inverse.Course;
import org.eyedb.benchmark.quicktour.eyedb.quicktour.inverse.Student;
import org.eyedb.benchmark.quicktour.eyedb.quicktour.inverse.Teacher;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public class EyeDBQuicktour extends Quicktour {

	public String getImplementation()
	{
		return "EyeDB Java implementation";
	}

	protected Database getDatabase()
	{
		return implementation.getDatabase();
	}

	public void prepare()
	{
		String databaseName = getProperties().getProperty( "eyedb.database");
		int tcpPort = getProperties().getIntProperty( "eyedb.tcp_port");

		implementation = new EyeDBImplementation( databaseName, tcpPort);
	}

	public void finish()
	{
		implementation.finish();
	}

	private Teacher[] fillTeachers( int nTeachers) throws org.eyedb.Exception
	{
		Teacher[] teachers = new Teacher[nTeachers];

		getDatabase().transactionBegin();

		for ( int n = 0; n < nTeachers; n++) {
			teachers[n] = new Teacher( getDatabase());
			teachers[n].setFirstName( "Teacher_"+n+"_firstName");
			teachers[n].setLastName( "Teacher_"+n);
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
				    student.addToCoursesColl( courses[ (i+c)%nCourses]);
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

	private EyeDBImplementation implementation;
}
