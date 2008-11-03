package org.eyedb.benchmark.polepos.teams.db4o;

import org.polepos.framework.Driver;
import org.polepos.teams.db4o.Db4oOptions;

public class Db4oTeam extends org.polepos.teams.db4o.Db4oTeam {


    public Db4oTeam()
    {
	super();

	int[] options = new int[] { 
	    Db4oOptions.CLIENT_SERVER,
	    Db4oOptions.CLIENT_SERVER_TCP 
	};

	configure( options);
    }

    public String name()
    {
	return "db4o_mod";
    }

    public Driver[] drivers() 
    {
        return new Driver[] {
	    new BarcelonaDb4o(),
	};
    }
}
