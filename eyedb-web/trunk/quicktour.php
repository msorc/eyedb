<? $title = 'EyeDB Quick Tour'; include( 'header.php'); ?>

<? $width = 540 ?>

<h1><?= $title ?></h1>

<p>
This page provide a very quick tour of the EyeDB OODBMS by
showing some of its capabilities.
<br>
<br>
We introduce here the ways to:
<ul>
<li><a href="#odl">Define a schema using the Object Definition Language (ODL)</a></li>
<li><a href="#oqlm">Create data using the Object
Query Language (OQL)</a></li>
<li><a href="#oqlq">Query data using OQL</a></li>
<li><a href="#cm">Create data using the C++ binding</a></li>
<li><a href="#cq">Query data using the C++ binding and OQL</a></li>
<li><a href="#jm">Create data using the Java binding</a></li>
<li><a href="#jq">Query data using the C++ binding and OQL</a></li>
</ul>
<h2><a name="odl">Defining a schema using ODL</a></h2>
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
<li>Student and registered courses
<li>Teacher and in charge courses
</ul>
</ul>
Here is the Object Definition Language (ODL) implementation of the previous
model:
<br>
<br>
<table border=1 cellpadding=5 cellspacing=0>
<tr><td width=<?= $width ?> >
<pre class="code">
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
<h2><a name="oqlm">Create data using OQL</a></h2>

<h3>Creating a few courses</h3>
<table border=1 cellpadding=5 cellspacing=0>
<tr><td width=<?= $width ?> >
<pre class="code">
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
<li>The <code>new</code> OQL operator...
<li>To construct on object, one must give its attribute values...
<li>The OQL variables <code>oodbms, rdbms</code> etc. are 
</ul>
<h3>Creating a few students</h3>
<table border=1 cellpadding=5 cellspacing=0>
<tr><td width=<?= $width ?> >
<pre class="code">
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
<br>
<h3>Creating two teachers</h3>
<table border=1 cellpadding=5 cellspacing=0>
<tr><td width=<?= $width ?> >
<pre class="code">
eric_viara := new Teacher(firstname : "Eric", lastname : "Viara");

francois_dechelle := new Teacher(firstname : "Francois",
                                 lastname : "Dechelle");
</pre>
</td></tr>
</table>
<h3>Assigning the courses to the teachers</h3>
<table border=1 cellpadding=5 cellspacing=0>
<tr><td width=<?= $width ?> >
<pre class="code">
oodbms.teacher := eric_viara;
rdbms.teacher := eric_viara;
uml.teacher := francois_dechelle;
cplus.teacher := eric_viara;
java.teacher := francois_dechelle;
php.teacher := francois_dechelle;
</pre>
</td></tr>
</table>
<h3>Assigning the courses to the students</h3>
<table border=1 cellpadding=5 cellspacing=0>
<tr><td width=<?= $width ?> >
<pre class="code">
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

<h2><a name="oqlq">Query data using OQL</a></h2>

<h3>Looking for persons</h3>
<table border=1 cellpadding=5 cellspacing=0>
<tr><td width=<?= $width ?> >
<pre class="code">
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
<h3>Looking for courses</h3>
<table border=1 cellpadding=5 cellspacing=0>
<tr><td width=<?= $width ?> >
<pre class="code">
select description from Course where title = "OODBMS";
select * from Course where title = "OODBMS";
</pre>
</td></tr>
</table>
<h3>Looking for Teacher teaching a given course</h3>
<table border=1 cellpadding=5 cellspacing=0>
<tr><td width=<?= $width ?> >
<pre class="code">
select x.teacher.firstname + " " + x.teacher.lastname from Course x
       where x.title = "OODBMS";

