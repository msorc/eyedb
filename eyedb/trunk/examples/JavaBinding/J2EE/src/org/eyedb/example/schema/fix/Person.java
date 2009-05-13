package org.eyedb.example.schema.fix;

import java.util.HashSet;
import java.util.Set;

import org.eyedb.CollSet;
import org.eyedb.Oid;
import org.eyedb.example.EyeDBBean;
import org.eyedb.example.schema.Car;

public class Person {

	private void loadCars() throws org.eyedb.Exception
	{
		cars = new HashSet<Car>();
		
		EyeDBBean.logger.info( getFirstname() + " " + getLastname() + " : " + person.getCarsCount());
		for (int i = 0; i < person.getCarsCount(); i++) {
			EyeDBBean.logger.info( "Adding car" + person.getCarsAt(i));
			cars.add( person.getCarsAt(i));
		}
	}

	public Person( org.eyedb.example.schema.Person person) throws org.eyedb.Exception
	{
		this.person = person;
		loadCars();
	}

	public Oid getOid() throws org.eyedb.Exception
	{
		return person.getOid();
	}
	
	public String getLastname() throws org.eyedb.Exception
	{
		return person.getLastname();
	}
	
	public void setLastname( String lastname) throws org.eyedb.Exception
	{
		person.setLastname( lastname);
	}
	
	public int getAge() throws org.eyedb.Exception
	{
		return person.getAge();
	}
	
	public void setAge( int age) throws org.eyedb.Exception
	{
		person.setAge( age);
	}
	
	public String getFirstname() throws org.eyedb.Exception
	{
		return person.getFirstname();
	}
	
	public void setFirstname( String firstname) throws org.eyedb.Exception
	{
		person.setFirstname( firstname);
	}
	
	public Set<Car> getCars() throws org.eyedb.Exception
	{
		return cars;
	}

	public void setCars( Set<Car> set) throws org.eyedb.Exception
	{
		CollSet cars= person.getCarsColl();
		
		for (int i = 0; i < cars.getCount(); i++)
			cars.suppress( (Car)cars.getObjectAt(i));
		
		for ( Car car: set)
			cars.insert( car);
	}
	
	private org.eyedb.example.schema.Person person;
	private Set<Car> cars;
}
