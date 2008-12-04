package org.eyedb.benchmark.quicktour.hibernate;

import java.util.HashSet;
import java.util.Set;

public class Student extends Person {

	public Student()
	{
		courses = new HashSet<Course>();
	}

	public Set<Course> getCourses()
	{
		return courses;
	}

	public void setCourses( Set<Course> courses)
	{
		this.courses = courses;
	}

	private Set<Course> courses;
}

