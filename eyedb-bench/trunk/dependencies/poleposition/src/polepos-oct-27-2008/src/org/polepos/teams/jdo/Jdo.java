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

package org.polepos.teams.jdo;

import java.lang.reflect.*;


public class Jdo {
    
    private final static JdoSettings sSettings = new JdoSettings();

    public static JdoSettings settings() {
        return sSettings;
    }
    
    /**
     * runs the JDO enhancer 
     * @param arguments
     */
    public static void main(String[] args) {
        
        
        if(args == null || args.length == 0){
            System.out.println("Supply the class");
        }
        
        enhanceObjectDB();
    }
    
    /**
     * ObjectDB is not supplied with the distribution 
     * @throws Exception
     */
    private static void enhanceObjectDB(){
        
        String clazz = "com.objectdb.Enhancer";
        String method = "enhance";
        Class[] types = new Class[] {String.class};
        Object[] params = new Object[] {"org.polepos.teams.jdo.data.*" };
        
        try{
            callByReflection(clazz, method, types, params);
        }catch(Exception e){
            System.out.println("ObjectDB libraries are not included");
            e.printStackTrace();
        }
    }
    
    private static void callByReflection(String enhancerClass, String enhancerMethod, Class[] parameterTypes, Object[] parameters) throws ClassNotFoundException, SecurityException, NoSuchMethodException, IllegalArgumentException, IllegalAccessException, InvocationTargetException{
        Class clazz = Class.forName(enhancerClass);
        Method method = clazz.getMethod(enhancerMethod, parameterTypes);
        method.invoke(null, parameters);
    }


}
