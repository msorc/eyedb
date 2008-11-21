package org.eyedb.benchmark.framework;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.IOException;
import java.text.DateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

/*
  Exemple of use:

    public static void main( String[] args)
    {
	Context c = new Context();

	for ( Map.Entry<String,String> entry: c.entrySet())
	    System.out.println( entry.getKey() + " : " + entry.getValue());
    }
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

    private String getCommandOutput( String command, String regex)
    {
	try {
	    Process p = Runtime.getRuntime().exec( command);

	    p.waitFor();

	    BufferedReader b = new BufferedReader( new InputStreamReader( p.getInputStream()));

	    Pattern pat = Pattern.compile( regex);
	    String input;

	    do {
		input = b.readLine();

		Matcher m = pat.matcher(input);
		if (m.matches())
		    return m.replaceFirst( "$1");
	    } while ( input != null);

	    return "";
	}
	catch( Exception e) {
	    e.printStackTrace();
	}

	return "";
    }

    private String getDate()
    {
	DateFormat fmt = DateFormat.getDateTimeInstance();

	return fmt.format(new Date());
    }

    private String getHost()
    {
	return getCommandOutput( "hostname", "(.*)");
    }

    private String getUptime()
    {
	return getCommandOutput( "uptime", "(.*)");
    }

    private String getCpu()
    {
	return getCommandOutput( "cat /proc/cpuinfo", "model name\t: (.*)");
    }

    private String getMemory()
    {
	return getCommandOutput( "free -m", "Mem: *([0-9]+) .*") + "MB";
    }

    private String getJava()
    {
	return System.getProperty("java.vm.vendor") + " " + System.getProperty("java.vm.name") + " " + System.getProperty("java.vm.version");
    }

    private Map<String,String> map;

}
