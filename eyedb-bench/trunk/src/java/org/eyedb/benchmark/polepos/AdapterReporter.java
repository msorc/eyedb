package org.eyedb.benchmark.polepos;

import java.util.ArrayList;
import java.util.List;

import org.eyedb.benchmark.framework.Benchmark;
import org.polepos.framework.Car;
import org.polepos.framework.Circuit;
import org.polepos.framework.Result;
import org.polepos.framework.SetupProperty;
import org.polepos.framework.Team;
import org.polepos.framework.TurnSetup;
import org.polepos.reporters.Reporter;

public class AdapterReporter extends Reporter {

    class AdapterBenchmark extends Benchmark {

	public String getDescription()
	{
	    return description;
	}

	public void setDescription(String description)
	{
	    this.description = description;
	}

	public String getName()
	{
	    return name;
	}

	public void setName(String name)
	{
	    this.name = name;
	}

	public String getRunDescription()
	{
	    return runDescription;
	}

	public void setRunDescription(String runDescription)
	{
	    this.runDescription = runDescription;
	}

	public void prepare()
	{
	}

	public void run()
	{
	}

	public void finish()
	{
	}

	private String name;
	private String description;
	private String runDescription;
    }
    
     public AdapterReporter()
    {
	taskNames = new ArrayList<String>();
	setupProperties = new ArrayList<String>();

	resultCount = 0;
    }

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
    
    public void sendToCircuit(Circuit circuit)
    {
        super.sendToCircuit(circuit);

        adapterBenchmark = new AdapterBenchmark();

        adapterBenchmark.setName( circuit.name());
        adapterBenchmark.setDescription( circuit.description());

        result = new org.eyedb.benchmark.framework.Result();
        
	taskNames.clear();
    }

    public void reportTaskName(int number, String name)
    {
	taskNames.add( name);
    }
    
    protected void reportTeam(Team team)
    {
	adapterBenchmark.setRunDescription( team.name());
    }
    
    protected void reportCar(Car car)
    {
	String s = adapterBenchmark.getRunDescription();
	adapterBenchmark.setRunDescription( s + " - " + car.name());
    }

    public void reportSetups(TurnSetup[] setups)
    {
	setupProperties.clear();

	for( SetupProperty sp : setups[0].properties()) {
	    setupProperties.add( sp.name());
	}

	addColumnHeaders();
    }

    private void addColumnHeaders()
    {
	for( String setupProperty : setupProperties)
	    result.addHeader(setupProperty);

	for (String taskName: taskNames)
	    result.addHeader( taskName);
    }

    protected void beginResults()
    {
	resultCount = 0;
    }

    public void reportResult(Result result)
    {
	if ( resultCount == 0) {
	    for( SetupProperty sp : result.getSetup().properties())
		this.result.addValue(sp.value());
	}

	this.result.addValue(result.getTime());

	resultCount++;
    }

    private AdapterBenchmark adapterBenchmark;
    private org.eyedb.benchmark.framework.Result result;
    
    private List<String> taskNames;
    private List<String> setupProperties;

    private int resultCount;
}
