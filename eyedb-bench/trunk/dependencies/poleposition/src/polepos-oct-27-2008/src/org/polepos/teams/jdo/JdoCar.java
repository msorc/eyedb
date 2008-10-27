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

import java.io.*;
import java.util.*;


import javax.jdo.*;

import org.polepos.framework.*;
import org.polepos.teams.jdbc.*;


/**
 * @author Herkules
 */
public class JdoCar extends Car {

    private PersistenceManagerFactory mFactory;

    private final String              mDbName;
    private final String              mName;

    JdoCar(String name, String dbName) throws CarMotorFailureException {

        mName = name;
        mDbName = dbName;

        _website = Jdo.settings().getWebsite(name);
        _description = Jdo.settings().getDescription(name);

        initialize();

    }

    private boolean isSQL() {
        return mDbName != null;
    }
    
    private void versantCreateSchema(){
        
        JdoImplementations.voaNotAvailable();
        
//        try{
//            
//            CreateJdbcSchemaTask t = new CreateJdbcSchemaTask();
//            
//            t.setDroptables("true");
//            t.setCreatetables("true");
//            
//            // This is a bit of an ugly hack.
//            // JDBC Connection information is redundant in the
//            // versant.[dbname] files. They should be written here.
//            t.setConfig("versant." + mDbName + ".properties");
//            
//            t.execute();
//            
//        }catch(Exception e){
//            e.printStackTrace();
//        }
        
    }

    
    private void initialize() {
        
        if(mName.equals("voa")){
            versantCreateSchema();
        }
        
        
        
        Properties properties = new Properties();

        properties.setProperty("javax.jdo.PersistenceManagerFactoryClass", Jdo.settings()
            .getFactory(mName));
        
        properties.setProperty("javax.jdo.option.NontransactionalRead", "true");
        
        properties.setProperty("versant.metadata.0", "org/polepos/teams/jdo/data/package.jdo");
        // properties.setProperty("versant.useClassloader", "true");
        properties.setProperty("versant.hyperdrive", "false");
        
        // turning metric snapshots off.
        properties.setProperty("versant.metricSnapshotIntervalMs", "1000000000");
        
        properties.setProperty("versant.logging.logEventsToSysOut", "false");

        if (isSQL()) {
            try {
                Class.forName(Jdbc.settings().getDriverClass(mDbName)).newInstance();
            } catch (Exception ex) {
                ex.printStackTrace();
            }

            properties.setProperty("javax.jdo.option.ConnectionDriverName", Jdbc.settings()
                .getDriverClass(mDbName));
            properties.setProperty("javax.jdo.option.ConnectionURL", Jdbc.settings().getConnectUrl(
                mDbName));
            String user = Jdbc.settings().getUsername(mDbName);
            if (user != null) {
                properties.setProperty("javax.jdo.option.ConnectionUserName", user);
            }

            String password = Jdbc.settings().getPassword(mDbName);
            if (password != null) {
                properties.setProperty("javax.jdo.option.ConnectionPassword", password);
            }
        } else {

            properties.setProperty("javax.jdo.option.ConnectionURL", Jdo.settings().getURL(mName));

            String user = Jdo.settings().getUsername(mName);
            if (user != null) {
                properties.setProperty("javax.jdo.option.ConnectionUserName", user);
            }

            String password = Jdo.settings().getPassword(mName);
            if (password != null) {
                properties.setProperty("javax.jdo.option.ConnectionPassword", password);
            }

            properties.setProperty("javax.jdo.option.ConnectionUserName", "login");
            properties.setProperty("javax.jdo.option.ConnectionPassword", "password");
        }

        properties.setProperty("org.jpox.autoCreateSchema", "true");
        properties.setProperty("org.jpox.validateTables", "false");
        properties.setProperty("org.jpox.validateConstraints", "false");

        // deleteObjectDBFile();   

        mFactory = JDOHelper.getPersistenceManagerFactory(properties, JDOHelper.class
            .getClassLoader());
        
        // mFactory = JDOHelper.getPersistenceManagerFactory(properties);
    }

    /**
     *
     */
    public PersistenceManager getPersistenceManager() {
        return mFactory.getPersistenceManager();
    }

    /**
     * Delete the file in case of an local ObjectDB setup.
     */
    private void deleteObjectDBFile() {
        String path = Jdo.settings().getConnectUrl();
        if (!path.startsWith("objectdb://")) // only local
        {
            new File(path).delete();
        }
    }

    @Override
    public String name() {
        
        String name = mName;
        
        String publicName=Jdo.settings().getName(name);
        if(publicName != null && publicName.length() > 1){
            name = publicName;
        }
        
        if (isSQL()) {
            return name + "/" + mDbName;
        }
        return name;
    }

}
