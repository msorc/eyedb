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

package org.polepos.reporters;

import java.util.*;

import org.polepos.framework.*;


public class Graph {
    
    private final List<TeamCar>teamCars;
	private final List<TurnSetup> setups;
    private final Map<ResultsKey,Result> results;
	
    private final Circuit circuit;
    private final Lap lap;
    
    public Graph(Result result){
        teamCars= new ArrayList<TeamCar>();
        setups=new ArrayList<TurnSetup>();
		results=new HashMap<ResultsKey,Result>();
        circuit = result.getCircuit();
        lap = result.getLap(); 
    }

    public void addResult(TeamCar teamCar, Result result) {
        TurnSetup setup = result.getSetup();
        results.put(new ResultsKey(teamCar,setup),result);
		if(!teamCars.contains(teamCar)) {
			teamCars.add(teamCar);
		}
		if(!setups.contains(setup)) {
			setups.add(setup);
		}
    }
    
    public Circuit circuit(){
        return circuit;
    }
    
    public Lap lap(){
        return lap;
    }
    
    public void compareCheckSums(){
        
        for (TurnSetup setup : setups()){
            
            long checkSum = 0;
            boolean first = true;
            
            for (TeamCar teamCar: teamCars()){
                
                if(first){
                    Result res = results.get(new ResultsKey(teamCar,setup)); 
                    if(res != null){
                        checkSum = res.getCheckSum();
                        first = false;
                    }
                } else{
                    Result res = results.get(new ResultsKey(teamCar,setup));
                    if(res != null){
                        if(checkSum != res.getCheckSum()){
                            System.err.println("Inconsistent checksum for " + res.getTeam().name() + " in " + circuit.name() + ":" + lap.name());
                        }
                    }
                }
            }
        }
    }
	
	
	public List<TeamCar> teamCars() {
		return Collections.unmodifiableList(teamCars);
	}

	public List<TurnSetup> setups() {
		return Collections.unmodifiableList(setups);
	}
	
	public final long timeFor(TeamCar teamCar, TurnSetup setup) {
        Result res = results.get(new ResultsKey(teamCar,setup));
        if(res == null){
            return Integer.MAX_VALUE;
        }
		return res.getTime();
	}
	
	public final long memoryFor(TeamCar teamCar, TurnSetup setup) {
	    Result res = results.get(new ResultsKey(teamCar,setup));
        if(res == null){
            return Integer.MAX_VALUE;
        }
		return res.getMemory();
	}
	
	public final long sizeFor(TeamCar teamCar, TurnSetup setup) {
	    Result res = results.get(new ResultsKey(teamCar,setup));
        if(res == null){
            return Integer.MAX_VALUE;
        }
		return res.getDatabaseSize();
	}
	
	private class ResultsKey {
        
        final TeamCar teamCar;
        final TurnSetup setup;
		
		public ResultsKey(TeamCar teamCar, TurnSetup setup) {
			this.teamCar = teamCar;
			this.setup = setup;
		}
		
		@Override
		public boolean equals(Object obj) {
			if(obj==this) {
				return true;
			}
			if(obj==null||obj.getClass()!=getClass()) {
				return false;
			}
			ResultsKey key=(ResultsKey)obj;
			return teamCar.equals(key.teamCar) && setup.equals(key.setup);
		}
		
		@Override
		public int hashCode() {
			return teamCar.hashCode() + setup.hashCode();
		}
	}
}
