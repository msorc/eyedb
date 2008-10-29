package org.eyedb.benchmark.polepos;

import org.eyedb.benchmark.polepos.teams.eyedb.EyeDBTeam;
import org.polepos.framework.Team;
import java.util.ArrayList;
import java.util.List;

public class RunSeasonEyeDB extends RunSeason {

    public static void main(String[] args) 
    {
	new RunSeasonEyeDB().run();
    }

    public List<Team> getTeams() 
    {
	List<Team> teams = new ArrayList<Team>();

	teams.add( new EyeDBTeam());
	
	return teams;
    }
}
