package org.eyedb.benchmark.quicktour.db4o;

import java.util.HashSet;
import java.util.Set;

public class Course {
	public Course()
	{
		title = "";
		description = "";
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

	private String title;
	private String description;
	private Set students;
	private Teacher teacher;
}
