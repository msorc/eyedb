package org.eyedb.benchmark.quicktour.hibernate;

import java.util.*;

public class Course {
    public Course()
    {
	this( "", "");
    }

    public Course( String title, String description)
    {
	this.title = title;
	this.description = description;

	students = new HashSet();
    }

    public String getTitle()
    {
	return title;
    }

    public void setTitle( String title)
    {
	this.title = title;
    }

    public String getDescription()
    {
	return description;
    }

    public void setDescription( String description)
    {
	this.description = description;
    }

    Set getStudents()
    {
	return students;
    }

    void setStudents( Set students)
    {
	this.students = students;
    }

    public void addStudent( Student student)
    {
	students.add( student);
	student.getCourses().add( this);
    }

    public void removeStudent( Student student)
    {
	students.remove( student);
	student.getCourses().remove( this);
    }

    public Teacher getTeacher()
    {
	return teacher;
    }

    public void setTeacher( Teacher teacher)
    {
	if (this.teacher != null)
	    this.teacher.getCourses().remove(this);
	this.teacher = teacher;
	teacher.getCourses().add( this);
    }

    public Long getId() {
        return id;
    }

    private void setId(Long id) {
        this.id = id;
    }

    private Long id;

    private String title;
    private String description;
    private Set students;
    private Teacher teacher;
}
