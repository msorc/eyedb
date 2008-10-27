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


public class Settings {
    
    /**
     * This variable is not final, to prevent inlining by the compiler, 
     * so it get's printed to System.out when it is accessed.
     */
    private static boolean DEBUG = false;
    
    public static final String CIRCUIT = DEBUG ? "settings/DebugCircuits.properties" : "settings/Circuits.properties" ;
    
    public static final String JDBC = "settings/Jdbc.properties";
    
    public static final String JDO = "settings/Jdo.properties";
    
    static{
        
        String className = Settings.class.getName() ;
        
        if(DEBUG){
            System.out.println(className + ".DEBUG is set to true.\n");
        }

    }

}
