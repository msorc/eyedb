package org.eyedb.benchmark.quicktour.hibernate;

import java.util.HashSet;
import java.util.Set;

public class Student extends Person {

    public Student()
    {
	courses = new HashSet<Course>();
    }

    public short getBeginYear()
    {
        return beginYear;
    }

    public void setBeginYear(short beginYear)
    {
        this.beginYear = beginYear;
    }

    public Set<Course> getCourses()
    {
	return courses;
    }

    public void setCourses( Set<Course> courses)
    {
	this.courses = courses;
    }

    private short beginYear;
    private Set<Course> courses;
}

