<? $title = 'EyeDB quick tour: create data using the Java binding'; include( 'header.php'); ?>

<h1><?= $title ?></h1>
<p>
We show in this section how to create objects 
as defined in the <a href="quicktour_odl.php">ODL schema</a>
using the Java language binding.

<table border=1 cellpadding=5 cellspacing=0>
<tr><td class="Code">
<pre>
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
View the whole <a href="quicktour/StudentTest.java" target="_blank">Java file</a>
<br>
<br>

<table border=0>

<tr><td>Next</td>
<td><a href="quicktour_java_q.php">Query data using the Java binding and OQL</a></td></tr>

<tr><td>Previous&nbsp;</td>
<td><a href="quicktour_cplus_q.php">Query data using the C++ binding and OQL</a></td></tr>

<tr><td>Top</td>
<td><a href="quicktour.php">EyeDB Quick Tour</a></td>
</tr></table>

</p>

<br>
<br>
<? include( 'footer.php'); ?>
