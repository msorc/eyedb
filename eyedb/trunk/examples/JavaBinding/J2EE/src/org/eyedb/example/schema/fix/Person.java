package org.eyedb.example.schema.fix;

import java.util.HashSet;
import java.util.Set;

import org.eyedb.CollSet;
import org.eyedb.Database;
import org.eyedb.example.schema.Car;

public class Person extends org.eyedb.example.schema.Person {

	public Person(Database db) throws org.eyedb.Exception
	{
		super(db);
	}

	public Set<Car> getCars() throws org.eyedb.Exception
	{
		Set<Car> set = new HashSet<Car>();
		CollSet cars= getCarsColl();
		
		for (int i = 0; i < cars.getCount(); i++)
			set.add( (Car)cars.getObjectAt(i));
		
		return set;
	}

	public void setCars( Set<Car> set) throws org.eyedb.Exception
	{
		CollSet cars= getCarsColl();
		
		for (int i = 0; i < cars.getCount(); i++)
			cars.suppress( (Car)cars.getObjectAt(i));
		
		for ( Car car: set)
			cars.insert( car);
	}
}
