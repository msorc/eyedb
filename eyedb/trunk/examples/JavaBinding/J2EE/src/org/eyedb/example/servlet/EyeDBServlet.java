package org.eyedb.example.servlet;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.eyedb.Connection;
import org.eyedb.Root;

import person.Database;

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
		databaseName = getServletConfig().getServletContext().getInitParameter("database");
		tcpPort = getServletConfig().getServletContext().getInitParameter("tcpPort");

		String[] args = new String[3];
		int i = 0;
		args[i++] = "--user=" + System.getProperty( "user.name");
		args[i++] = "--dbm=default";
		args[i++] = "--port=" + tcpPort;

		Root.init( databaseName, args);

		try {
		    Database.init();
		}
		catch( org.eyedb.Exception e) {
		    throw new ServletException( e);
		}
	}

	protected void openDatabase() throws org.eyedb.Exception
	{
		conn = new Connection();
		database = new Database( databaseName);
		database.open(conn, Database.DBRW);
	}

	protected Database getDatabase()
	{
		return database;
	}

	protected void closeDatabase() throws org.eyedb.Exception
	{
		database.close();
		conn.close();
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