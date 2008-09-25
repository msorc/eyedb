package org.eyedb.benchmark.polepos.teams.eyedb;

import java.util.*;

import org.polepos.framework.Car;
import org.polepos.framework.Driver;
import org.polepos.framework.TurnSetup;
import org.polepos.framework.CarMotorFailureException;

public abstract class EyeDBDriver extends Driver {

    public void prepare() throws CarMotorFailureException 
    {
    }

    public void backToPit()
    {
    }
}
