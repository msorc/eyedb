package org.eyedb.benchmark.polepos;

import org.polepos.framework.Car;
import org.polepos.framework.Circuit;
import org.polepos.framework.Result;
import org.polepos.framework.Team;
import org.polepos.framework.TurnSetup;
import org.polepos.framework.SetupProperty;
import org.polepos.reporters.Reporter;
import java.io.IOException;
import java.io.PrintStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Formatter;

public class SimpleReporter extends Reporter {

    private static final int defaultColumnWidth = 15;
    private static final char defaultColumnSeparator = ',';

    public SimpleReporter( PrintStream out)
    {
	this.out = out;

	columnWidth = defaultColumnWidth;
	columnSeparator = defaultColumnSeparator;

	taskNames = new ArrayList<String>();
	setupProperties = new ArrayList<String>();

	resultCount = 0;
    }

    public SimpleReporter( String filename) throws IOException
    {
	this( new PrintStream( filename));
    }

    public SimpleReporter()
    {
	this( System.out);
    }

    public String file()
    {
	return "";
    }
    
    public boolean append()
    {
	return false;
    }
    
    public void startSeason()
    {
    }
    
    public void endSeason()
    {
	out.println();
    }
    
    public void sendToCircuit(Circuit circuit)
    {
        super.sendToCircuit(circuit);
	out.println();
	out.println();
        out.println("Bench: " + circuit.name());
        out.println(circuit.description());
	out.println();

	taskNames.clear();
    }

    public void reportTaskName(int number, String name)
    {
	taskNames.add( name);
    }
    
    protected void reportTeam(Team team)
    {
	out.print( team.name());
    }
    
    protected void reportCar(Car car)
    {
	out.println( " - " + car.name());
    }

    public void reportSetups(TurnSetup[] setups)
    {
	setupProperties.clear();

	for( SetupProperty sp : setups[0].properties()) {
	    setupProperties.add( sp.name());
	}

	printColumnHeaders();
    }

    private void printColumnHeaders()
    {
	Formatter fmt = new Formatter( out);

	for( String setupProperty : setupProperties)
	    fmt.format( "%" + columnWidth + "s%c", setupProperty, columnSeparator);

	for ( int i = 0; i < taskNames.size(); i++) {
	    char c = (i != taskNames.size() - 1) ? columnSeparator : ' ';
	    fmt.format( "%" + columnWidth + "s%c", taskNames.get(i) + " (ms)", c);
	}

	out.println();
    }

    protected void beginResults()
    {
	resultCount = 0;
    }

    public void reportResult(Result result)
    {
	Formatter fmt = new Formatter( out);

	if ( resultCount == 0) {
	    for( SetupProperty sp : result.getSetup().properties())
		fmt.format( "%" + columnWidth + "s%c", sp.value(), columnSeparator);
	}

	char c = ( resultCount != taskNames.size() - 1) ? columnSeparator : ' ';
	
	fmt.format( "%" + columnWidth + "s%c", result.getTime(), c);

	if (resultCount == taskNames.size() - 1)
	    out.println();

	resultCount++;
    }

    private PrintStream out;

    private int columnWidth;
    private char columnSeparator;

    private List<String> taskNames;
    private List<String> setupProperties;

    private int resultCount;
}
