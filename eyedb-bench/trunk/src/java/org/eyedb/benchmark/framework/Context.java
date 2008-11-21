package org.eyedb.benchmark.framework;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.IOException;
import java.text.DateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public class Context {

    public Context()
    {
	map = new HashMap<String,String>();

	map.put( "date", getDate());
	map.put( "host", getHost());
	map.put( "uptime", getUptime());
	map.put( "cpu", getCpu());
	map.put( "memory", getMemory());
	map.put( "java", getJava());
    }

    public String get( String key)
    {
	return map.get( key);
    }

    public String put( String key, String value)
    {
	return map.put( key, value);
    }

    public int size()
    {
	return map.size();
    }

    public Set<Map.Entry<String,String>> entrySet()
    {
	return map.entrySet();
    }

    private String getDate()
    {
	DateFormat fmt = DateFormat.getDateTimeInstance();

	return fmt.format(new Date());
    }

    private String getHost()
    {
	return "";
    }

    private String getUptime()
    {
	return "";
    }

    private String getCommandOutput( String command)
    {
	try {
	    Process p = Runtime.getRuntime().exec( command);

	    p.waitFor();

	    BufferedReader b = new BufferedReader( new InputStreamReader( p.getInputStream()));

	    String s;
	    do {
		s = b.readLine();
		System.err.println( "-> " + s);
	    } while ( s != null);

	    return "zob";
	}
	catch( Exception e) {
	    e.printStackTrace();
	}

	return "";
    }

    private String getCpu()
    {
	getCommandOutput( "bash -c \"cat /proc/cpuinfo\"");

	return getCommandOutput( "bash -c \"cat /proc/cpuinfo | grep 'model name' | sort -u | awk -F : '{print $2}'\"");
    }

    private String getMemory()
    {
	getCommandOutput( "free -m");

	return getCommandOutput( "bash -c \"free -m | grep Mem: | awk '{print $2}'\"") + "MB";
    }

    private String getJava()
    {
	return "";
    }

    private Map<String,String> map;


    public static void main( String[] args)
    {
	Context c = new Context();

	for ( Map.Entry<String,String> entry: c.entrySet())
	    System.out.println( entry.getKey() + " : " + entry.getValue());
    }
}
