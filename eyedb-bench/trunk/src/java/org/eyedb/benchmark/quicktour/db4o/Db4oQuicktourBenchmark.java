package org.eyedb.benchmark.quicktour.db4o;

import org.eyedb.benchmark.quicktour.QuicktourBenchmark;
import com.db4o.Db4o;
import com.db4o.ObjectContainer;
import com.db4o.ObjectSet;
import com.db4o.query.Predicate;
import com.db4o.query.Query;

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
		String filename = getProperties().getProperty( "db4o.file");

		container = Db4o.openFile( filename);		

		Db4o.newConfiguration().objectClass( Student.class ).objectField( "lastName" ).indexed( true );
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

			container.store( t);
		}

		container.commit();

		return teachers;
	}

	private Course[] fillCourses( int nCourses, Teacher[] teachers)
	{
		Course courses[] = new Course[ nCourses];

		for ( int n = 0; n < nCourses; n++) {
			Course c = new Course();
			c.setTitle( "Course_"+n);
			c.setDescription( "Description of course "+n);
			c.setTeacher( teachers[ getRandom().nextInt( teachers.length)]);
			courses[n] = c;

			container.store( c);
		}

		container.commit();

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

			if (n % nObjectsPerTransaction == nObjectsPerTransaction - 1) {
				container.commit();
			}
		}

		container.commit();

	}

	private static class StudentFind extends Predicate<Student> {
		StudentFind( String pattern)
		{
			this.pattern = pattern;
		}

		public boolean match( Student p)
		{
			return p.getLastName().matches( pattern);
		}

		private String pattern;
	}

	private void queryStudentsNative( int nSelects)
	{
		for ( int n = 0;  n < nSelects; n++) {
			StudentFind f = new StudentFind( ""+n);
			for (Student student : container.query( f)) {
				System.out.println( student);
			}
		}

		container.commit();
	}

	private void queryStudentsSODA( int nSelects)
	{
		int count = 0;

		for ( int n = 0;  n < nSelects; n++) {
			Query q = container.query();
			q.constrain( Student.class );
			q.descend( "lastName" ).constrain("Student_" + n);

			ObjectSet result = q.execute();
			count = 0;
			while (result.hasNext()) {
				Student s = (Student)result.next();
				count++;
			}
		}

		System.out.println( "Retrieved " + count + " students");

		container.commit();
	}


	public void query(int nSelects)
	{
		//	queryStudentsNative( nSelects);
		queryStudentsSODA( nSelects);
	}

	private void removeByClass( Class cl)
	{
		ObjectSet result = container.queryByExample( cl);

		while(result.hasNext()) {
			container.delete( result.next());
		}

		container.commit();
	}

	public void remove()
	{
		removeByClass( Teacher.class);
		removeByClass( Course.class);
		removeByClass( Student.class);
	}

	private ObjectContainer container;
}

