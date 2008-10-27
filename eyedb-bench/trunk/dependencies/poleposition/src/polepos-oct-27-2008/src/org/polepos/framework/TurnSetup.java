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

import org.polepos.*;


/**
 * @author Herkules
 */
public class TurnSetup implements Cloneable{
    
    
    private final static PropertiesHandler mProperties = new PropertiesHandler(Settings.CIRCUIT);
    
    private final static String OBJECTCOUNT = "objects";
    private final static String SELECTCOUNT = "selects";
    private final static String UPDATECOUNT = "updates";
    private final static String COMMITCOUNT = "commits";
    private final static String TREEWIDTH = "width";
    private final static String TREEDEPTH = "depth";
    private final static String COMMITINTERVAL = "commitinterval";
    private final static String OBJECTSIZE = "size";

    private final static String[] AVAILABLE_SETTINGS = new String[]{
        OBJECTCOUNT,
        SELECTCOUNT,
        UPDATECOUNT,
        COMMITCOUNT,
        TREEWIDTH,
        TREEDEPTH,
        COMMITINTERVAL,
        OBJECTSIZE
    };
    
    private Map<SetupProperty, SetupProperty> mSettings = new Hashtable<SetupProperty, SetupProperty>();
    
    private TurnSetup deepClone(){
        TurnSetup res = null;
        try {
            res = (TurnSetup)this.clone();
        } catch (CloneNotSupportedException e) {
            e.printStackTrace();
        }
        res.mSettings = new Hashtable<SetupProperty, SetupProperty>();
        for(SetupProperty sp : mSettings.keySet()){
            res.mSettings.put(sp, sp);
        }
        return res;
    }
    
    public static TurnSetup[] read(Circuit circuit){
        
        Vector<TurnSetup> vec = new Vector<TurnSetup>();
        
        for (int i = 0; i < AVAILABLE_SETTINGS.length; i++) {
            
            int[] values = null;
            
            try{
                values = mProperties.getIntArray(circuit.internalName() + "." + AVAILABLE_SETTINGS[i]);
            }catch(Exception e){
                
            }
            
            if(values!= null && values.length > 0){
                int len = values.length;
                
                // make sure that we have enough LapSetup objects in our vector
                // and clone the last if we dont or create a first one
                while(vec.size() < len){
                    if(vec.size() > 0){
                        vec.add((vec.get(vec.size() - 1)).deepClone());
                    }else{
                        vec.add(new TurnSetup());
                    }
                }
                
                // pass values to all LapSetup objects and take the last value as
                // the default if there are more than we have values
                int j = 0;
                Iterator it = vec.iterator();
                while(it.hasNext()){
                    TurnSetup ls = (TurnSetup)it.next();
                    SetupProperty sp =new SetupProperty(AVAILABLE_SETTINGS[i], values[j]); 
                    ls.mSettings.put(sp, sp);
                    if(j < values.length - 1){
                        j++;
                    }
                }
            }
        }
        
        TurnSetup[] res = new TurnSetup[vec.size()];
        vec.toArray(res);
        
        return res;
    }
    
    private int getSetting(String key){
        SetupProperty p = mSettings.get(new SetupProperty(key, 0));
        if(p != null){
            return p.value();
        }
        return 0;
    }
    
    public int getCommitInterval(){
        return getSetting(COMMITINTERVAL);
    }

    public int getCommitCount(){
        return getSetting(COMMITCOUNT);
    }

    public int getObjectCount(){
        return getSetting(OBJECTCOUNT);
    }
    
    public int getSelectCount(){
        return getSetting(SELECTCOUNT);
    }
    
    public int getUpdateCount(){
        return getSetting(UPDATECOUNT);
    }
    
    public int getTreeWidth(){
        return getSetting(TREEWIDTH);
    }
    
    public int getTreeDepth(){
        return getSetting(TREEDEPTH);
    }
    
    public int getObjectSize(){
        return getSetting(OBJECTSIZE);
    }
    
    public int getMostImportantValueForGraph(){
        for (int i = 0; i < AVAILABLE_SETTINGS.length; i++) {
            int val = getSetting(AVAILABLE_SETTINGS[i]);
            if(val > 0){
                return val;
            }
        }
        return 0;
    }
    
    public String getMostImportantNameForGraph(){
        for (int i = 0; i < AVAILABLE_SETTINGS.length; i++) {
            int val = getSetting(AVAILABLE_SETTINGS[i]);
            if(val > 0){
                return AVAILABLE_SETTINGS[i];
            }
        }
        return "";
    }
    
    public Set<SetupProperty> properties() {
        return Collections.unmodifiableSet(mSettings.keySet());
    }
    
}
