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

package org.polepos.teams.db4o;

import org.polepos.circuits.imola.*;
import org.polepos.data.*;


public class ImolaDb4o extends Db4oDriver implements ImolaDriver {

    private long[] ids;

    public void store() {
        begin();
        ids = new long[setup().getSelectCount()];
        int count = setup().getObjectCount();
        for (int i = 1; i <= count; i++) {
            storePilot(i);
        }
        commit();
    }

    public void retrieve() {
        for (long id : ids) {
            Pilot pilot = (Pilot) db().getByID(id);
            db().activate(pilot, 1);
            if (pilot == null) {
                System.err.println("Object not found by ID.");
            } else {
                addToCheckSum(pilot.getPoints());
            }
        }
    }

    private void storePilot(int idx) {
        Pilot pilot = new Pilot("Pilot_" + idx, "Jonny_" + idx, idx, idx);
        store(pilot);
        if (idx <= setup().getSelectCount()) {
            ids[idx - 1] = db().getID(pilot);
        }
        if (isCommitPoint(idx)) {
            commit();
            begin();
        }
    }

    private boolean isCommitPoint(int idx) {
        int commitInterval = setup().getCommitInterval();
        return commitInterval > 0 && idx % commitInterval == 0 && idx < setup().getObjectCount();
    }
}
