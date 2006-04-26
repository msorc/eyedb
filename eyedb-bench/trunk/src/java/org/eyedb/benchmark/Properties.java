package org.eyedb.benchmark;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public class Properties extends java.util.Properties {

    public boolean getBooleanProperty( String key)
    {
	String value = getProperty( key);

	if (value != null) {
	    try {
		return Boolean.getBoolean( value);
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
}

