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

package org.polepos.framework;

import java.lang.reflect.*;
import java.util.*;

import org.polepos.watcher.*;

/**
 * a set of timed test cases that work against the same data
 */
public abstract class Circuit{
    
    private final List<Lap> mLaps;
    
    private final TurnSetup[] mLapSetups;
        
    // TODO: watcher can be installed, and should be sorted, i.e. memory watcher
	// should start before time watcher
    private TimeWatcher _timeWatcher;
    
    private MemoryWatcher _memoryWatcher;
    
    private FileSizeWatcher _fileSizeWatcher;
    
    protected Circuit(){
        initWatchers();
        mLaps = new ArrayList<Lap>();
        mLapSetups = TurnSetup.read(this);
        addLaps();
    }

	private void initWatchers() {
		_timeWatcher = new TimeWatcher();
		_memoryWatcher = new MemoryWatcher();
		_fileSizeWatcher = new FileSizeWatcher();
	}
    
	/**
     * public official name for reporting
	 */
    public final String name(){
        String name = internalName();
        return name.substring(0,1).toUpperCase() + name.substring(1);
    }

    /**
     * internal name for BenchmarkSettings.properties
     */
    public final String internalName(){
        String name = this.getClass().getName();
        int pos = name.lastIndexOf(".");
        return name.substring(pos + 1).toLowerCase();
    }
    
    /**
     * describes the intent of this circuit, what it wants to test
     */
	public abstract String description();

    /**
     * @return the driver class needed to run on this Circuit
     */
    public abstract Class requiredDriver();
    
    /**
     * @return the methods that are intended to be run 
     */
    protected abstract void addLaps();
    
    public void add(Lap lap){
        mLaps.add(lap);
    }
    
    /**
     * setups are needed for reporting
     */
    public TurnSetup[] lapSetups(){
        return mLapSetups;
    }
    
    public List<Lap> laps() {
        return Collections.unmodifiableList(mLaps);
    }
    
    /**
     * calling all the laps for all the lapSetups
     */
    public TurnResult[] race( Team team, Car car, Driver driver){
  
        TurnResult[] results = new TurnResult[ mLapSetups.length ];

        int index = 0;
        
        Driver[] drivers = null;
    	
        boolean concurrent = team.isConcurrent() && driver.canConcurrent();
        
        if (concurrent) {
			drivers = new Driver[team.getConcurrentCount()];
			drivers[0] = driver;
			for (int i = 1; i < drivers.length; ++i) {
				drivers[i] = driver.clone();
			}
		}
        
        for(TurnSetup setup : mLapSetups) {
            
            TurnResult result = new TurnResult(); 
            results[index++] = result;
            
            team.setUp();
            
            try {
				if (concurrent) {
					for (int i = 0; i < drivers.length; ++i) {
						drivers[i].takeSeatIn(car, setup);
					}
				} else {
					driver.takeSeatIn(car, setup);
				}
			} catch (CarMotorFailureException e1) {
				e1.printStackTrace();
				break;
			}
            
            
            boolean first = true;
            
            for(Lap lap : mLaps) {
                
                
                Method method = null; 
            
                try {
                    method = driver.getClass().getDeclaredMethod(lap.name(), (Class[])null);
                } catch (SecurityException e) {
                    e.printStackTrace();
                } catch (NoSuchMethodException e) {
                    e.printStackTrace();
                }
                
                
                if( ! lap.hot() ){
                    if(first){
                       first = false;
                    }else{
                    	if (concurrent) {
							for (Driver d : drivers) {
								d.backToPit();
							}
						} else {
							driver.backToPit();
						}
                    }
                    
                    try {
                    	if (concurrent) {
							for (Driver d : drivers) {
								d.prepare();
							}
						} else {
							driver.prepare();
						}
                    } catch (CarMotorFailureException e) {
                        e.printStackTrace();
                    }        
                }
                
                RunLapThread[] threads = null;
                if(concurrent) {
                	threads = new RunLapThread[drivers.length];
                	for(int i = 0; i < drivers.length; ++i) {
                		threads[i] = new RunLapThread(method, drivers[i]);
                	}
                }
                
                _memoryWatcher.start();
                _timeWatcher.start();
                _fileSizeWatcher.monitorFile(team.databaseFile());
                _fileSizeWatcher.start();
                
                try {
                	if(concurrent) {
						for (RunLapThread t : threads) {
							t.start();
						}
					} else {
						method.invoke(driver, (Object[]) null);
					}
                } catch (Exception e) {
                    System.err.println("Exception on calling method " + method);
                    e.printStackTrace();
                }
                
                if(concurrent) {
					for (RunLapThread t : threads) {
						try {
							t.join();
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
					}
				}
                
                _timeWatcher.stop();
                _memoryWatcher.stop();
                _fileSizeWatcher.stop();
                
                if(lap.reportResult()){
                	long time = (Long)_timeWatcher.value();
                	long memory = (Long) _memoryWatcher.value();
                	long databaseSize = (Long) _fileSizeWatcher.value();
                	
                    result.report(new Result(this, team, lap, setup, index, time, memory, databaseSize, driver.checkSum()));
                }
            }
            
            if(concurrent) {
				for (Driver d : drivers) {
					d.backToPit();
				}
			} else {
				driver.backToPit();
			}
            
            team.tearDown();
        }
        return results;
    }
    
}

class RunLapThread extends Thread {
	Method method;
	Driver driver;
	public RunLapThread(Method method, Driver driver) {
		super();
		this.method = method;
		this.driver = driver;
	}
	public void run () {
		try {
			method.invoke(driver, (Object[]) null);
		} catch (Exception e) {
            System.err.println("Exception on calling method " + method);
            e.printStackTrace();
        }
	}
}