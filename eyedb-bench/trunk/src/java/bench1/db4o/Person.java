public class Person {

    public Person()
    {
	this.name = "unknown";
	this.age = 0;
    }

    public Person( String name, int age)
    {
	this.name = name;
	this.age = age;
    }

    public String getName()
    {
	return name;
    }

    public void setName( String name)
    {
	this.name = name;
    }

    public int getAge()
    {
	return age;
    }

    public void setAge( int age)
    {
	this.age = age;
    }

    public String toString()
    {
	return name + "(" + age + ")";
    }

    private String name;
    private int age;
}

