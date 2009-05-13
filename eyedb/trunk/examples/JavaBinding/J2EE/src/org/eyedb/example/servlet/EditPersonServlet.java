package org.eyedb.example.servlet;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.eyedb.Oid;
import org.eyedb.RecMode;
import org.eyedb.example.EyeDBBean;
import org.eyedb.example.schema.Person;

/**
 * Servlet implementation class for Servlet: CreatePersonServlet
 *
 */
public class EditPersonServlet extends javax.servlet.http.HttpServlet  {

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

			Oid personOid = new Oid( request.getParameter( "oid"));
			Person person = (Person)bean.getDatabase().loadObject( personOid);

			person.setFirstname( request.getParameter( "firstname"));
			person.setLastname( request.getParameter( "lastname"));
			person.setAge( Integer.parseInt( request.getParameter( "age")));

			String[] carOids = request.getParameterValues( "cars");
			{
				if (carOids != null) {
					for (int i = 0; i < carOids.length; i++)
						EyeDBBean.logger.info( "#" + i + " " + carOids[i]);
				}
			}

			while (person.getCarsCount() > 0)
				person.rmvFromCarsColl( person.getCarsAt(0));

			person.store( RecMode.FullRecurs);

			EyeDBBean.logger.info( ""+person.getCarsCount());			
			
			if (carOids != null) {
				for ( int i = 0; i < carOids.length; i++) {
					EyeDBBean.logger.info( "adding oid " + carOids[i]);
					person.addToCarsColl( new Oid(carOids[i]));
				}
			}

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