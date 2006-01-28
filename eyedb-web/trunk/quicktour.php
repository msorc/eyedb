<? include( 'header.php'); ?>
<? include( 'nav.php'); ?>

<div id="CenterBlock">
<h1 id="PageTitle">EyeDB Quick Tour</h1>
<p>

<h2>A short ODL schema</h2>
<pre class="code">
class Person {
  attribute string firstname;
  attribute string lastname;
  attribute date birthdate;
};

class Student extends Person {
  attribute short begin_year;
  relationship set<Course *> courses inverse students;
};

enum CourseStatus {
  COMPLEMENTARY,
  REQUIRED
};

class Course {
  attribute CourseStatus status;
  attribute string title;
  attribute string description;
  relationship set<Student *> students inverse courses;
  relationship Teacher *teacher inverse courses;
};

class Teacher extends Person {
  relationship set<Course *> courses inverse teacher;
};
</pre>
Get the <a href="quicktour/student.odl" target="_blank">ODL file</a>
<h2>OQL creation</h2>
<pre class="code">
// Creating a few courses

oodbms := new Course(status: REQUIRED,
		     title : "OODBMS",
		     description : "Object database management systems");


rdbms := new Course(status: REQUIRED,
		    title : "RDBMS",
		    description : "Relational database management systems");

uml := new Course(status: REQUIRED,
		  title : "UML",
		  description : "Unified Modeling Language");

cplus := new Course(status: REQUIRED,
		    title : "C++",
		    description : "C++ Language");

java := new Course(status: REQUIRED,
		   title : "Java",
		   description : "Java Language");

php := new Course(status: COMPLEMENTARY,
		  title : "PHP",
		  description : "PHP Language");


// Creating a few students

john_harris := new Student(firstname : "John",
			   lastname : "Harris",
			   birthdate : date::date("1994-03-02"),
			   begin_year : 2002);

suzan_mulder := new Student(firstname : "Suzan",
			    lastname : "Mulder",
			    birthdate : date::date("1996-04-12"),
			    begin_year : 2002);

francois_martin := new Student(firstname : "Francois",
			       lastname : "Martin",
			       birthdate : date::date("1983-08-04"),
			       begin_year : 2001);

// Creating a few teachers

eric_viara := new Teacher(firstname : "Eric",
			  lastname : "Viara",
			  birthdate : date::date("1958-12-15"));

francois_dechelle := new Teacher(firstname : "Francois",
				 lastname : "Dechelle",
				 birthdate : date::date("1960-06-23"));

// Assign courses to teachers

oodbms.teacher := eric_viara;
rdbms.teacher := eric_viara;
uml.teacher := francois_dechelle;
cplus.teacher := eric_viara;
java.teacher := francois_dechelle;
php.teacher := francois_dechelle;

// Assign courses to student
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

<h2>OQL queries</h2>

<pre class="code">
// looking for Persons
select Student;
select Teacher;
select Person;

select Student.firstname = "Francois";
select firstname + " " + lastname from Student where firstname = "Francois";

select Teacher.firstname = "Francois";
select Person.firstname = "Francois";

// looking for Courses
select description from Course where title = "OODBMS";
select * from Course where title = "OODBMS";

select title from Course where status = REQUIRED;

// looking for Teacher teaching a given course
select x.teacher.firstname + " " + x.teacher.lastname from Course x
       where x.title = "OODBMS";

select (Course.title = "OODBMS").teacher.lastname;

// looking for courses teached by a given teacher
// from Course class:
select title from Course where teacher.lastname = "Dechelle";
// from Teacher class:
select x.courses[?].title from Teacher x where x.lastname = "Dechelle";

// looking for courses learnt by a given student
// from Student
select s.courses[?].title from Student s where s.lastname = "Mulder";

select s.courses[?].title from Student s where s.lastname = "Mulder" 
                                               and s.firstname = "Suzan";

// from Course
select c.title from Course c where c.students[?].lastname = "Mulder";

select c.title from Course c where c.students[?] =
   (select one s from Student s where s.lastname = "Mulder" and
                                      s.firstname = "Suzan");
</pre>
Get the <a href="quicktour/student.oql" target="_blank">OQL file</a>
<h2>C++ creation</h2>
<pre class="code">
static void create(eyedb::Database *db)
{
  Course *perl = new Course(db);
  perl->setStatus(REQUIRED);
  perl->setTitle("Perl");
  perl->setDescription("Perl Language");

  Course *python = new Course(db);
  perl->setStatus(COMPLEMENTARY);
  python->setTitle("Python");
  python->setDescription("Python Language");

  Course *eyedb_ = new Course(db);
  perl->setStatus(REQUIRED);
  eyedb_->setTitle("EyeDB");
  eyedb_->setDescription("EyeDB OODBMS");
  eyedb::Date *birthdate;

  Student *henri_muller = new Student(db);
  henri_muller->setFirstname("Henri");
  henri_muller->setLastname("Muller");
  birthdate = eyedb::Date::date(0, 1986, eyedb::Month::September, 20);
  henri_muller->setBirthdate(birthdate);
  birthdate->release();
  henri_muller->setBeginYear(2003);

  Student *jacques_martin = new Student(db);
  jacques_martin->setFirstname("Jacques");
  jacques_martin->setLastname("Martin");
  birthdate = eyedb::Date::date(0, 1988, eyedb::Month::October, 2);
  jacques_martin->setBirthdate(birthdate);
  birthdate->release();
  jacques_martin->setBeginYear(2003);
  
  Student *mary_kiss = new Student(db);
  mary_kiss->setFirstname("Mary");
  mary_kiss->setLastname("Kiss");
  birthdate = eyedb::Date::date(0, 1989, eyedb::Month::January, 10);
  mary_kiss->setBirthdate(birthdate);
  birthdate->release();
  mary_kiss->setBeginYear(2003);

  Teacher *max_first = new Teacher(db);
  max_first->setFirstname("Max");
  max_first->setLastname("First");
  birthdate = eyedb::Date::date(0, 1969, eyedb::Month::March, 12);
  max_first->setBirthdate(birthdate);
  birthdate->release();

  Teacher *georges_shorter = new Teacher(db);
  georges_shorter->setFirstname("Georges");
  georges_shorter->setLastname("Shorter");
  birthdate = eyedb::Date::date(0, 1954, eyedb::Month::July, 30);
  georges_shorter->setBirthdate(birthdate);
  birthdate->release();

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
</pre>
<h2>C++ queries</h2>
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
Get the <a href="quicktour/student_test.cc" target="_blank">C++ file</a>
</div>

<? include( 'footer.php'); ?>
