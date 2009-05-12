package org.eyedb.example.servlet;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.eyedb.Oid;
import org.eyedb.example.EyeDBBean;
import org.eyedb.example.schema.Car;

/**
 * Servlet implementation class for Servlet: DeleteCarServlet
 *
 */
public class DeleteCarServlet extends javax.servlet.http.HttpServlet  {

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

		if (request.getParameter( "cancel") != null) {
			response.sendRedirect("/eyedb/listcar.jsp");
			return;
		}

		if (request.getParameter( "delete") != null) {
			try {
				EyeDBBean bean = new EyeDBBean();

				bean.setDatabaseName( getServletConfig().getServletContext().getInitParameter("database"));
				bean.setTcpPort( getServletConfig().getServletContext().getInitParameter("tcpPort"));

				bean.openDatabase();
				bean.getDatabase().transactionBegin();

				Oid carOid = new Oid( request.getParameter( "oid"));
				Car car = (Car)bean.getDatabase().loadObject( carOid);

				car.remove();

				bean.getDatabase().transactionCommit();
				bean.closeDatabase();
			}
			catch( org.eyedb.Exception e) {
				throw new ServletException( e);
			}

			response.sendRedirect("/eyedb/listcar.jsp");
		}
	}   	  	    
}