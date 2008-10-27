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

import org.polepos.framework.*;
import org.polepos.util.*;

import com.db4o.*;
import com.db4o.ext.*;
import com.db4o.query.*;

/**
 * @author Herkules, Andrew Zhang
 */
public class Db4oDriver extends Driver {

	private ExtObjectContainer _container;

	public void takeSeatIn(Car car, TurnSetup setup)
			throws CarMotorFailureException {
		super.takeSeatIn(car, setup);
	}

	public void prepare() {
		_container = ((Db4oCar) car()).createObjectContainer();
	}

	public void backToPit() {
		_container.close();
		// give the weak reference collector thread time to end
		ThreadUtil.sleepIgnoreInterruption(500);
	}

	public ExtObjectContainer db() {
		return _container;
	}

	protected ObjectSet doQuery(Query q) {
		ObjectSet result = q.execute();
		while (result.hasNext()) {
			Object o = result.next();
			if (o instanceof CheckSummable) {
				addToCheckSum(((CheckSummable) o).checkSum());
			}
		}
		result.reset();
		return result;
	}

	protected void doQuery(Class clazz) {
		Query q = db().query();
		q.constrain(clazz);
		doQuery(q);
	}

	protected void begin() {
		// db4o always works in a transaction so a begin call
		// is not needed.
	}

	protected void commit() {
		_container.commit();
	}

	protected void store(Object obj) {
		_container.set(obj);
	}
}
