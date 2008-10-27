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

package org.polepos.teams.hibernate.data;

import org.polepos.framework.*;

/**
 * @author Herkules
 */
public class HibernateIndexedPilot implements CheckSummable
{
	private		String	mName;		
	private		String	mFirstName;
	private		int		mPoints;
	private		int		mLicenseID;

	
	/**
	 * Default.
	 */
	public HibernateIndexedPilot()
	{
	}
	
	
	/** 
	 * Creates a new instance of Pilot.
	 */
	public HibernateIndexedPilot(String name, int points)
	{
		this.mName=name;
		this.mPoints=points;
	}
	
	/**
	 * Full ctor.
	 */ 
	public HibernateIndexedPilot( String name, String firstname, int points, int licenseID )
	{
		mName		= name;
		mFirstName	= firstname;
		mPoints		= points;
		mLicenseID	= licenseID;
	}
	
	
	public int getPoints()
	{
		return mPoints;
	}
	
	public void setPoints( int points )
	{
		mPoints = points;
	}

	public void addPoints(int points)
	{
		this.mPoints+=points;
	}
	
	public String getName()
	{
		return mName;
	}

	public void setName( String name )
	{
		mName = name;
	}

	public String getFirstName()
	{
		return mFirstName;
	}
	
	public void setFirstName( String firstname )
	{
		mFirstName = firstname;
	}
	
	public int getLicenseID()
	{
		return mLicenseID;
	}
	
	public void setLicenseID( int id ){
		mLicenseID = id;
	}

	public String toString(){
		return mName+"/"+mPoints;
	}
    
    public long checkSum() {
        return getPoints();
    }

}
