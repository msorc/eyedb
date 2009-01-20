package org.eyedb.benchmark.quicktour;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

import org.eyedb.benchmark.framework.Benchmark;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public abstract class QuicktourQuery extends Benchmark {

	public QuicktourQuery()
	{
	}

	public String getName()
	{
		return "Quicktour";
	}

	public String getDescription()
	{
		return "Quicktour benchmark, query objects with one-level inheritance, one-to-many and many-to-many relations with referential integrity check";
	}

	public abstract void query( int nSelects);

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

	getResult().addHeader( "students");
	getResult().addHeader( "courses");
	getResult().addHeader( "teachers");
	getResult().addHeader( "objectsPerTransaction");
	getResult().addHeader( "selects");
	getResult().addHeader( "query");

		for ( int i = 0; i < students.size(); i++) {
			int nStudents = students.get(i).intValue();
			int nCourses = courses.get(i).intValue();
			int nTeachers = teachers.get(i).intValue();
			int nObjectsPerTransaction = objectsPerTransaction.get(i).intValue();
			int nSelects = selects.get(i).intValue();

			getResult().add( nStudents);
			getResult().add( nCourses);
			getResult().add( nTeachers);
			getResult().add( nObjectsPerTransaction);
			getResult().add( nSelects);

			getStopWatch().start();

			query( nSelects);
			getStopWatch().lap( "query");

			getResult().add( getStopWatch().getLaps());

			getStopWatch().reset();

			getResult().next();
		}
	}
}
