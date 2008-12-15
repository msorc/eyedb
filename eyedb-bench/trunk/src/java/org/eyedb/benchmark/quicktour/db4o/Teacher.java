package org.eyedb.benchmark.quicktour.db4o;

import java.util.HashSet;
import java.util.Set;

public class Teacher extends Person {
	public Teacher()
	{
		courses = new HashSet();
	}

	Set getCourses()
	{
		return courses;
	}

	private Set courses;
}

