/* Copyright (C) 2004 - 2006  db4objects Inc.  http://www.db4o.com */

package org.polepos.circuits.montreal;

import org.polepos.framework.*;


public class Montreal extends Circuit{

    @Override
    public String description() {
        return "writes and reads 1000 ArrayLists";
    }

    @Override
    protected void addLaps() {
        add(new Lap("write"));
        add(new Lap("read"));
    }
    
    @Override
    public Class requiredDriver() {
        return MontrealDriver.class;
    }


}
