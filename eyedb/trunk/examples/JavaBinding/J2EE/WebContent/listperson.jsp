<?xml version="1.0" encoding="UTF-8" ?>
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<title>Person</title>
</head>
<body>

<%=  pageContext.getServletContext().getInitParameter("database") %>

<jsp:useBean id="query" class="org.eyedb.example.QueryBean" scope="page">
	<jsp:setProperty property="database" value="${pageContext.getServletContext().getInitParameter("database") %>"/>
</jsp:useBean>

<table border="1">
<tr>
<th>#</th>
<th>Firstname</th>
<th>Lastname</th>
</tr>

</table>

</body>
</html>