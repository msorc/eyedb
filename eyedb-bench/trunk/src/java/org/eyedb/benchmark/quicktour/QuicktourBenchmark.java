package org.eyedb.benchmark.quicktour;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

import org.eyedb.benchmark.framework.Benchmark;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public abstract class QuicktourBenchmark extends Benchmark {

	public QuicktourBenchmark()
	{
		random = new Random(System.currentTimeMillis());
	}

	public String getName()
	{
		return "Quicktour";
	}

	public String getDescription()
	{
		return "Quicktour benchmark, create, query and delete objects with one-level inheritance, one-to-many and many-to-many relations with referential integrity check";
	}

	public abstract void create( int nStudents, int nCourses, int nTeachers, int nObjectsPerTransaction);

	public abstract void query( int nSelects);

	public abstract void remove();

	public void run()
	{
		List<Integer> students = new ArrayList<Integer>();
		List<Integer> courses = new ArrayList<Integer>();
		List<Integer> teachers = new ArrayList<Integer>();
		List<Integer> objectsPerTransaction = new ArrayList<Integer>();
		List<Integer> selects = new ArrayList<Integer>();

		getProperties().getIntProperty( "quicktour.students", students);
		getProperties().getIntProperty( "quicktour.courses", courses);
		getProperties().getIntProperty( "quicktour.teachers", teachers);
		getProperties().getIntProperty( "quicktour.objects_per_transaction", objectsPerTransaction);
		getProperties().getIntProperty( "quicktour.selects", selects);

		for ( int i = 0; i < students.size(); i++) {
			int nStudents = students.get(i).intValue();
			int nCourses = courses.get(i).intValue();
			int nTeachers = teachers.get(i).intValue();
			int nObjectsPerTransaction = objectsPerTransaction.get(i).intValue();
			int nSelects = selects.get(i).intValue();

			getResult().add( "students", nStudents);
			getResult().add( "courses", nCourses);
			getResult().add( "teachers", nTeachers);
			getResult().add( "objectsPerTransaction", nObjectsPerTransaction);
			getResult().add( "selects", nSelects);

			getStopWatch().start();

			create( nStudents, nCourses, nTeachers, nObjectsPerTransaction);
			getStopWatch().lap( "create");

			query( nSelects);
			getStopWatch().lap( "query");

			remove();
			getStopWatch().lap( "remove");

			getResult().add( getStopWatch().getLaps());

			getStopWatch().reset();

			getResult().next();
		}
	}

	protected Random getRandom()
	{
		return random;
	}

	private Random random;
}
