package org.eyedb.benchmark.polepos.teams.eyedb;

import org.polepos.framework.Car;
import org.polepos.framework.Driver;
import org.polepos.framework.Team;

public class EyeDBTeam extends Team {
    
    private Car[] cars;
    
    public EyeDBTeam()
    {
	cars = new EyeDBCar[1];
	for ( int i = 0; i < cars.length; i++)
	    cars[i] = new EyeDBCar();
    }

    public String name()
    {
	return "EyeDB";
    }

    @Override
    public String description()
    {
        return "object database management system";
    }

    public String databaseFile()
    {
    	// not supported yet
    	return null;
    }

    public Car[] cars()
    {
	return cars;
    }
    
    public Driver[] drivers() 
    {
        return new Driver[] {
            new MelbourneEyeDB(),
            new SepangEyeDB(),
            new BahrainEyeDB(),
            new ImolaEyeDB(),
            new BarcelonaEyeDB()
        };
    }
    
    @Override
    public String website()
    {
        return "http://www.eyedb.org";
    }

    @Override
    public void configure(int[] options)
    {
    }
}
