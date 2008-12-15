package org.eyedb.benchmark.quicktour.hibernate;

import java.io.File;
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

	private void hack( Configuration cfg)
	{
		cfg.setProperty("hibernate.query.substitutions", "true 1, false 0, yes 'Y', no 'N'");
		cfg.setProperty("hibernate.connection.pool_size", "1");
		cfg.setProperty("hibernate.proxool.pool_alias", "pool1");
		cfg.setProperty("hibernate.jdbc.batch_size", "0");
		cfg.setProperty("hibernate.jdbc.batch_versioned_data", "true");
		cfg.setProperty("hibernate.jdbc.use_streams_for_binary", "true");
		cfg.setProperty("hibernate.max_fetch_depth", "1");
//		cfg.setProperty("hibernate.cache.region_prefix", "hibernate.test");
		cfg.setProperty("hibernate.cache.use_query_cache", "true");
//		cfg.setProperty("hibernate.cache.provider_class", "net.sf.hibernate.cache.EhCacheProvider");

		cfg.setProperty("hibernate.proxool.pool_alias", "pool1");
		cfg.setProperty("hibernate.proxool.pool_alias", "pool1");
	}

	public void prepare()
	{
		try {
			Configuration configuration = new Configuration();
			configuration.configure( new File("hibernate.cfg.xml"));

			String url = getProperties().getProperty( "hibernate.connection.url");
			configuration.setProperty( "connection.url", url);
			String username = getProperties().getProperty( "hibernate.connection.username");
			configuration.setProperty( "connection.username", username);
			String password = getProperties().getProperty( "hibernate.connection.password");
			configuration.setProperty( "connection.password", password);

			//hack( configuration);

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

		getSession().beginTransaction();

		for ( int n = 0; n < nTeachers; n++) {
			Teacher t = new Teacher();
			t.setFirstName("Teacher_"+n+"_firstName");
			t.setLastName("Teacher_"+n);
			teachers[n] = t;

			getSession().save( t);
		}

		getSession().getTransaction().commit();

		return teachers;
	}

	private Course[] fillCourses( int nCourses, Teacher[] teachers)
	{
		Course courses[] = new Course[ nCourses];

		getSession().beginTransaction();

		for ( int n = 0; n < nCourses; n++) {
			Course c = new Course();
			c.setTitle("Course_"+n);
			c.setDescription("Description of course "+n);
			c.setTeacher( teachers[ getRandom().nextInt( teachers.length)]);
			courses[n] = c;

			getSession().save( c);
		}

		getSession().getTransaction().commit();

		return courses;
	}

	public void create(int nStudents, int nCourses, int nTeachers, int nObjectsPerTransaction)
	{
		Teacher[] teachers = fillTeachers( nTeachers);
		Course[] courses = fillCourses( nCourses, teachers);

		getSession().beginTransaction();

		for (int n = 0; n < nStudents; n++ ) {
			Student student = new Student();
			student.setFirstName("Student_"+n+"_firstName");
			student.setLastName("Student_"+n);
			//		    student.setBeginYear( (short)(random.nextInt( 3) + 1));

			for ( int c = 0; c < courses.length; c++) {
				int i = getRandom().nextInt( courses.length);
				student.getCourses().add( courses[i]);
			}

			getSession().save(student);

			if (n % nObjectsPerTransaction == nObjectsPerTransaction - 1) {
				getSession().getTransaction().commit();
				getSession().beginTransaction();
			}
		}

		getSession().getTransaction().commit();

		getSession().flush();
	}

	private void queryByClass( String className, int nSelects)
	{
		try {
			Transaction tx = getSession().beginTransaction();

			for ( int n = 0;  n < nSelects; n++) {
				String s = "from " + className + " as x where x.lastName like ?";
				Query q = getSession().createQuery( s);
				q.setString( 0, ""+n);

				for (Object o: q.list()) {
					Teacher t = (Teacher)o;

					System.out.println( t);
					for ( Course c: t.getCourses())
						System.out.println( c);
				}
//				int count = 0;
//				Iterator it = q.iterate();
//				while(it.hasNext()) 
//				count++;
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
	private Transaction transaction;
}

