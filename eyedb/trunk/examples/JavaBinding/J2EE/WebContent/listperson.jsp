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

<h1>List persons</h1>

<table border="1">
<tr>
<th>#</th>
<th>First name</th>
<th>Last name</th>
<th>Age</th>
<th>&nbsp;</th>
<th>&nbsp;</th>
<th>&nbsp;</th>
</tr>
<c:set var="counter" value="${1}"/>
<c:forEach var="person" items="${eyedb.persons}">
<tr>
<td>${counter}</td>
<td>${person.firstname}</td>
<td>${person.lastname}</td>
<td>${person.age}</td>
<td>
<form action="viewperson.jsp" method="get">
<input type="hidden" name="oid" value="${person.oid}"/>
<input type="submit" value="View"/>
</form>
</td>
<td>
<form action="editperson.jsp" method="get">
<input type="hidden" name="oid" value="${person.oid}"/>
<input type="submit" value="Edit"/>
</form>
</td>
<td>
<form action="DeletePersonServlet" method="get">
<input type="hidden" name="oid" value="${person.oid}"/>
<input type="submit" value="Delete"/>
</form>
</td>
</tr>
<c:set var="counter" value="${counter + 1}"/>
</c:forEach>
</table>

</body>
</html>