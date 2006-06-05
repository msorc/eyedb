public class Person {
    public Person()
    {
	this( "", "");
    }

    public Person( String firstName, String lastName)
    {
	this.firstName = firstName;
	this.lastName = lastName;
    }

    public String getFirstName()
    {
	return firstName;
    }

    public void setFirstName( String firstName)
    {
	this.firstName = firstName;
    }

    public String getLastName()
    {
	return lastName;
    }

    public void setLastName( String lastName)
    {
	this.lastName = lastName;
    }

    public Long getId() {
        return id;
    }

    private void setId(Long id) {
        this.id = id;
    }

    private Long id;

    private String firstName;
    private String lastName;
}
