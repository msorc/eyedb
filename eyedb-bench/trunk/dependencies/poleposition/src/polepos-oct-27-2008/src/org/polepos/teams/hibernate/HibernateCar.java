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

package org.polepos.teams.hibernate;

import org.polepos.framework.*;
import org.polepos.teams.hibernate.data.*;
import org.polepos.teams.jdbc.*;

import net.sf.hibernate.*;
import net.sf.hibernate.Session;
import net.sf.hibernate.cfg.*;
import net.sf.hibernate.tool.hbm2ddl.*;

/**
 *
 * @author Herkules
 */
public class HibernateCar extends Car
{
    
    private SessionFactory mFactory;
    
    private final String mDBType;
    
    private Session mSession;
    
    
    public HibernateCar(String dbType){
        mDBType = dbType;
    }

    public String name(){
        return mDBType;
    }
    
    public void openSession() throws CarMotorFailureException{

        if ( mFactory == null) {
            mFactory = getSessionFactory();
        }

        assert mSession == null;
        
        try {
            mSession = mFactory.openSession();
        } catch (HibernateException e) {
            e.printStackTrace();
            throw new CarMotorFailureException();
        }
    }
    
    public void closeSession(){
        
        if(mSession != null){
            try {
                mSession.close();
            } catch (HibernateException e) {
                e.printStackTrace();
            }
            mSession = null;
        }
        
    }
    
    public Session getSession(){
        return mSession;
    }
    
    /**
     *
     */
    private SessionFactory getSessionFactory()
    {
        try
        {
            Configuration cfg = new Configuration()
                    .addClass( HibernatePilot.class )
                    .addClass( HibernateTree.class )
                    .addClass( HibernateIndexedPilot.class )
                    .addClass(HB0.class);
            
            try{
                Class.forName( Jdbc.settings().getDriverClass( mDBType ) ).newInstance();
            } catch ( Exception ex ) {
                ex.printStackTrace();
            }
            
            cfg.setProperty("hibernate.connection.url", Jdbc.settings().getConnectUrl( mDBType ));
            
            String user = Jdbc.settings().getUsername( mDBType );
            if(user != null){
                cfg.setProperty("hibernate.connection.user", user);
            }
            
            String password = Jdbc.settings().getPassword( mDBType );
            if(password != null){
                cfg.setProperty("hibernate.connection.password", password);
            }
            
            String dialect = Jdbc.settings().getHibernateDialect( mDBType );
            if(dialect != null){
                cfg.setProperty("hibernate.dialect", dialect);    
            }
            
            cfg.setProperty("hibernate.query.substitutions", "true 1, false 0, yes 'Y', no 'N'");
            cfg.setProperty("hibernate.connection.pool_size", "1");
            cfg.setProperty("hibernate.proxool.pool_alias", "pool1");
            cfg.setProperty("hibernate.jdbc.batch_size", "0");
            cfg.setProperty("hibernate.jdbc.batch_versioned_data", "true");
            cfg.setProperty("hibernate.jdbc.use_streams_for_binary", "true");
            cfg.setProperty("hibernate.max_fetch_depth", "1");
            cfg.setProperty("hibernate.cache.region_prefix", "hibernate.test");
            cfg.setProperty("hibernate.cache.use_query_cache", "true");
            cfg.setProperty("hibernate.cache.provider_class", "net.sf.hibernate.cache.EhCacheProvider");
            
            cfg.setProperty("hibernate.proxool.pool_alias", "pool1");
            cfg.setProperty("hibernate.proxool.pool_alias", "pool1");
            


            
            
            
            SessionFactory factory = cfg.buildSessionFactory();     
            new SchemaExport(cfg).create(true, true);
            return factory;         
        }
        catch ( MappingException mex )
        {
            mex.printStackTrace();
        }
        catch ( HibernateException hex )
        {
            hex.printStackTrace();
        }       
        return null;
    }

}
