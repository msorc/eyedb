<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>

<jsp:useBean id="eyedb" class="org.eyedb.example.EyeDBBean" scope="page">
	<jsp:setProperty name="eyedb" property="databaseName" value='<%= pageContext.getServletContext().getInitParameter("database") %>' />
	<jsp:setProperty name="eyedb" property="tcpPort" value='<%= pageContext.getServletContext().getInitParameter("tcpPort") %>' />
</jsp:useBean>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<title>Person</title>
</head>
<body>

<%@ include file="menu.jsp" %>

<h1>Create car</h1>

<form name="createCarnForm" action="CreateCarServlet" method="post">
Model: <input type="text" name="model"/><br/>
Number: <input type="text" name="number"/><br/>
Owner: <select name="owner">
<c:forEach var="person" items="${eyedb.persons}">
	<option value="${person.oid}">${person.firstname} ${person.lastname}</option>
</c:forEach>
</select>
<input type="submit" value="Create"/>
</form>

</body>
</html>