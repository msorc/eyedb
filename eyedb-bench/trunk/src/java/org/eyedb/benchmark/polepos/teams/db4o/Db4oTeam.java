package org.eyedb.benchmark.polepos.teams.db4o;

import org.polepos.framework.Driver;

public class Db4oTeam extends org.polepos.teams.db4o.Db4oTeam {

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
