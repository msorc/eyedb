package org.eyedb.benchmark.framework.reporter.openoffice;

import org.eyedb.benchmark.framework.Benchmark;
import org.eyedb.benchmark.framework.Reporter;

import com.sun.star.container.XNameAccess;
import com.sun.star.lang.XMultiComponentFactory;
import com.sun.star.sheet.XSpreadsheet;
import com.sun.star.sheet.XSpreadsheetDocument;
import com.sun.star.sheet.XSpreadsheets;
import com.sun.star.uno.UnoRuntime;
import com.sun.star.uno.XComponentContext;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public class OpenOfficeReporter implements Reporter {

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

    protected XSpreadsheet getSpreadsheet( XSpreadsheetDocument doc, String name) 
    {
	XSpreadsheets sheets = doc.getSheets();
	XSpreadsheet s = null;

	try {
	    XNameAccess access = (XNameAccess)UnoRuntime.queryInterface( XNameAccess.class, sheets);
	    s = (XSpreadsheet) access.getByName( name);
	}
	catch (Exception ex) {
	}

	return s;
    } 
  
    public void report( Benchmark benchmark)
    {
    }

    private XComponentContext remoteContext;
    private XMultiComponentFactory  remoteServiceManager;
    private XSpreadsheetDocument document;
}
