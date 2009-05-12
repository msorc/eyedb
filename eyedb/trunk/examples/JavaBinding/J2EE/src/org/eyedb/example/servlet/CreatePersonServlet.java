package org.eyedb.example.servlet;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.eyedb.RecMode;
import org.eyedb.example.EyeDBBean;
import org.eyedb.example.schema.Person;

/**
 * Servlet implementation class for Servlet: CreatePersonServlet
 *
 */
public class CreatePersonServlet extends javax.servlet.http.HttpServlet  {

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
		try {
			EyeDBBean bean = new EyeDBBean();

			bean.setDatabaseName( getServletConfig().getServletContext().getInitParameter("database"));
			bean.setTcpPort( getServletConfig().getServletContext().getInitParameter("tcpPort"));
			
			bean.openDatabase();
			bean.getDatabase().transactionBegin();

			Person person = new Person( bean.getDatabase());

			person.setFirstname( request.getParameter( "firstname"));
			person.setLastname( request.getParameter( "lastname"));

			// TODO
			//person.setAge()
			//person.setCars()
			
			person.store( RecMode.FullRecurs);

			bean.getDatabase().transactionCommit();
			bean.closeDatabase();
		}
		catch( org.eyedb.Exception e) {
			throw new ServletException( e);
		}

		response.sendRedirect("/eyedb/listperson.jsp");
	}   	  	    
}