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

import java.util.*;

import org.polepos.reporters.*;


/**
 * collects all the results for a turn. 
 */
public class TurnResult
{
    private List<Result> results = new ArrayList<Result>();

    public void report(Result result) {
        results.add(result);
    }

    public void requestTaskNames(Reporter reporter) {
        int num = 1;
        Iterator i = results.iterator();
        while(i.hasNext()){
            Result result = (Result)i.next();
            reporter.reportTaskName(num ++, result.getName());
        }
    }
    
    public void report(Reporter reporter) {
        for(Result res : results){
            reporter.reportResult(res);
        }
    }

    
	
}
