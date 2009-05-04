package org.eyedb.example.servlet;

import java.io.IOException;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.eyedb.Connection;
import org.eyedb.Database;
import org.eyedb.Root;

/**
 * Servlet implementation class for Servlet: EyeDBServlet
 *
 */
public class EyeDBServlet extends javax.servlet.http.HttpServlet implements javax.servlet.Servlet {
    /* (non-Java-doc)
     * @see javax.servlet.http.HttpServlet#HttpServlet()
     */
    public EyeDBServlet() {
	super();
    }   	

    public void init() throws ServletException
    {
	databaseName = getServletConfig().getInitParameter("database");
	tcpPort = getServletConfig().getInitParameter("tcpPort");
    }

    protected void openDatabase() throws ServletException
    {
	String[] args = new String[3];
	int i = 0;
	args[i++] = "--user=" + System.getProperty( "user.name");
	args[i++] = "--dbm=default";
	args[i++] = "--port=" + tcpPort;

	Root.init( databaseName, args);

	try {
	    conn = new Connection();

	    database = new Database( databaseName);

	    database.open(conn, Database.DBRead);
	}
	catch( org.eyedb.Exception e) {
	    throw new ServletException( e);
	}
    }

    protected Database getDatabase()
    {
	return database;
    }
    
    protected void closeDatabase() throws ServletException
    {
	try {
	    database.close();
	    conn.close();
	}
	catch( org.eyedb.Exception e) {
	    throw new ServletException( e);
	}
    }

	/* (non-Java-doc)
     * @see javax.servlet.http.HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
     */
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
	// TODO Auto-generated method stub
    }  	

    /* (non-Java-doc)
     * @see javax.servlet.http.HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
     */
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
	// TODO Auto-generated method stub
    }   	  	    

    protected String databaseName;
    protected String tcpPort;
    private Database database;
    private Connection conn;
}