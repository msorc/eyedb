package org.eyedb.benchmark.quicktour.db4o;

import java.util.HashSet;
import java.util.Set;

public class Student extends Person {
	public Student()
	{
		courses = new HashSet();
	}

	Set getCourses()
	{
		return courses;
	}

	public void addCourse( Course course)
	{
		courses.add( course);
		course.getStudents().add( this);
	}

	public void removeCourse( Course course)
	{
		courses.remove( course);
		course.getStudents().remove( this);
	}

	private Set courses;
}

