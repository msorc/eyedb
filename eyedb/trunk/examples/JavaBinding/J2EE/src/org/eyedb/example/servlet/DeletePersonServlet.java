package org.eyedb.example.servlet;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.eyedb.Oid;
import org.eyedb.example.EyeDBBean;
import org.eyedb.example.schema.Person;

/**
 * Servlet implementation class for Servlet: CreatePersonServlet
 *
 */
public class DeletePersonServlet extends javax.servlet.http.HttpServlet  {

	/* (non-Java-doc)
	 * @see javax.servlet.http.HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException 
	{
		doPost( request, response);
	}  	

	/* (non-Java-doc)
	 * @see javax.servlet.http.HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		EyeDBBean bean = new EyeDBBean();

		bean.setDatabaseName( getServletConfig().getServletContext().getInitParameter("database"));
		bean.setTcpPort( getServletConfig().getServletContext().getInitParameter("tcpPort"));
		
		try {
			bean.openDatabase();
			bean.getDatabase().transactionBegin();

			Oid personOid = new Oid( request.getParameter( "oid"));
			Person person = (Person)bean.getDatabase().loadObject( personOid);

			person.remove();

			bean.getDatabase().transactionCommit();
			bean.closeDatabase();
		}
		catch( org.eyedb.Exception e) {
			throw new ServletException( e);
		}

		response.sendRedirect("/eyedb/listperson.jsp");
	}   	  	    
}