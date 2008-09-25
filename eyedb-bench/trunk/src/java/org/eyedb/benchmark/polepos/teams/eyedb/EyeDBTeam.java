package org.eyedb.benchmark.polepos.teams.eyedb;

import org.polepos.framework.Car;
import org.polepos.framework.Driver;
import org.polepos.framework.Team;

public class EyeDBTeam extends Team {
    
    private final Car[] mCars;
    
    public EyeDBTeam()
    {
	mCars = new EyeDBCar[2];
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
	return mCars;
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
