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
import java.util.*;

public class JdoEnhancer {

    private String                   className;
    private String                   methodName;
    private Class[]                  types;
    private Object[]                 params;

    private final static Map<String, JdoEnhancer> enhancers = registerEnhancers();
    
    private JdoEnhancer(){
    }

    public JdoEnhancer(String enhancerClass, String enhancerMethod, Class[] parameterTypes,
        Object[] parameters) {
        className = enhancerClass;
        methodName = enhancerMethod;
        types = parameterTypes;
        params = parameters;
    }

    public static void main(String[] args) throws Exception{
        
        if(args == null || args.length == 0){
            System.err.println("Supply the name of the enhancer to org.polepos.teams.jdo.JdoEnhancer#main()");
            printRegisteredEnhancers();
            return;
        }
        
        JdoEnhancer enhancer = enhancers.get(args[0]);
        if(enhancer == null){
            System.err.println("Enhancer " + args[0] + " is not registered in org.polepos.teams.jdo.JdoEnhancer");
            printRegisteredEnhancers();
            return;
        }
        
        if(enhancer.isRunnable()){
            enhancer.run();
        }else{
            try {
                enhancer.callByReflection();
            } catch (Exception e) {
                System.err.println("Jdo enhancing was not possible with the supplied enhancer name.");
                e.printStackTrace();
                printRegisteredEnhancers();
            }
        }
    }
    
    private static void printRegisteredEnhancers(){
        System.err.println("The following enhancers are registered, but they are only");
        System.err.println("available if the respective Jars are present in /lib");
        for(String key : enhancers.keySet()){
            System.err.println(key);
        }
    }

    private void callByReflection() throws ClassNotFoundException, SecurityException,
        NoSuchMethodException, IllegalArgumentException, IllegalAccessException,
        InvocationTargetException {
        Class clazz = Class.forName(className);
        Method method = clazz.getMethod(methodName, types);
        method.invoke(null, params);
    }

    private final static Map<String, JdoEnhancer> registerEnhancers() {

        Map<String, JdoEnhancer> map = new HashMap<String, JdoEnhancer>();
        
        //  tested successfully with odbfe.jar and jdo.jar in the lib folder 
        //  Both are supplied the ObjectDB free edition downloadable from:
        //  http://www.objectdb.com
        JdoEnhancer objectDBEnhancer = new JdoEnhancer(
            "com.objectdb.Enhancer", 
            "enhance",
            new Class[] { String.class }, 
            new Object[] { "org.polepos.teams.jdo.data.*" }
        );
        map.put("objectdb", objectDBEnhancer);
        
        
        JdoEnhancer voaEnhancer = new JdoEnhancer(){
            
            public boolean isRunnable(){
                
                // uncomment
                return false;
                
            }
            
            public void run(){
                
                JdoImplementations.voaNotAvailable();
                
                
//                com.versant.core.jdo.tools.enhancer.Enhancer.main(
//                    new String[]{
//                    "-p",
//                    "versant.properties",
//                });
            }
            
        };
        
        map.put("voa", voaEnhancer);
        
        
        // TODO: add more enhancers here and register them like above
        

        return map;
    }

    public void run() {
        // virtual method to override 
    }
    
    public boolean isRunnable() {
        // virtual method to override
        return false;
    }

}
