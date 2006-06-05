import java.util.*;

public class Teacher extends Person {

    public Teacher()
    {
	this( "", "");
    }

    public Teacher( String firstName, String lastName)
    {
	super( firstName, lastName);

	courses = new HashSet();
    }

    Set getCourses()
    {
	return courses;
    }

    void setCourses( Set courses)
    {
	this.courses = courses;
    }

    private Set courses;
}

