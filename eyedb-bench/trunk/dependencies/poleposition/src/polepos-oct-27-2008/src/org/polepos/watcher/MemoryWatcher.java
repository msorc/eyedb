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

package org.polepos.watcher;

import org.polepos.util.*;

public class MemoryWatcher implements Watcher {

	private long _startUsedMemory;
	
	private long _maxUsedMemory;
	
	private boolean _stop;
	
	public void start() {
		_stop = false;
		MemoryUtil.clear();
		_maxUsedMemory = _startUsedMemory = MemoryUtil.usedMemory();
		new MemoryWatcherThread().start();
	}

	public void stop() {
		_stop = true;
		monitorMemory();
	}

	public long value() {
		return _maxUsedMemory - _startUsedMemory ;
	}

	private void monitorMemory() {
		long usedMemory = MemoryUtil.usedMemory();
		if(usedMemory > _maxUsedMemory) {
			_maxUsedMemory = usedMemory;
		}
	}
	private class MemoryWatcherThread extends Thread {
		public void run() {
			while(!_stop) {
				monitorMemory();
				ThreadUtil.sleepIgnoreInterruption(10);
			}
		}
	}
}