package org.eyedb.benchmark.quicktour.hibernate;

import java.util.Iterator;

import org.eyedb.benchmark.quicktour.QuicktourBenchmark;
import org.hibernate.HibernateException;
import org.hibernate.Query;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.Transaction;
import org.hibernate.cfg.Configuration;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public class HibernateQuicktourBenchmark extends QuicktourBenchmark {

	public String getImplementation()
	{
		return "Hibernate Java implementation";
	}

	public void prepare()
	{
		try {
			Configuration configuration = new Configuration();
			configuration.configure();

			String url = getProperties().getProperty( "hibernate.connection.url");
			configuration.setProperty( "connection.url", url);
			String username = getProperties().getProperty( "hibernate.connection.username");
			configuration.setProperty( "connection.username", username);
			String password = getProperties().getProperty( "hibernate.connection.password");
			configuration.setProperty( "connection.password", password);

			sessionFactory = configuration.buildSessionFactory();

			session = sessionFactory.openSession();

		} catch (Throwable ex) {
			System.err.println("Initial SessionFactory creation failed." + ex);
			ex.printStackTrace();
			System.exit(1);
		}
	}

	public void finish()
	{
		session.close();
		sessionFactory.close();
	}

	protected Session getSession()
	{
		return session;
	}

	private Teacher[] fillTeachers( int nTeachers)
	{
		Teacher[] teachers = new Teacher[nTeachers];

		for ( int n = 0; n < nTeachers; n++) {
			teachers[n] = new Teacher( "Teacher_"+n+"_firstName", "Teacher_"+n);
		}

		return teachers;
	}

	private Course[] fillCourses( int nCourses, Teacher[] teachers)
	{
		Course courses[] = new Course[ nCourses];

		for ( int n = 0; n < nCourses; n++) {
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

			Transaction tx = getSession().beginTransaction();

			for ( int n = 0; n < nObjectsPerTransaction; n++) {
				student = new Student( "Student_"+n+"_firstName", "Student_"+n);
				//		    student.setBeginYear( (short)(random.nextInt( 3) + 1));

				int courseIndex = getRandom().nextInt( courses.length);
				for ( int c = 0; c < courses.length/3; c++) {
					student.addCourse( courses[ courseIndex]);
					courseIndex = (courseIndex + 719518367) % courses.length; // 719518367 is prime
				}

				getSession().save(student);
			}

			tx.commit();
		}
	}

	private void queryByClass( String className, int nSelects)
	{
		try {
			Transaction tx = getSession().beginTransaction();

			for ( int n = 0;  n < nSelects; n++) {
				String s = "from " + className + " as x where x.lastName like '%" + n + "%'";
				Query q = getSession().createQuery( s);

				int count = 0;
				Iterator it = q.iterate();
				while(it.hasNext()) 
					count++;
			}

			tx.commit();
		}
		catch( HibernateException e) {
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
			Transaction tx = getSession().beginTransaction();

			String s = "from " + className;
			Query q = getSession().createQuery( s);

			Iterator it = q.iterate();
			while(it.hasNext()) 
				getSession().delete( it.next());

			tx.commit();
		}
		catch( HibernateException e) {
			e.printStackTrace();
		}
	}

	public void remove()
	{
		removeByClass( "Teacher");
		removeByClass( "Course");
		removeByClass( "Student");
	}

	private SessionFactory sessionFactory;
	private Session session;
}

