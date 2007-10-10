<? $title = 'EyeDB quick tour: create data using the OQL'; include( 'header.php'); ?>

<h1><?= $title ?></h1>

<p>
We show in this section how to create the objects and relationships between
these objects as defined in the <a href="quicktour_odl.php">ODL schema</a>
using the Object Query Language (OQL). The EyeDB OQL
is a superset of the standard ODMG 3 OQL.
<br>
To enter the following OQL statement, we can use the <code>eyedboql</code>
interactive tool included in the distribution.
<h3>Creating a few courses</h3>
We begin by creating a few courses:
<br>
<br>
<table border=1 cellpadding=5 cellspacing=0>
<tr><td class="Code">
<pre>
oodbms := new Course(title : "OODBMS",
		     description : "Object database management systems");

rdbms := new Course(title : "RDBMS",
		    description : "Relational database management systems");

uml := new Course(title : "UML",
		  description : "Unified Modeling Language");

cplus := new Course(title : "C++",
		    description : "C++ Language");

java := new Course(title : "Java",
		   description : "Java Language");

php := new Course(title : "PHP",
		  description : "PHP Language");
</pre>
</td></tr>
</table>
<br>
<br>
A few comments:
<ul>
<li>The <code>new</code> OQL operator allows us to create
a persistent object of the given type
<li>To construct an object, we give a list
of pairs of attribute name and attributes value as shown previously, for
instance: <code>title: "Java", description : "Java Language"</code>.
<br>
If an attribute value is not given during construction, its value
is assigned to the <code>NULL</code> value (not initialised)
<li>The OQL variables <code>oodbms, rdbms</code> and so on are 
not persistent: they are assigned to persistent objects, but
there scope is limitated to the current OQL session.
</ul>
<h3>Creating a few students</h3>
We can now create a few students:
<table border=1 cellpadding=5 cellspacing=0>
<tr><td class="Code">
<pre>
john_harris := new Student(firstname : "John", lastname : "Harris",
			   begin_year : 2002);

suzan_mulder := new Student(firstname : "Suzan", lastname : "Mulder",
			    begin_year : 2002);

francois_martin := new Student(firstname : "Francois", lastname : "Martin",
			       begin_year : 2001);
</pre>
</td></tr>
</table>
<br>
Note that as a <code>Student</code> inherits from a <code>Person</code>,
it includes the <code>firstname</code> and <code>lastname</code>
attributes.
<br>
<h3>Creating two teachers</h3>
We create now two teachers:
<table border=1 cellpadding=5 cellspacing=0>
<tr><td class="Code">
<pre>
eric_viara := new Teacher(firstname : "Eric", lastname : "Viara");

francois_dechelle := new Teacher(firstname : "Francois",
                                 lastname : "Dechelle");
</pre>
</td></tr>
</table>
<h3>Assigning the courses to the teachers</h3>
We deal now with the relationship "in charge" between <code>Course</code>
and <code>Teacher</code>:
<table border=1 cellpadding=5 cellspacing=0>
<tr><td class="Code">
<pre>
oodbms.teacher := eric_viara;
rdbms.teacher := eric_viara;
uml.teacher := francois_dechelle;
cplus.teacher := eric_viara;
java.teacher := francois_dechelle;
php.teacher := francois_dechelle;
</pre>
</td></tr>
</table>
<br>
<br>
Important notices:
<ul>
<li> When we assigned a teacher to a given course (for
instance <code>oodbms.teacher := eric_viara</code>), the inverse attribute
<code>courses</code> in the <code>Teacher</code> class is automatically
updated (the course is added to the <code>courses</code> collections of
the teacher <code>eric_viara</code>) because of the inverse directive
<li> When we change the teacher of a given course (for
instance <code>oodbms.teacher := francois_dechelle</code>),
the inverse attribute <code>courses</code> of <code>eric_viara</code> is updated
(the course is suppressed from its <code>courses</code>
collection) and the inverse attribute <code>courses</code> of <code>francois_dechelle</code>
is also updated (the course is added to the <code>courses</code>
collections of the teacher <code>francois_dechelle</code>).
</ul>
<h3>Assigning the courses to the students</h3>
We deal now with the relationship "registered to" between <code>Student</code>
and <code>Course</code>:
<table border=1 cellpadding=5 cellspacing=0>
<tr><td class="Code">
<pre>
add oodbms to john_harris.courses;
add rdbms to john_harris.courses;

add oodbms to suzan_mulder.courses;
add uml to suzan_mulder.courses;
add java to suzan_mulder.courses;

add oodbms to francois_martin.courses;
add rdbms to francois_martin.courses;
add uml to francois_martin.courses;
add java to francois_martin.courses;
add cplus to francois_martin.courses;
add php to francois_martin.courses;
</pre>
</td></tr></table>
<br>
<br>
Important notices:
<ul>
<li> When we add a course to a given student (for
instance <code>add oodbms to suzan_mulder.courses</code>), the inverse attribute
<code>students</code> in the <code>Course</code> class is automatically
updated (the student is added to the <code>students</code> collections of
the course <code>oodbms</code>) because of the inverse directive
<li> When we suppress a course of a given student (for
instance <code>suppress oodbms from suzan_mulder.courses</code>),
the inverse attribute <code>students</code> of <code>oodbms</code> is updated
(the student is suppressed from its <code>students</code>
collection)
</ul>

<br>
<br>
<? quicktour_nav("quicktour_odl.php", "quicktour_oql_q.php" ); ?>

</p>
<br>
<br>


<? include( 'footer.php'); ?>
