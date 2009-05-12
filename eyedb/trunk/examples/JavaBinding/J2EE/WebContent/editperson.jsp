<%@ page contentType="text/html; charset=UTF-8" %>
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

<h1>Edit person</h1>

<c:set var="oid" value='${param["oid"]}' />
<c:set var="person" value="${eyedb.objects[oid]}"/>

<form name="createPersonForm" action="EditPersonServlet" method="post">
<table border="0">
<tr>
<td>First name</td>
<td><input type="text" name="firstname" value="${person.firstname}"/></td>
</tr>
<tr>
<td>Last name</td>
<td><input type="text" name="lastname" value="${person.lastname}"/></td>
</tr>
<tr>
<td>Age</td>
<td><input type="text" name="age" value="${person.age}"/></td>
</tr>
</table>
<input type="hidden" name="oid" value="${person.oid}"/>
<input type="submit" value="Ok"/>
</form>

</body>
</html>