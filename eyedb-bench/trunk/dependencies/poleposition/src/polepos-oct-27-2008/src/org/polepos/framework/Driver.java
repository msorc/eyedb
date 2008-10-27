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

/**
 * an implementation of a circuit for a team
 *
 * @author Herkules
 */
public abstract class Driver implements Cloneable
{
    
    private Car mCar;
    
    private TurnSetup mSetup;
    
    private long mCheckSum;
	
    public Car car(){
        return mCar;
    }
    
	/**
	 * take a seat in a car.
	 */
	public void takeSeatIn( Car car, TurnSetup setup ) throws CarMotorFailureException{
        mCar = car;
        mSetup = setup;
        mCheckSum = 0;
    }

	/**
	 * Called just before one of the specific benchmark calls are issued.
	 * Normally opens the database.
	 */
	public abstract void prepare() throws CarMotorFailureException;
	
	
	/**
     * Called after the lap so that the driver can clean up any files it
     * created and close any resources it opened. 
     */
    public abstract void backToPit();
    
    public TurnSetup setup(){
        return mSetup;
    }
    
    /**
     * Collecting a checksum to make sure every team does a complete job  
     */
    public void addToCheckSum(long l){
        mCheckSum += l;
    }
    
    public long checkSum(){
        return mCheckSum; 
    }
    
    public Driver clone(){
        try{
            return (Driver) super.clone();
        }catch(CloneNotSupportedException e){
            e.printStackTrace();
        }
        return null;
    }
    
    public boolean canConcurrent() {
    	return true;
    }
}
