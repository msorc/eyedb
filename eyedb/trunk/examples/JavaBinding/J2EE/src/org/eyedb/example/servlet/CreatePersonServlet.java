package org.eyedb.example.servlet;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.eyedb.RecMode;
import org.eyedb.example.schema.Person;

/**
 * Servlet implementation class for Servlet: CreatePersonServlet
 *
 */
public class CreatePersonServlet extends EyeDBServlet implements javax.servlet.Servlet {
	/* (non-Java-doc)
	 * @see javax.servlet.http.HttpServlet#HttpServlet()
	 */
	public CreatePersonServlet() {
		super();
	}   	

	private void doCreatePerson( String firstname, String lastname) throws org.eyedb.Exception
	{
		getDatabase().transactionBegin();

		Person p = new Person( getDatabase());

		p.setFirstname( firstname);
		p.setLastname( lastname);

		p.store( RecMode.FullRecurs);

		getDatabase().transactionCommit();
	}

	/* (non-Java-doc)
	 * @see javax.servlet.http.HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException 
	{
	}  	

	/* (non-Java-doc)
	 * @see javax.servlet.http.HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		String firstname = request.getParameter( "firstname");
		String lastname = request.getParameter( "lastname");

		if (firstname != null && !firstname.isEmpty()) {
			try {
				openDatabase();

				doCreatePerson( firstname, lastname);

				closeDatabase();
			}
			catch( org.eyedb.Exception e) {
				throw new ServletException( e);
			}
		}

		response.sendRedirect("/eyedb/listperson.jsp");
	}   	  	    
}