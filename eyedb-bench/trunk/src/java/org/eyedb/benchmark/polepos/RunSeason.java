package org.eyedb.benchmark.polepos;

import org.polepos.circuits.bahrain.Bahrain;
import org.polepos.circuits.barcelona.Barcelona;
import org.polepos.circuits.imola.Imola;
import org.polepos.circuits.melbourne.Melbourne;
import org.polepos.circuits.sepang.Sepang;
import org.polepos.framework.Circuit;
import org.polepos.framework.Team;
import org.polepos.reporters.Reporter;
import org.polepos.reporters.PlainTextReporter;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

abstract public class RunSeason {

    public RunSeason()
    {
	properties = new org.eyedb.benchmark.framework.Properties();
    }

    public void run()
    {
	new Racer( getCircuits(), getTeams(), getReporters()).run();
    }

    public List<Circuit> getCircuits() 
    {
	List<Circuit> circuits = new ArrayList<Circuit>();
	Collection<String> circuitNames = new ArrayList<String>();

	if ( properties.getStringProperty( "polepos.circuits", circuitNames)) {
	    for ( String cn: circuitNames) {
		Class cl = RunSeason.getCircuitClass( cn);
		if (cl != null) {
		    try {
			circuits.add( (Circuit)cl.newInstance());
		    }
		    catch( Exception e) {
		    }
		}
	    }
	} else {
	    circuits.add (new Bahrain());
	    circuits.add (new Barcelona());
	    circuits.add (new Imola());
	    circuits.add (new Melbourne());
	    circuits.add (new Sepang());
	}

	return circuits;
    }

    public abstract List<Team> getTeams();

    public List<Reporter> getReporters()
    {
	List<Reporter> reporters = new ArrayList<Reporter>();

	reporters.add( new PlainTextReporter());
//	reporters.add( new SimpleReporter());
	reporters.add( new AdapterReporter());
	
	return reporters;
    }

    private static HashMap<String, Class> circuitClasses;

    static
    {
	circuitClasses = new HashMap<String, Class>();

	circuitClasses.put( "bahrain", org.polepos.circuits.bahrain.Bahrain.class);
	circuitClasses.put( "barcelona", org.polepos.circuits.barcelona.Barcelona.class);
	circuitClasses.put( "imola", org.polepos.circuits.imola.Imola.class);
	circuitClasses.put( "melbourne", org.polepos.circuits.melbourne.Melbourne.class);
	circuitClasses.put( "sepang", org.polepos.circuits.sepang.Sepang.class);
    }

    private static Class getCircuitClass( String circuitName)
    {
	return (Class)circuitClasses.get( circuitName);
    }

    private org.eyedb.benchmark.framework.Properties properties;
}
