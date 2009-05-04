package org.eyedb.example.servlet;

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.eyedb.Connection;
import org.eyedb.Database;
import org.eyedb.Root;

/**
 * Servlet implementation class for Servlet: BasicServlet
 *
 */
public class BasicServlet extends javax.servlet.http.HttpServlet implements javax.servlet.Servlet {
    /* (non-Java-doc)
     * @see javax.servlet.http.HttpServlet#HttpServlet()
     */
    public BasicServlet() 
    {
	super();
    }   	

    public void init() throws ServletException
    {
	databaseName = getServletConfig().getInitParameter("database");
	tcpPort = getServletConfig().getInitParameter("tcpPort");
    }

    /* (non-Java-doc)
     * @see javax.servlet.http.HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
     */
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException 
    {
	String[] args = new String[3];
	int i = 0;
	args[i++] = "--user=" + System.getProperty( "user.name");
	args[i++] = "--dbm=default";
	args[i++] = "--port=" + tcpPort;

	Root.init( databaseName, args);

	Connection conn;
	Database db;

	try {
	    conn = new Connection();

	    db = new Database( databaseName);

	    db.open(conn, Database.DBRead);
	}
	catch( org.eyedb.Exception e) {
	    throw new ServletException( e);
	}

	PrintWriter out = response.getWriter();

	out.println( "<html>");
	out.println( "<body>");

	out.println( "<table>");
	out.println( "<tr><td>Database</td><td>" + databaseName + "</td></tr>");
	out.println( "<tr><td>TCP port</td><td>" + tcpPort + "</td></tr>");
	out.println( "</table>");

	out.println( "<p>");
	out.println( "Connected ok, connection: " + conn);
	out.println( "</p>");

	out.println( "</body>");
	out.println( "</html>");

	try {
	    db.close();
	    conn.close();
	}
	catch( org.eyedb.Exception e) {
	    throw new ServletException( e);
	}
    }  	

    /* (non-Java-doc)
     * @see javax.servlet.http.HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
     */
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException 
    {
	// TODO Auto-generated method stub
    }   	  	    

    private String databaseName;
    private String tcpPort;
}