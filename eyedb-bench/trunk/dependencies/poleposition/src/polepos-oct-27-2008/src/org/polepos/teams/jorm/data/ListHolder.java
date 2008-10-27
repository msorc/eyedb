/* Copyright (C) 2004 - 2006  db4objects Inc.  http://www.db4o.com */

package org.polepos.teams.jorm.data;

import de.ama.db.DB;
import de.ama.db.PersistentMarker;
import org.polepos.framework.CheckSummable;

import java.util.List;


public class ListHolder implements CheckSummable,PersistentMarker{
    
    private List list;
    
    public ListHolder(){
        
    }
    
    public static ListHolder generate(int index, int elements){
        ListHolder lh = new ListHolder();
        List list = DB.createSmallList();
        lh.setList(list);
        for (int i = 0; i < elements; i++) {
            list.add( new Integer(i));
        }
        return lh;
    }


    public long checkSum() {
        return list.size();
    }

    public List getList() {
        return list;
    }
    
    public void setList(List list) {
        this.list = list;
    }

}
