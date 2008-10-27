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

public abstract class RdbmsSettings extends PropertiesHandler{

    protected final static String     KEY_CONNECTURL  = "connecturl";
    protected final static String     KEY_DESCRIPTION = "description";
    protected final static String     KEY_DRIVERCLASS = "driverclass";
    protected final static String     KEY_FACTORY     = "factory";
    protected final static String     KEY_PASSWORD    = "password";
    protected final static String     KEY_URL         = "url";
    protected final static String     KEY_USER        = "user";
    protected final static String     KEY_HIBERNATE   = "hibernate";
    protected final static String     KEY_JDBC        = "jdbc";
    protected final static String     KEY_JDO         = "jdo";
    protected final static String     KEY_NAME        = "name";
    protected final static String     KEY_WEBSITE     = "website";
    protected final static String     KEY_BATCH		  = "executebatch";

    public RdbmsSettings(String file) {
        super(file);
    }

    public String getFactory(String name) {
        return get(name + "." + KEY_FACTORY);
    }
    
    public String[] getJdbc(String name){
        return getArray(name + "." + KEY_JDBC);
    }

    public String getUsername(String name) {
        return get(name + "." + KEY_USER);
    }

    public String getPassword(String name) {
        return get(name + "." + KEY_PASSWORD);
    }
    
    public String getURL(String name) {
        return get(name + "." + KEY_URL);
    }
    
    public String getDriverClass( String dbtype ){
        return get( dbtype + "." + KEY_DRIVERCLASS );
    }
    
    public String getHibernateDialect( String dbtype ){
        return get( dbtype + "." + KEY_HIBERNATE );
    }
    
    public String getConnectUrl( String dbtype ){
        return get( dbtype + "." + KEY_CONNECTURL );
    }
    
    public String getWebsite( String dbtype ){
        return get( dbtype + "." + KEY_WEBSITE );
    }
    
    public String getName( String dbtype ){
        return get( dbtype + "." + KEY_NAME );
    }

    public String getDescription(String dbtype) {
        return get( dbtype + "." + KEY_DESCRIPTION );
    }
        
    public boolean getExecuteBatch(String dbtype) {
    	return Boolean.valueOf(get(dbtype + "." + KEY_BATCH, "true")).booleanValue();
	}
    
    public boolean getBoolean(String key){
        String str = get(key);
        if(str == null || str.length() == 0){
            return false;
        }
        String[] canstartwith = new String[]{
            "1",
            "y",
            "Y",
            "t",
            "T"
        };
        for( String start : canstartwith){
            if(str.startsWith(start)){
                return true;
            }
        }
        return false;
    }

}
