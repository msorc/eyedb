package org.eyedb.benchmark.quicktour.hibernate;

import java.util.HashSet;
import java.util.Set;

public class Course {
	public Course()
	{
	    students = new HashSet<Student>();
	    teacher = null;
	}

	public Long getId()
	{
		return id;
	}

	private void setId(Long id)
	{
		this.id = id;
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

	public Set<Student> getStudents()
	{
		return students;
	}

	public void setStudents( Set<Student> students)
	{
		this.students = students;
	}

	public Teacher getTeacher()
	{
		return teacher;
	}

	public void setTeacher( Teacher teacher)
	{
		this.teacher = teacher;
	}

	public String toString()
	{
		return "Course( id:" + getId() + ", title:\"" + getTitle() + "\", description:\"" + getDescription() + "\")";
	}

	private Long id;
	private String title;
	private String description;
	private Set<Student> students;
	private Teacher teacher;
}
