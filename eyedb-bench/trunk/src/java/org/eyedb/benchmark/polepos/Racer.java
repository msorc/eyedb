/* 
 This file is derived from software part of the PolePosition 
 database benchmark
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
 MA  02111-1307, USA. 
*/
package org.eyedb.benchmark.polepos;

import org.polepos.framework.Car;
import org.polepos.framework.Circuit;
import org.polepos.framework.Driver;
import org.polepos.framework.Team;
import org.polepos.framework.TurnSetup;
import org.polepos.framework.TurnResult;
import org.polepos.reporters.Reporter;
import java.util.List;

public class Racer {

    public Racer( List<Circuit> circuits, List<Team> teams, List<Reporter> reporters)
    {
	this.circuits = circuits;
	this.teams = teams;
	this.reporters = reporters;
    }

    public void run() {
        synchronized (this) {

            for (Reporter reporter : reporters) {
                reporter.startSeason();
            }
            
            for (Team team : teams) {
                
                for (Car car : team.cars()) {
		    
                    for (Circuit circuit : circuits) {

                        System.out.println("\n** Racing " + team.name() + "/"
                            + car.name() + " on " + circuit.name() + "\n");

                        for (Reporter reporter : reporters) {
                            reporter.sendToCircuit(circuit);
                        }

                        Driver[] drivers = team.nominate(circuit);

                        if (drivers == null || drivers.length == 0) {
    
                            for (Reporter reporter : reporters) {
                                reporter.noDriver(team, circuit);
                            }
                            
                            continue;
                        }
    
                        for (Driver driver : drivers) {
    
                            TurnSetup[] setups = circuit.lapSetups();
                            TurnResult[] results = circuit.race(team, car, driver);
    
                            for (Reporter reporter : reporters) {
                                reporter.report(team, car, setups, results);
                            }
                        }
                    }
                }
            }

            for (Reporter reporter : reporters) {
                reporter.endSeason();
            }
        }
    }

    private List<Circuit> circuits;
    private List<Team> teams;
    private List<Reporter> reporters;
}
