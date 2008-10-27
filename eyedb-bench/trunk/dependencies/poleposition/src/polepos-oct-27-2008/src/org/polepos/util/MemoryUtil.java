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


package org.polepos.util;


public class MemoryUtil {

	private static final int GC_TIMES = 5;
	private static Runtime _runtime = Runtime.getRuntime();

	public static long usedMemory() {
		return _runtime.totalMemory() - _runtime.freeMemory();
	}

	public static void clear() {
		for (int i = 0; i < GC_TIMES; ++i) {
			System.gc();
			System.runFinalization();
		}
		try {
			Thread.sleep(10);
		} catch (InterruptedException e) {
			// ignore
		}
	}
}