package org.eyedb.benchmark.quicktour.db4o;

import java.util.*;

public class Teacher extends Person {
    public Teacher( String firstName, String lastName)
    {
	super( firstName, lastName);

	courses = new HashSet();
    }

    Set getCourses()
    {
	return courses;
    }

    private Set courses;
}

