import java.util.*;

public class Course {
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

    public void addStudent( Student s)
    {
	if (students.contains( s))
	    return;

	students.add( s);
	s.addCourse( this);
    }

    public void removeStudent( Student s)
    {
	if (!students.contains( s))
	    return;

	students.remove( s);
	s.removeCourse( this);
    }

    public Teacher getTeacher()
    {
	return teacher;
    }

    public void setTeacher( Teacher teacher)
    {
	if (this.teacher == teacher)
	    return;

	if (this.teacher != null)
	    this.teacher.removeCourse( this);

	this.teacher = teacher;

	if (teacher != null)
	    teacher.addCourse( this);
    }

    private String title;
    private String description;
    private Set students;
    private Teacher teacher;
}
