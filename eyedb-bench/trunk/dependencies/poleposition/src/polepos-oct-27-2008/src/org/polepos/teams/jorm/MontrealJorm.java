

package org.polepos.teams.jorm;

import de.ama.db.DB;
import de.ama.db.OidIterator;
import de.ama.db.Persistent;
import org.polepos.circuits.montreal.MontrealDriver;
import org.polepos.framework.CheckSummable;
import org.polepos.teams.jorm.data.ListHolder;


public class MontrealJorm extends JormDriver implements MontrealDriver {

    public void prepare(){
        DB.session().delete(Persistent.class);
    }


    public void write() {
        
        int count = 1000;
        int elements = setup().getObjectSize();
        
        for (int i = 1; i<= count; i++) {
            store(ListHolder.generate(i, elements));
        }
        commit();
    }

    public void read() {
        OidIterator list = db().getObjects(ListHolder.class);
        for (int i = 0; i < list.size(); i++) {
            Object o = list.get(i);
            if (o instanceof CheckSummable) {
                addToCheckSum(((CheckSummable) o).checkSum());
            }
        }


    }

}
