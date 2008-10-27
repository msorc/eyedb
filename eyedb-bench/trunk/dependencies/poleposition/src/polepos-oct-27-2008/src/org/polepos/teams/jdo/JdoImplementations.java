/* Copyright (C) 2006  db4objects Inc.  http://www.db4o.com */

package org.polepos.teams.jdo;


/**
 * @exclude
 */
public class JdoImplementations {
    
    public static void voaNotAvailable(){
        throw new RuntimeException("Versant Open Access functionality is not available.");
    }

}
