package org.eyedb.benchmark.polepos;

import org.polepos.teams.hibernate.HibernateTeam;
import org.polepos.framework.Team;
import java.util.ArrayList;
import java.util.List;

public class RunSeasonHibernate extends RunSeason {

    public static void main(String[] args)
    {
	new RunSeasonHibernate().run();
    }

    public List<Team> getTeams() 
    {
	List<Team> teams = new ArrayList<Team>();

	teams.add( new HibernateTeam());
	
	return teams;
    }

}