select (Course.title = "OODBMS").teacher.lastname;
</pre>
</td></tr>
</table>
<h3>Looking for courses teached by a given teacher</h3>
<table border=1 cellpadding=5 cellspacing=0>
<tr><td width=<?= $width ?> >
<pre class="code">
// Using Course class:
select title from Course where teacher.lastname = "Dechelle";
// Using Teacher class:
select x.courses[?].title from Teacher x where x.lastname = "Dechelle";
</pre>
</td></tr>
</table>
<h3>Looking for courses learnt by a given student</h3>
<table border=1 cellpadding=5 cellspacing=0>
<tr><td width=<?= $width ?> >
<pre class="code">
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
<h2><a name="cm">Create data using the C++ binding</a></h2>

<table border=1 cellpadding=5 cellspacing=0>
<tr><td width=<?= $width ?> >
<pre class="code">
static void create(eyedb::Database *db)
{
  Course *perl = new Course(db);
  perl->setTitle("Perl");
  perl->setDescription("Perl Language");

  Course *python = new Course(db);
  python->setTitle("Python");
  python->setDescription("Python Language");

  Course *eyedb_ = new Course(db);
  eyedb_->setTitle("EyeDB");
  eyedb_->setDescription("EyeDB OODBMS");

  Student *henri_muller = new Student(db);
  henri_muller->setFirstname("Henri");
  henri_muller->setLastname("Muller");
  henri_muller->setBeginYear(2003);

  Student *jacques_martin = new Student(db);
  jacques_martin->setFirstname("Jacques");
  jacques_martin->setLastname("Martin");
  jacques_martin->setBeginYear(2003);
  
  Student *mary_kiss = new Student(db);
  mary_kiss->setFirstname("Mary");
  mary_kiss->setLastname("Kiss");
  mary_kiss->setBeginYear(2003);

  Teacher *max_first = new Teacher(db);
  max_first->setFirstname("Max");
  max_first->setLastname("First");

  Teacher *georges_shorter = new Teacher(db);
  georges_shorter->setFirstname("Georges");
  georges_shorter->setLastname("Shorter");

  perl->setTeacher(max_first);
  python->setTeacher(max_first);
  eyedb_->setTeacher(georges_shorter);

  henri_muller->addToCoursesColl(perl);
  henri_muller->addToCoursesColl(eyedb_);

  jacques_martin->addToCoursesColl(python);
  jacques_martin->addToCoursesColl(perl);
  jacques_martin->addToCoursesColl(eyedb_);

  mary_kiss->addToCoursesColl(python);

  // storing objects to database
  henri_muller->store(eyedb::RecMode::FullRecurs);
  jacques_martin->store(eyedb::RecMode::FullRecurs);
  mary_kiss->store(eyedb::RecMode::FullRecurs);
}
</pre>
</td></tr></table>
<br>
<br>
<h2><a name="cq">Query data using the C++ binding and OQL</a></h2>
<table border=1 cellpadding=5 cellspacing=0>
<tr><td width=<?= $width ?> >
<pre class="code">
static void query_students(eyedb::Database *db)
{
  eyedb::OQL oql(db, "select Student");
  eyedb::ObjectArray obj_arr;
  oql.execute(obj_arr);
  unsigned int count = obj_arr.getCount();
  for (int n = 0; n < count; n++) {
    Student *s = Student_c(obj_arr[n]);
    if (s) {
      std::cout << s->getFirstname() << " " << s->getLastname() << std::endl;
    }
  }
}

static void query_courses(eyedb::Database *db,
			  const char *firstname, const char *lastname)
{
  eyedb::OQL oql(db, "select c from Course c where "
		 "c.teacher.lastname = \"%s\" && "
		 "c.teacher.firstname = \"%s\"", lastname, firstname);
  eyedb::ObjectArray obj_arr;
  oql.execute(obj_arr);
  unsigned int count = obj_arr.getCount();
  for (int n = 0; n < count; n++) {
    Course *c = Course_c(obj_arr[n]);
    if (c) {
      std::cout << firstname << " " << lastname << ": " <<
	c->getTitle() << " " << c->getDescription() << std::endl;
    }
  }
}
</pre>
</td></tr></table>
<br>
<br>

