<? $title = 'EyeDB quick tour: define a schema using ODL'; include( 'header.php'); ?>

<h1><?= $title ?></h1>

<p>
We want to modelize a very simple university environment:
<ul>
<li>A course is caracterized by a title and a description
<li>A student is caracterized by a firstname,
a lastname and a registration year
<li>A teacher is caracterized by a firstname and
a lastname
<li>A teacher is in charge of one or several courses
<li>A student is registered to one or several courses
</ul>
The following schema implements the previous model by introducing:
<ul>
<li>The given attributes: firstname, lastname, begin_year, title, description
<li>A superclass Person which factorizes the firstname and lastname
attributes of students and teachers
<li>Relationships between:
<ul>
<li>Student and registered courses (called "registered to")
<li>Teacher and in charge courses (called "in charge")
</ul>
</ul>
Here is the Object Definition Language (ODL) implementation of the previous
model:
<br>
<br>
<table border=1 cellpadding=5 cellspacing=0>
<tr><td class="Code">
<pre>
class Person {
  attribute string firstname;
  attribute string lastname;
};

class Student extends Person {
  attribute short begin_year;
  relationship set<Course *> courses inverse students;
};

class Course {
  attribute string title;
  attribute string description;
  relationship set<Student *> students inverse courses;
  relationship Teacher *teacher inverse courses;
};

class Teacher extends Person {
  relationship set<Course *> courses inverse teacher;
};
</pre>
</td></tr></table>
<br>
<br>
View the <a href="quicktour/student.odl" target="_blank">ODL file</a>
<br>
<br>
A few comments:
<ul>
<li>The inheritance between the classes <code>Student</code> and
<code>Person</code> and between
<code>Teacher</code> and <code>Person</code>
is indicated by the <code>extends</code> keyword.
<li>The <code>set&lt;Course *&gt; courses</code> attribute in
the <code>Student</code> class and
the <code>set&lt;Student *&gt; students</code> attribute in
the <code>Course</code> class
denote the relationship
between the <code>Student</code> and its registered courses
<li>The <code>set&lt;Course *&gt; courses</code> attribute in
the <code>Teacher</code> class 
and the <code>Teacher *teacher</code> attribute in the
<code>Course</code> class denotes the relationship
between the <code>Teacher</code> and its in charge <code>Courses</code>
<li>The inverse directives on the relationship attributes are there to
indicate to EyeDB to maintain the referential integrity of
the relationships
</ul>
To enter this schema in a database, we must use the <code>eyedbodl</code>
tool which checks the syntax and semantics of the schema and in
case of succes submit the given schema to the given database.

<br>
<br>
<table border=0>

<tr><td>Next</td>
<td><a href="quicktour_oql_c.php">Create data using OQL</a></td></tr>

<tr><td>Top</td>
<td><a href="quicktour.php">EyeDB Quick Tour</a></td>
</tr></table>
</p>
<br>
<br>

<? include( 'footer.php'); ?>
