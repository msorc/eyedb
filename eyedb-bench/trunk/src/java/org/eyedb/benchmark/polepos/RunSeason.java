package org.eyedb.benchmark.polepos;

import org.polepos.circuits.bahrain.Bahrain;
import org.polepos.circuits.barcelona.Barcelona;
import org.polepos.circuits.imola.Imola;
import org.polepos.circuits.melbourne.Melbourne;
import org.polepos.circuits.sepang.Sepang;
import org.polepos.framework.Circuit;
import org.polepos.framework.Team;
import org.polepos.reporters.Reporter;
//import org.polepos.reporters.PlainTextReporter;
import java.util.ArrayList;
import java.util.List;

abstract public class RunSeason {

    public void run()
    {
	new Racer( getCircuits(), getTeams(), getReporters()).run();
    }

    public List<Circuit> getCircuits() 
    {
	List<Circuit> circuits = new ArrayList<Circuit>();

	circuits.add (new Bahrain());
	circuits.add (new Barcelona());
	circuits.add (new Imola());
	circuits.add (new Melbourne());
	circuits.add (new Sepang());

	return circuits;
    }

    public abstract List<Team> getTeams();

    public List<Reporter> getReporters()
    {
	List<Reporter> reporters = new ArrayList<Reporter>();

	//	reporters.add( new PlainTextReporter());
	reporters.add( new SimpleReporter());
	
	return reporters;
    }
}
