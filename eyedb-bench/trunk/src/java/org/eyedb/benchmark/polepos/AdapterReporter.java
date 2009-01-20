package org.eyedb.benchmark.polepos;

import java.util.ArrayList;
import java.util.List;

import org.eyedb.benchmark.framework.Benchmark;
import org.eyedb.benchmark.framework.reporter.ReporterFactory;
import org.polepos.framework.Car;
import org.polepos.framework.Circuit;
import org.polepos.framework.Result;
import org.polepos.framework.SetupProperty;
import org.polepos.framework.Team;
import org.polepos.framework.TurnSetup;
import org.polepos.reporters.Reporter;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

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

	public String getImplementation()
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
	
	reporter = ReporterFactory.newInstance().newReporter();
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
	if (benchmark != null)
	    reporter.report(benchmark);
    }
    
    public void sendToCircuit(Circuit circuit)
    {
        super.sendToCircuit(circuit);

        if (benchmark != null)
            reporter.report( benchmark);
        
        benchmark = new AdapterBenchmark();

        benchmark.setName( circuit.name());
        benchmark.setDescription( circuit.description());
        
	taskNames.clear();
    }

    public void reportTaskName(int number, String name)
    {
	taskNames.add( name);
    }
    
    protected void reportTeam(Team team)
    {
	benchmark.setRunDescription( team.name());
    }
    
    protected void reportCar(Car car)
    {
	String s = benchmark.getImplementation();
	benchmark.setRunDescription( s + " - " + car.name());
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
	    benchmark.getResult().addHeader(setupProperty);

	for (String taskName: taskNames)
	    benchmark.getResult().addHeader( taskName);
    }

    protected void beginResults()
    {
	resultCount = 0;
    }

    public void reportResult(Result result)
    {
	if ( resultCount == 0) {
	    for( SetupProperty sp : result.getSetup().properties())
		benchmark.getResult().add(sp.value());
	}

	benchmark.getResult().add(result.getTime());

	if (resultCount == taskNames.size() - 1)
	    benchmark.getResult().next();

	resultCount++;
    }

    private AdapterBenchmark benchmark;
    private org.eyedb.benchmark.framework.Reporter reporter;
    
    private List<String> taskNames;
    private List<String> setupProperties;

    private int resultCount;
}
