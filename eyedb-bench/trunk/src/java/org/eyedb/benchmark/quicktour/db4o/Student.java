package org.eyedb.benchmark.quicktour.db4o;

import java.util.HashSet;
import java.util.Set;

public class Student extends Person {
	public Student()
	{
		courses = new HashSet<Course>();
	}

	Set<Course> getCourses()
	{
		return courses;
	}

	private Set<Course> courses;
}

