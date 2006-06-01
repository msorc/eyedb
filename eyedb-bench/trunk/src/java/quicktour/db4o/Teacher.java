import java.util.*;

public class Teacher extends Person {
    public Teacher( String firstName, String lastName)
    {
	super( firstName, lastName);

	courses = new HashSet();
    }

    public void addCourse( Course c)
    {
	courses.add( c);

	c.setTeacher( this);
    }

    public void removeCourse( Course c)
    {
	courses.remove( c);

	c.setTeacher( null);
    }

    private Set courses;
}

