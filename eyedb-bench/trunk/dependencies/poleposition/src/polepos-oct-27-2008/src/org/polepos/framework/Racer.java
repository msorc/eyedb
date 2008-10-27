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

package org.polepos.framework;

import java.util.*;

import org.polepos.reporters.*;

public class Racer implements Runnable {

    private final List<Circuit> circuits;
    
    private final List<Team>    teams;

    public Racer(List<Circuit> circuits_, List<Team> teams_) {
        circuits = circuits_;
        teams = teams_;
    }
    
    public Racer(Circuit[] circuits_, Team[] teams_) {

        circuits = new ArrayList <Circuit>();
        for(Circuit circuit : circuits_){
            circuits.add(circuit);
        }

        teams = new ArrayList <Team>();
        for(Team team : teams_){
            teams.add(team);
        }
    }

    public void run() {

        synchronized (this) {

            long start = System.currentTimeMillis();

            Reporter[] reporters = new Reporter[] { new PlainTextReporter(), new PDFReporter(),
                new CSVReporter(), new HTMLReporter()};

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

            long stop = System.currentTimeMillis();
            long duration = stop - start;

            System.out.println("\n****************************************************");
            System.out.println("The F1 season was run O.K. without lethal accidents.");
            System.out.println("****************************************************\n");
            System.out.println("Overall time taken: " + duration + "ms\n");
            System.out.println("All output from this benchmark run can be found in:");
            System.out.println(reporters[0].path());
            this.notify();
        }
        
    }
}
