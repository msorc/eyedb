package org.eyedb.benchmark.polepos;

import java.util.ArrayList;
import java.util.List;

import org.eyedb.benchmark.polepos.teams.db4o.Db4oTeam;
import org.polepos.circuits.barcelona.Barcelona;
import org.polepos.framework.Circuit;
import org.polepos.framework.Team;

public class RunSeasonDb4oMod extends RunSeason {

    public static void main(String[] args)
    {
	new RunSeasonDb4oMod().run();
    }

    public List<Circuit> getCircuits() 
    {
	List<Circuit> circuits = new ArrayList<Circuit>();

	circuits.add (new Barcelona());

	return circuits;
    }

    public List<Team> getTeams() 
    {
	List<Team> teams = new ArrayList<Team>();

	teams.add( new Db4oTeam());

	return teams;
    }
}
