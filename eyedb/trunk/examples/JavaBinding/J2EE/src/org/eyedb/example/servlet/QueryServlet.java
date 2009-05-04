package org.eyedb.example.servlet;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.PrintStream;
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.eyedb.Connection;
import org.eyedb.Database;
import org.eyedb.OQL;
import org.eyedb.ObjectArray;
import org.eyedb.RecMode;
import org.eyedb.Root;

/**
 * Servlet implementation class for Servlet: QueryServlet
 *
 */
public class QueryServlet extends EyeDBServlet implements javax.servlet.Servlet {
    /* (non-Java-doc)
     * @see javax.servlet.http.HttpServlet#HttpServlet()
     */
    public QueryServlet() 
    {
	super();
    }   	

    private void doOQLQuery( String query, PrintWriter out) throws org.eyedb.Exception
    {
	getDatabase().transactionBegin();

	OQL q = new org.eyedb.OQL( getDatabase(), query);
	ObjectArray obj_arr = new ObjectArray();
	q.execute(obj_arr, RecMode.FullRecurs);

	out.println( "<p>Query result (" + obj_arr.getCount() + " objects):<br>");
	out.println( "<table border=\"1\"");
	out.println( "<tr>");
	out.println("<th>#</th>");
	out.println("<th>Object</th>");
	out.println( "</tr>");
	
	ByteArrayOutputStream bos = new ByteArrayOutputStream();
	PrintStream ps = new PrintStream( bos);
	
	for (int i = 0; i < obj_arr.getCount(); i++) {
	    out.println( "<tr>");
	    out.println("<td>Object " + i + "</td>");
	    bos.reset();
	    obj_arr.getObject(i).trace(ps);
	    out.println("<td><pre>" + bos.toString() + "</pre></td>");
	    out.println( "</tr>");
	}
	out.println( "</table>");
	out.println( "</p>");
	
	getDatabase().transactionCommit();
    }

    /* (non-Java-doc)
     * @see javax.servlet.http.HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
     */
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException 
    {
	PrintWriter out = response.getWriter();

	out.println( "<html>");
	out.println( "<body>");

	out.println( "<form name=\"query\" action=\"/eyedb/QueryServlet\" method=\"get\">");
	out.println( "OQL query: <input type=\"text\" name=\"query\">");
	out.println( "<input type=\"submit\" value=\"Ok\">");
	out.println( "</form>");

	String query = request.getParameter( "query");
	
	if (query != null && !query.isEmpty()) {
	    try {
		openDatabase();

		doOQLQuery( query, out);

		closeDatabase();
	    }
	    catch( org.eyedb.Exception e) {
		throw new ServletException( e);
	    }
	}

	out.println( "</body>");
	out.println( "</html>");
    }  	

    /* (non-Java-doc)
     * @see javax.servlet.http.HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
     */
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException 
    {
	// TODO Auto-generated method stub
    }   	  	    
}