package org.eyedb.benchmark.polepos;

import org.polepos.framework.Car;
import org.polepos.framework.Result;
import org.polepos.framework.Team;
import org.polepos.reporters.Reporter;

public class SimpleReporter extends Reporter {

    public String file()
    {
	return "";
    }
    
    public boolean append()
    {
	return false;
    }
    
    public void startSeason()
    {
    }
    
    public void endSeason()
    {
    }
    
    public void reportTaskName(int number, String name)
    {
    }
    
    protected void reportTeam(Team team)
    {
    }
    
    protected void reportCar(Car car)

    {
    }

    protected void beginResults()
    {
    }

    public void reportResult(Result result)
    {
    }
}
