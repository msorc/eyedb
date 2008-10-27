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

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Properties;
import java.util.StringTokenizer;

/**
 *
 * @author Herkules
 */
public class PropertiesHandler
{
	private final static	String		DBPROPSDIR = ".bdbench";
	private final			String		_fileName;
	private					Properties	_properties;
	
	/** 
	 * Creates a new instance of BenchmarkSettings.
	 */
	public PropertiesHandler( String propertiesname ){
		_fileName = propertiesname;
		load();
	}

	private final String getSettingsDirectoryName(){
		String home = System.getProperty( "user.home", "." );
		return home + File.separator + DBPROPSDIR;
	}
	
	private final String getSettingsFilename(){
        
        String fileName = _fileName;
        
        File file = new File(fileName);
        if(file.exists()){
            String path = file.getAbsolutePath();
            reportSettingsFile(path);
            return path;
        }
        fileName = getSettingsDirectoryName() + File.separator + fileName;
        reportSettingsFile(fileName);
		return fileName;
	}
    
    private void reportSettingsFile(String path){
        System.out.println("\nUsing settings file:");
        System.out.println(path + "\n");
    }
	
	/**
	 * Load default and custom settings.
	 */
	public boolean load()
	{
		try
		{
			_properties = new Properties();
            File file = new File(_fileName);
            if(file.exists()){
                _properties.load(new FileInputStream(file));
            }else{
                _properties.load( PropertiesHandler.class.getClassLoader().getResourceAsStream( _fileName ) );
            }
		}
		catch ( IOException ioex )
		{
			Log.logger.warning( "Cannot load default properties." );
			return false;
		}

		try
		{
			FileInputStream in = new FileInputStream( getSettingsFilename() );
			_properties.load( in );
		}
		catch ( IOException ioex )
		{
			// no custom file present .... thats ok.
			Log.logger.info( "No custom properties found. Using defaults." );

			// create the missing file
			save();
		}
		
		return true;
	}
	

	/**
	 * Persist the custom settings.
	 */
	public boolean save()
	{
		try
		{
			File dir = new File( getSettingsDirectoryName() );
			dir.mkdir();
			FileOutputStream out = new FileOutputStream( getSettingsFilename() );
			_properties.store( out, "DB benchmark settings" );
		}
		catch ( IOException ioex )
		{
			Log.logger.warning( "Cannot save custom settings." );
			return false;
		}
		return true;
	}
	
	
	/**
	 * same as <code>Properties#getProperty()</code>
	 */
	public String get( String key ){
		return _properties.getProperty(key);
	}

	
	/**
	 * same as <code>Properties#getProperty()</code>
	 */
	public String get( String key, String defaultValue ){
		return _properties.getProperty( key, defaultValue );
	}
	
	
	/**
	 * same as <code>Properties#put()</code>
	 */
	public void put( String key, String value ){
		_properties.put( key, value );
	}
	

	/**
	 * retrieve an array that might be formatted like 1,2,3 or [1,2,3] or 1 2 3 or 1;2;3 ...
	 */
	public String[] getArray( String key )
	{
        
        try{
    		String s = get( key );
    		
    		StringTokenizer tokenizer = new StringTokenizer(s, "[ \t,;]" );
    		int len = tokenizer.countTokens();
    		String[] res = new String[ len ];
    		for ( int i = 0; i < len; i++ ){
    			res[i] = tokenizer.nextToken();
    		}
    		
    		return res;
        }catch(Exception e){
            System.out.println("Key not available in " + _fileName +":\n" + key + "\n");
        }
        return null;
	}
	
	
	/**
	 * retrieve an array that might be formatted like 1,2,3 or [1,2,3] or 1 2 3 or 1;2;3 ...
	 */
	public int[] getIntArray( String key ){
		String s = get( key );
		
		StringTokenizer tokenizer = new StringTokenizer(s, "[ \t,;]" );
		int len = tokenizer.countTokens();
		int[] res = new int[ len ];
		for ( int i = 0; i < len; i++ )
		{
			res[i] = Integer.parseInt( tokenizer.nextToken() );
		}
		
		return res;
	}
    
	/**
	 * retrieve a flag
	 */
	public boolean getBoolean( String key ){
		String val = _properties.getProperty(key);
        return Boolean.valueOf( val ).booleanValue();
	}
	
}
