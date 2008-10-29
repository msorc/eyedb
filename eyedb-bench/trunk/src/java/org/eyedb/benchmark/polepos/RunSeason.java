package org.eyedb.benchmark.polepos;

import org.polepos.framework.Circuit;
import org.polepos.framework.Team;
import org.polepos.reporters.Reporter;
import org.polepos.reporters.PlainTextReporter;
import java.util.ArrayList;
import java.util.List;

abstract public class RunSeason {

    public void run()
    {
	new Racer( getCircuits(), getTeams(), getReporters()).run();
    }

    public abstract List<Circuit> getCircuits();

    public abstract List<Team> getTeams();

    public List<Reporter> getReporters()
    {
	List<Reporter> reporters = new ArrayList<Reporter>();

	reporters.add( new PlainTextReporter());
	
	return reporters;
    }
}
