package org.eyedb.benchmark.quicktour.dummy;

import org.eyedb.benchmark.quicktour.Quicktour;

/**
 * @author Fran&ccedil;ois D&eacute;chelle (francois@dechelle.net)
 */

public class DummyQuicktour extends Quicktour {

    public String getImplementation()
    {
	return null;
    }

    public void prepare()
    {
    }

    public void finish()
    {
    }

    public void create(int nStudents, int nCourses, int nTeachers, int nObjectsPerTransaction)
    {
    }

    public void query( int nSelects)
    {
    }

    public void remove()
    {
    }
}

