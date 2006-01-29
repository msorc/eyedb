<? $title = 'EyeDB quick tour: query data using the Java binding and OQL'; include( 'header.php'); ?>

<h1><?= $title ?></h1>
<p>
We show in this section how to query objects 
as defined in the <a href="quicktour_odl.php">ODL schema</a>
using the Java language binding and OQL.

<table border=1 cellpadding=5 cellspacing=0>
<tr><td class="Code">
<pre>
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

<br>
<br>
<table border=0>

<tr><td>Previous&nbsp;</td>
<td><a href="quicktour_java_c.php">Create data using the Java binding</a></td></tr>
<tr><td>Top</td>
<td><a href="quicktour.php">EyeDB Quick Tour</a></td>
</tr></table>
<br>
<br>
<? include( 'footer.php'); ?>
