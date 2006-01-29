<? $title = 'EyeDB quick tour: query data using the OQL'; include( 'header.php'); ?>

<h1><?= $title ?></h1>

<p>
We show in this section how to query objects 
as defined in the <a href="quicktour_odl.php">ODL schema</a>
using the Object Query Language (OQL). 
<h2>Looking for persons</h2>
<table border=1 cellpadding=5 cellspacing=0>
<tr><td class="Code">
<pre>
select Student;
select Teacher;
select Person;

select Student.firstname = "Francois";
select firstname + " " + lastname from Student where firstname = "Francois";

select Teacher.firstname = "Francois";
select Person.firstname = "Francois";

</pre>
</td></tr>
</table>
<h2>Looking for courses</h2>
<table border=1 cellpadding=5 cellspacing=0>
<tr><td class="Code">
<pre>
select description from Course where title = "OODBMS";
select * from Course where title = "OODBMS";
</pre>
</td></tr>
</table>
<h2>Looking for Teacher teaching a given course</h2>
<table border=1 cellpadding=5 cellspacing=0>
<tr><td class="Code">
<pre>
select x.teacher.firstname + " " + x.teacher.lastname from Course x
       where x.title = "OODBMS";

select (Course.title = "OODBMS").teacher.lastname;
</pre>
</td></tr>
</table>
<h2>Looking for courses teached by a given teacher</h2>
<table border=1 cellpadding=5 cellspacing=0>
<tr><td class="Code">
<pre>
// Using Course class:
select title from Course where teacher.lastname = "Dechelle";
// Using Teacher class:
select x.courses[?].title from Teacher x where x.lastname = "Dechelle";
</pre>
</td></tr>
</table>
<h2>Looking for courses learnt by a given student</h2>
<table border=1 cellpadding=5 cellspacing=0>
<tr><td class="Code">
<pre>
// Using Student class:
select s.courses[?].title from Student s where s.lastname = "Mulder";

select s.courses[?].title from Student s where s.lastname = "Mulder" 
                                               and s.firstname = "Suzan";

// Using Course class:
select c.title from Course c where c.students[?].lastname = "Mulder";

select c.title from Course c where c.students[?] =
   (select one s from Student s where s.lastname = "Mulder" and
                                      s.firstname = "Suzan");
</pre>
</td></tr></table>
<br>
<br>
View the whole <a href="quicktour/student.oql" target="_blank">OQL file</a>

<br>
<br>
<table border=0>

<tr><td>Next</td>
<td><a href="quicktour_cplus_c.php">Create data using the C++ binding</a></td></tr>
<tr><td>Previous&nbsp;</td>
<td><a href="quicktour_oql_c.php">Create data using OQL</a></td></tr>
<tr><td>Top</td>
<td><a href="quicktour.php">EyeDB Quick Tour</a></td>
</tr></table>

</p>

<br>
<br>

<? include( 'footer.php'); ?>
