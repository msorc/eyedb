import java.util.*;

public class Student extends Person {
    public Student( String firstName, String lastName)
    {
	super( firstName, lastName);

	courses = new HashSet();
    }

    public void addCourse( Course c)
    {
	if (courses.contains( c))
	    return;

	courses.add( c);
	c.addStudent( this);
    }

    public void removeCourse( Course c)
    {
	if (!courses.contains( c))
	    return;

	courses.remove( c);
	c.removeStudent( this);
    }

    private Set courses;
}

