<%@ page contentType="text/html; charset=UTF-8" %>
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>

<jsp:useBean id="query" class="org.eyedb.example.EyeDBBean" scope="page">
	<jsp:setProperty name="query" property="databaseName" value='<%= pageContext.getServletContext().getInitParameter("database") %>' />
	<jsp:setProperty name="query" property="tcpPort" value='<%= pageContext.getServletContext().getInitParameter("tcpPort") %>' />
	<jsp:setProperty name="query" property="oid" value='<%= request.getParameter("oid") %>' />
</jsp:useBean>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<title>Person</title>
</head>
<body>

<%@ include file="menu.jsp" %>

<h1>View person</h1>

<c:set var="person" value="${query.person}"/>

<table border="1">
<tr>
<th>First name</th>
<td>${person.firstname}</td>
</tr>
<tr>
<th>Last name</th>
<td>${person.lastname}</td>
</tr>
</table>

</body>
</html>