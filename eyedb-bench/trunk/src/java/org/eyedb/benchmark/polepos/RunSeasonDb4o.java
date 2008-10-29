package org.eyedb.benchmark.polepos;

import org.polepos.teams.db4o.Db4oTeam;
import org.polepos.framework.Team;
import java.util.ArrayList;
import java.util.List;

public class RunSeasonDb4o extends RunSeason {

    public static void main(String[] args)
    {
	new RunSeasonDb4o().run();
    }

    public List<Team> getTeams() 
    {
	List<Team> teams = new ArrayList<Team>();

	teams.add( new Db4oTeam());

	return teams;
    }
}
