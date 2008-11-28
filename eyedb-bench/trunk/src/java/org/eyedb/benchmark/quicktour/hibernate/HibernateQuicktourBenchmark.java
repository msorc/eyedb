package org.eyedb.benchmark.quicktour.hibernate;

import org.eyedb.benchmark.quicktour.QuicktourBenchmark;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
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
			// Create the SessionFactory from hibernate.cfg.xml
			sessionFactory = new Configuration().configure().buildSessionFactory();
		} catch (Throwable ex) {
			System.err.println("Initial SessionFactory creation failed." + ex);
			ex.printStackTrace();
			System.exit(1);
		}
	}

	public void finish()
	{
		sessionFactory.close();
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

			Session session = sessionFactory.getCurrentSession();

			session.beginTransaction();

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

				session.save(student);
			}

			session.getTransaction().commit();
		}
	}

	public void query(int nSelects)
	{
	}

	public void remove()
	{
	}

	protected SessionFactory sessionFactory;
}

