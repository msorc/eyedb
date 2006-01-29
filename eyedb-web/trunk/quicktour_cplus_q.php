<? $title = 'EyeDB quick tour: query data using the C++ binding and OQL'; include( 'header.php'); ?>

<h1><?= $title ?></h1>
<p>
We show in this section how to query objects 
as defined in the <a href="quicktour_odl.php">ODL schema</a>
using the C++ language binding and OQL.

<table border=1 cellpadding=5 cellspacing=0>
<tr><td class="Code">
<pre>
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

<br>
<br>
<? quicktour_nav("quicktour_cplus_c.php", "quicktour_java_c.php" ); ?>

</p>
<br>
<br>

<? include( 'footer.php'); ?>
