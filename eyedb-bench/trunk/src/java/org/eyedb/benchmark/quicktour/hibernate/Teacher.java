package org.eyedb.benchmark.quicktour.hibernate;

import java.util.*;

public class Teacher extends Person {

    public Teacher()
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

    public String toString()
    {
	return "Teacher( id:" + getId() + ", firstname:\"" + getFirstName() + "\", name:\"" + getFirstName() + "\")";
    }

    private Set<Course> courses;
}

