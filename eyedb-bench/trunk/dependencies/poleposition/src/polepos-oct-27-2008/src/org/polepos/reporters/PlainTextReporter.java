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

import java.io.*;
import java.util.*;

import org.polepos.framework.*;


public class PlainTextReporter extends Reporter{
    
    protected PrintStream mOut;
    private boolean mSetupReported;

    @Override
    public void startSeason() {
        
        if(!append()){
            new File(file()).delete();
        }
        try{
            mOut = new PrintStream( new FileOutputStream( file(), append()));
        }
        catch ( IOException ioex ){
            ioex.printStackTrace();
        }
		
        mOut.println();
        mOut.println( "===========================================================================================" ); 
        mOut.println( "Season started at " + new Date().toString() );
        mOut.println( "===========================================================================================" );
    }

    @Override
    public boolean append() {
        return true;
    }

    @Override
    public String file() {
        return path() + "/F1Season.log";
    }

    @Override
    public void noDriver(Team team, Circuit circuit) {
        mOut.println("*** No driver for team " + team.name() + " in " + circuit.name() );
    }
    
    @Override
    public void sendToCircuit(Circuit circuit) {
        super.sendToCircuit(circuit);
        mOut.println();
        mOut.println( "-------------------------------------------------------------------------------------------" );
        mOut.println(circuit.name());
        mOut.println(circuit.description());
        mOut.println( "-------------------------------------------------------------------------------------------" );
        mOut.println();
    }
    
    @Override
    public void reportTaskName(int number, String name){
        mOut.println("[" + number + "] " + name );
    }

    @Override
    public void reportTeam(Team team) {
        mOut.println();
        mOut.println("- " + team.name());
    }

    @Override
    public void reportCar(Car car) {
        mOut.println("---   " + car.name());
        mOut.println();
    }

    @Override
    public void beginResults() {
        mOut.println();
        mSetupReported = false;
    }
    
    @Override
    public void reportResult(Result result) {
        if(! mSetupReported){
            mSetupReported = true;
            TurnSetup setup = result.getSetup();
            for(SetupProperty sp : setup.properties()){
                mOut.print(sp.name());
                mOut.print(":");
                mOut.print(sp.value());
                mOut.print("   ");
            }
            mOut.println();
        }
        mOut.println("[" + result.getIndex() + "] " + result.getTime() + "ms   ");
    }

	public void endSeason() {
        mOut.flush();
        mOut.close();
	}
}
