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


public class CSVReporter extends GraphReporter {
    
    private final static String TAB = "\t";

	protected void report(Graph graph){
        new File(path()).mkdirs();
        
        String fileName = path() + "/" + graph.circuit().name() + "_" +  graph.lap().name() + ".f1graph";
        new File(fileName).delete();
        
        PrintStream ps = null;
        try {
            ps = new PrintStream( new FileOutputStream( new File(fileName), false));
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
        
        ps.print(graph.circuit().name());
        ps.print(" ");
        ps.print(graph.lap().name());
		renderTableHeader(graph, ps);
		renderTableBody(graph, ps);
        
        ps.flush();
        ps.close();
    }

	private void renderTableHeader(Graph graph, PrintStream ps) {
		for(TurnSetup setup : graph.setups()) {
            ps.print(TAB);
            boolean first = true;
            for(SetupProperty sp : setup.properties()){
                if(! first){
                    ps.print(" ");
                }
                ps.print(sp.name());
                ps.print(":");
                ps.print(sp.value());
                first = false;
            }
        }
		ps.println();
	}

	private void renderTableBody(Graph graph, PrintStream ps) {
		for(TeamCar teamCar : graph.teamCars()) {
            ps.print(teamCar);            
			for(TurnSetup setup : graph.setups()) {
                ps.print(TAB);
                ps.print(graph.timeFor(teamCar,setup));
            }
            ps.println();
        }
	}

	protected void finish(List <TeamCar> cars) {
	}

}
