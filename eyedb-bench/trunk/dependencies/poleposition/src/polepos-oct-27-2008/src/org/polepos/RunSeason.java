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

package org.polepos;

import org.polepos.circuits.bahrain.Bahrain;
import org.polepos.circuits.barcelona.Barcelona;
import org.polepos.circuits.imola.Imola;
import org.polepos.circuits.melbourne.Melbourne;
import org.polepos.circuits.sepang.Sepang;
import org.polepos.framework.Circuit;
import org.polepos.framework.Team;
import org.polepos.runner.AbstractRunner;
import org.polepos.teams.db4o.Db4oTeam;
import org.polepos.teams.hibernate.HibernateTeam;
import org.polepos.teams.jdbc.JdbcTeam;
import org.polepos.teams.jdo.JdoTeam;

/**
 * @author Herkules, Andrew Zhang
 * 
 * This is the Main class to run PolePosition. If JDO is to be tested also,
 * JdoEnhance has to be run first.
 */
public class RunSeason extends AbstractRunner {

	public static void main(String[] args) {
		new RunSeason().run();
	}

	@Override
	public Circuit[] circuits() {
		return new Circuit[] { 
				new Melbourne(), 
				new Sepang(), 
				new Bahrain(),
				new Imola(),
				new Barcelona(), 
		};
	}

	@Override
	public Team[] teams() {
		return new Team[] { 
				new Db4oTeam(), 
				new HibernateTeam(),
				new JdbcTeam(),
				new JdoTeam() 
		};
	}

}
