package org.eyedb.benchmark.polepos;

import org.eyedb.benchmark.framework.Benchmark;
import org.eyedb.benchmark.framework.reporter.ReporterFactory;
import org.polepos.framework.Car;
import org.polepos.framework.Circuit;
import org.polepos.framework.Result;
import org.polepos.framework.SetupProperty;
import org.polepos.framework.Team;
import org.polepos.framework.TurnSetup;
import org.polepos.reporters.Reporter;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public class AdapterReporter extends Reporter {

	class AdapterBenchmark extends Benchmark {

		public String getDescription()
		{
			return description;
		}

		void setDescription(String description)
		{
			this.description = description;
		}

		public String getName()
		{
			return name;
		}

		void setName(String name)
		{
			this.name = name;
		}

		public String getImplementation()
		{
			return implementation;
		}

		void setRunDescription(String implementation)
		{
			this.implementation = implementation;
		}

		public void prepare()
		{
		}

		public void run()
		{
		}

		public void finish()
		{
		}

		private String name;
		private String description;
		private String implementation;
	}

	public AdapterReporter()
	{
		taskCount = 0;
		resultCount = 0;

		reporter = ReporterFactory.newInstance().newReporter();
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
		if (benchmark != null)
			reporter.report(benchmark);
	}

	public void sendToCircuit(Circuit circuit)
	{
		super.sendToCircuit(circuit);

		if (benchmark != null)
			reporter.report( benchmark);

		benchmark = new AdapterBenchmark();

		benchmark.setName( circuit.name());
		benchmark.setDescription( circuit.description());

		taskCount = 0;
	}

	public void reportTaskName(int number, String name)
	{
		taskCount++;
	}

	protected void reportTeam(Team team)
	{
		benchmark.setRunDescription( team.name());
	}

	protected void reportCar(Car car)
	{
		String s = benchmark.getImplementation();
		benchmark.setRunDescription( s + " - " + car.name());
	}

	public void reportSetups(TurnSetup[] setups)
	{
	}

	protected void beginResults()
	{
		resultCount = 0;
	}

	public void reportResult(Result result)
	{
		if ( resultCount == 0) {
			for( SetupProperty sp : result.getSetup().properties())
				benchmark.getResult().add(sp.name(), sp.value());
		}

		benchmark.getResult().add(result.getName(), result.getTime());

		if (resultCount == taskCount - 1)
			benchmark.getResult().next();

		resultCount++;
	}

	private AdapterBenchmark benchmark;
	private org.eyedb.benchmark.framework.Reporter reporter;

	private int taskCount;
	private int resultCount;
}