View the whole <a href="quicktour/student_test.cc" target="_blank">C++ file</a>
<h2><a name="jm">Create data using the Java binding</a></h2>
<table border=1 cellpadding=5 cellspacing=0>
<tr><td width=<?= $width ?> >
<pre class="code">
static void create(student.Database db) throws org.eyedb.Exception {
    Course perl = new Course(db);
    perl.setTitle("Perl");
    perl.setDescription("Perl Language");

    Course python = new Course(db);
    python.setTitle("Python");
    python.setDescription("Python Language");

    Course eyedb_ = new Course(db);
    eyedb_.setTitle("EyeDB");
    eyedb_.setDescription("EyeDB OODBMS");

    Student henri_muller = new Student(db);
    henri_muller.setFirstname("Henri");
    henri_muller.setLastname("Muller");
    henri_muller.setBeginYear((short)2003);

    Student jacques_martin = new Student(db);
    jacques_martin.setFirstname("Jacques");
    jacques_martin.setLastname("Martin");
    jacques_martin.setBeginYear((short)2003);

    Student mary_kiss = new Student(db);
    mary_kiss.setFirstname("Mary");
    mary_kiss.setLastname("Kiss");
    mary_kiss.setBeginYear((short)2003);

    Teacher max_first = new Teacher(db);
    max_first.setFirstname("Max");
    max_first.setLastname("First");

    Teacher georges_shorter = new Teacher(db);
    georges_shorter.setFirstname("Georges");
    georges_shorter.setLastname("Shorter");

    perl.setTeacher(max_first);
    python.setTeacher(max_first);
    eyedb_.setTeacher(georges_shorter);

    henri_muller.addToCoursesColl(perl);
    henri_muller.addToCoursesColl(eyedb_);

    jacques_martin.addToCoursesColl(python);
    jacques_martin.addToCoursesColl(perl);
    jacques_martin.addToCoursesColl(eyedb_);

    mary_kiss.addToCoursesColl(python);

    // storing objects to database
    henri_muller.store(org.eyedb.RecMode.FullRecurs);
    jacques_martin.store(org.eyedb.RecMode.FullRecurs);
    mary_kiss.store(org.eyedb.RecMode.FullRecurs);
}
</pre>
</td></tr></table>
<br>
<br>

<h2><a name="jq">Query data using the C++ binding and OQL</a></h2>
<table border=1 cellpadding=5 cellspacing=0>
<tr><td width=<?= $width ?> >
<pre class="code">
static void query_students(org.eyedb.Database db)
   throws org.eyedb.Exception {
    org.eyedb.OQL oql = new org.eyedb.OQL(db, "select Student");
    org.eyedb.ObjectArray obj_arr = new org.eyedb.ObjectArray();
    oql.execute(obj_arr);
    int count = obj_arr.getCount();
    for (int n = 0; n < count; n++) {
        Student s = (Student)obj_arr.getObject(n);
        if (s != null) {
            System.out.println(s.getFirstname() + " " + s.getLastname());
        }
    }
}

static void query_courses(org.eyedb.Database db,
                          String firstname, String lastname)
    throws org.eyedb.Exception
{
    org.eyedb.OQL oql = new org.eyedb.OQL
        (db, "select c from Course c where " +
         "c.teacher.lastname = \"" + lastname + "\" && " +
         "c.teacher.firstname = \"" + firstname + "\"");
    
    org.eyedb.ObjectArray obj_arr = new org.eyedb.ObjectArray();
    oql.execute(obj_arr);
    int count = obj_arr.getCount();
    for (int n = 0; n < count; n++) {
        Course c = (Course)obj_arr.getObject(n);
        if (c != null) {
            System.out.println(firstname + " " + lastname + ": " +
                               c.getTitle() + " " + c.getDescription());
        }
    }
}
</pre>
</td></tr></table>
<br>
<br>
View the whole <a href="quicktour/StudentTest.java" target="_blank">Java file</a>

</p>

<? include( 'footer.php'); ?>
