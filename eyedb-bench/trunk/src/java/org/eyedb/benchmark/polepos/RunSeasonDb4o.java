package org.eyedb.benchmark.polepos;

import org.polepos.teams.db4o.Db4oTeam;
import org.polepos.circuits.bahrain.Bahrain;
import org.polepos.circuits.barcelona.Barcelona;
import org.polepos.circuits.imola.Imola;
import org.polepos.circuits.melbourne.Melbourne;
import org.polepos.circuits.sepang.Sepang;
import org.polepos.framework.Circuit;
import org.polepos.framework.Team;
import java.util.ArrayList;
import java.util.List;

public class RunSeasonDb4o extends RunSeason {

    public static void main(String[] args)
    {
	new RunSeasonDb4o().run();
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

    public List<Team> getTeams() 
    {
	List<Team> teams = new ArrayList<Team>();

	teams.add( new Db4oTeam());

	return teams;
    }

}
