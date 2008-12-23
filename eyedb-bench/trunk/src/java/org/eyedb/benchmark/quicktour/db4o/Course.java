package org.eyedb.benchmark.quicktour.db4o;

import java.util.HashSet;
import java.util.Set;

public class Course {
	public Course()
	{
		title = "";
		description = "";
		students = new HashSet<Student>();
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

	Set<Student> getStudents()
	{
		return students;
	}

	public Teacher getTeacher()
	{
		return teacher;
	}

	public void setTeacher( Teacher teacher)
	{
		this.teacher = teacher;
	}

	private String title;
	private String description;
	private Set<Student> students;
	private Teacher teacher;
}
