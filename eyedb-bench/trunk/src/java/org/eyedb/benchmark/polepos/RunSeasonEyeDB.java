package org.eyedb.benchmark.polepos;

import org.eyedb.benchmark.polepos.teams.eyedb.EyeDBTeam;
import org.polepos.circuits.bahrain.Bahrain;
import org.polepos.circuits.barcelona.Barcelona;
import org.polepos.circuits.imola.Imola;
import org.polepos.circuits.melbourne.Melbourne;
import org.polepos.circuits.sepang.Sepang;
import org.polepos.framework.Circuit;
import org.polepos.framework.Team;
import org.polepos.runner.AbstractRunner;

public class RunSeasonEyeDB extends AbstractRunner {

	public static void main(String[] args) {
		new RunSeasonEyeDB().run();
	}

	@Override
	public Circuit[] circuits() {
		return new Circuit[] { 
				new Bahrain(),
				new Barcelona(), 
				new Imola(),
				new Melbourne(), 
				new Sepang(), 
		};
	}

	@Override
	public Team[] teams() {
		return new Team[] { 
				new EyeDBTeam(), 
		};
	}

}
