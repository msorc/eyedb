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

/**
 * a result for a lap that holds the name and the time.
 */
public class Result {
    
    
    private final Circuit _circuit;
    
    private final Team _team;
    
    private final TurnSetup _setup;
    
    private final int _index;
    
    private final Lap _lap;
    
    private final long _time;
    
    private final long _checkSum;
    
    private final long _memory;    
    
    private final long _databaseSize;
    
    public Result(Circuit circuit, Team team, Lap lap, TurnSetup setup, int index, long time, long memory, long databaseSize, long checkSum){
        _circuit = circuit;
        _team = team;
        _lap = lap;
        _setup = setup;
        _index = index;
        _time = time;
        _memory = memory;
        _databaseSize = databaseSize;
        _checkSum = checkSum;
    }
    
    public String getName(){
        return _lap.name();
    }
    
    public long getTime(){
        return _time;
    }
    
    public TurnSetup getSetup(){
        return _setup;
    }
    
    public int getIndex(){
        return _index;
    }
    
    public Circuit getCircuit(){
        return _circuit;
    }
    
    public Lap getLap(){
        return _lap;
    }
    
    public long getCheckSum(){
        return _checkSum;
    }
    
    public Team getTeam(){
        return _team;
    }
    
    public long getMemory(){
        return _memory;
    }
    
    public long getDatabaseSize() {
    	return _databaseSize;
    }
}

