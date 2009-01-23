package org.eyedb.benchmark.framework;

import java.io.IOException;
import java.io.FileInputStream;
import java.util.Collection;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public class Properties extends java.util.Properties {
    static final long serialVersionUID = -2400285034232109403L;

    public Properties()
    {
	//	super( System.getProperties());
    }

    public boolean getBooleanProperty( String key)
    {
	String value = getProperty( key);

	if (value != null) {
	    try {
		return value.equals("true") || value.equals("1");
	    }
	    catch( NumberFormatException e) {
	    }
	}

	return false;
    }

    public boolean getBooleanProperty( String key, boolean defaultValue)
    {
	if (getProperty( key) != null)
	    return getBooleanProperty( key);

	return defaultValue;
    }

    public int getIntProperty( String key)
    {
	String value = getProperty( key);

	if (value != null) {
	    try {
		return Integer.parseInt( value);
	    }
	    catch( NumberFormatException e) {
	    }
	}

	return 0;
    }

    public int getIntProperty( String key, int defaultValue)
    {
	if (getProperty( key) != null)
	    return getIntProperty( key);

	return defaultValue;
    }

    public boolean getIntProperty( String key, Collection<Integer> values)
    {
	String s = getProperty(key);

	if ( s == null)
	    return false;

	String[] res = s.split( "[ \t,;]");

	for ( int i = 0; i < res.length; i++) {
	    try {
		values.add( new Integer( Integer.parseInt( res[i])));
	    }
	    catch (NumberFormatException e) {
		return false;
	    }
	}

	return true;
    }

    public long getLongProperty( String key)
    {
	String value = getProperty( key);

	if (value != null) {
	    try {
		return Long.parseLong( value);
	    }
	    catch( NumberFormatException e) {
	    }
	}

	return 0L;
    }

    public long getLongProperty( String key, long defaultValue)
    {
	if (getProperty( key) != null)
	    return getLongProperty( key);

	return defaultValue;
    }

    public String getStringProperty( String key)
    {
	return getProperty(key);
    }

    public String getStringProperty( String key, String defaultValue)
    {
	return getProperty(key, defaultValue);
    }

    public boolean getStringProperty( String key, Collection<String> values)
    {
	String s = getProperty(key);

	if ( s == null)
	    return false;

	String[] res = s.split( "[ \t,;]");

	for ( int i = 0; i < res.length; i++)
	    values.add( res[i]);

	return true;
    }

    public void load( String filename) throws IOException
    {
	load( new FileInputStream( filename));
    }

    public void load( String[] args)
    {
	for ( int i = 0; i < args.length; i++) {
	    Pattern pat = Pattern.compile( "-D([a-zA-Z0-9][.a-zA-Z0-9]*)=(.*)");
	    Matcher m = pat.matcher( args[i]);
	    if (m.matches()) {
		String k = m.replaceFirst( "$1");
		String v = m.replaceFirst( "$2");
		setProperty( k, v);
	    }
	}
    }
}

