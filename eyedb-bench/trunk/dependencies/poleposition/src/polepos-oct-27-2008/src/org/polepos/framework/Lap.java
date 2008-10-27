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
 * a single (timed) test
 */
public class Lap {
    
    private final String mName;
    
    private boolean mHot;
    
    private boolean mReportResult;
    
    public Lap(String name){
        this.mName = name;
        mHot = false;
        mReportResult = true;
    }
    
    public Lap(String name, boolean hot, boolean reportResult){
        this(name);
        this.mHot = hot;
        this.mReportResult = reportResult;
    }
    
    public String name(){
        return mName;
    }
    
    public boolean hot(){
        return mHot;
    }
    
    public boolean reportResult(){
        return mReportResult;
    }
    
    @Override
    public boolean equals(Object obj) {
        if(obj==this) {
            return true;
        }
        if(obj==null||obj.getClass()!=getClass()) {
            return false;
        }
        Lap key=(Lap)obj;
        return mName.equals(key.mName);
    }
    
    @Override
    public int hashCode() {
        return mName.hashCode();
    }

}
