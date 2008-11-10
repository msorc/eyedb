package org.eyedb.benchmark.framework.reporter.openoffice;

import com.sun.star.uno.XComponentContext;
import com.sun.star.lang.XMultiComponentFactory;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public class OpenOfficeReporter {

    public OpenOfficeReporter()
    {
	try {
	    connect();
	}
	catch (Exception e) {
	    e.printStackTrace();
	}
    }

    private void connect() throws Exception 
    {
	remoteContext = com.sun.star.comp.helper.Bootstrap.bootstrap();
	remoteServiceManager = remoteContext.getServiceManager();
    }

    private XComponentContext remoteContext;
    private XMultiComponentFactory  remoteServiceManager;
}
