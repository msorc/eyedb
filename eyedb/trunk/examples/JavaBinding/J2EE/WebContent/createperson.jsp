<?xml version="1.0" encoding="UTF-8" ?>
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<title>Create Person object</title>
</head>
<body>

<form name="createPersonForm" action="CreatePersonServlet" method="post">
First name: <input type="text" name="firstname"/><br/>
Last name: <input type="text" name="lastname"/><br/>
<input type="submit" value="Create"/>
</form>

</body>
</html>