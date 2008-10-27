/* 
This file is part of the PolePosition database benchmark
http://www.polepos.org

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public
License along with this program; if not, write to the Free
Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
MA  02111-1307, USA. */

package org.polepos.teams.jorm;

import de.ama.db.DB;
import de.ama.db.Persistent;
import org.polepos.framework.Car;
import org.polepos.framework.Driver;
import org.polepos.framework.Team;


public class JormTeam extends Team{

	private final Car[] mCars;
    
    public JormTeam() {
        mCars = new Car[]{new JormCar() };

        DB db = new DB("localhost", "root", "polepos_jorm","");
        db.directExecuteSqlStatement("drop database if exists polepos_jorm");
        db.joinCatalog("polepos_jorm");

        DB.session().setVerbose(false);
        DB.session().delete(Persistent.class);
    }


    public void configure(int[] options) {

    }

    public String name(){
		return "GNA-JORM";
	}


    public String description() {
        return "the Jorm Team";
    }


	public Car[] cars(){
		return mCars;
	}
    

    public Driver[] drivers() {
        return new Driver[]{
            new MelbourneJorm(),
            new SepangJorm(),
            new BahrainJorm(),
            new ImolaJorm(),
            new NurburgringJorm(),
            new MonacoJorm(),
            new MontrealJorm(),
            new BarcelonaJorm()
        };
    }
    

    public String website() {
        return "http://www.marochow.de/gna-jorm/index.html";
    }

    public String databaseFile() {
        return null;
    }
}
