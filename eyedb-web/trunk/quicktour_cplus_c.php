<? $title = 'EyeDB quick tour: Create data using the C++ binding'; include( 'header.php'); ?>

<h1><?= $title ?></h1>
<p>
We show in this section how to create objects 
as defined in the <a href="quicktour_odl.php">ODL schema</a>
using the C++ language binding.

<table border=1 cellpadding=5 cellspacing=0>
<tr><td class="Code">
<pre>
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
View the whole <a href="quicktour/student_test.cc" target="_blank">C++ file</a>

<br>
<br>
<? quicktour_nav("quicktour_oql_q.php", "quicktour_cplus_q.php" ); ?>
</p>
<br>
<br>

<? include( 'footer.php'); ?>
