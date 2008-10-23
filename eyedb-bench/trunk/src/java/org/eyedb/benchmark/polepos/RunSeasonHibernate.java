package org.eyedb.benchmark.polepos;

import org.polepos.teams.hibernate.HibernateTeam;
import org.polepos.circuits.bahrain.Bahrain;
import org.polepos.circuits.barcelona.Barcelona;
import org.polepos.circuits.imola.Imola;
import org.polepos.circuits.melbourne.Melbourne;
import org.polepos.circuits.sepang.Sepang;
import org.polepos.framework.Circuit;
import org.polepos.framework.Team;
import org.polepos.runner.AbstractRunner;

public class RunSeasonHibernate extends AbstractRunner {

    public static void main(String[] args)
    {
	new RunSeasonHibernate().run();
    }

    public Circuit[] circuits() 
    {
	return new Circuit[] { 
	    new Bahrain(),
	    new Barcelona(), 
	    new Imola(),
	    new Melbourne(), 
	    new Sepang(), 
	};
    }

    public Team[] teams() 
    {
	return new Team[] { 
	    new HibernateTeam(),
	};
    }

}
