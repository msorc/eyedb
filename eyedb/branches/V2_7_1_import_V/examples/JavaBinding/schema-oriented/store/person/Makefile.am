packagedir= $(pkglibdir)/java/classes/org/eyedb/examples/schema-oriented/store/person
package_JAVA=Address.java array_class_Person_ref.java Car.java CivilState.java Database.java Employee.java Person.java set_class_Car_ref.java 

JAVAROOT=$(top_builddir)/examples/JavaBinding/schema-oriented/store
CLASSPATH_ENV= CLASSPATH=$(JAVAROOT):$(top_builddir)/src/java/eyedb.jar

EYEDBODL_exe=$(bindir)/eyedbodl
EYEDBODL = $(EYEDBODL_exe) --gencode=java
#EYEDBODLFLAGS= --no-rootclass --package=person
EYEDBODLFLAGS= --no-rootclass --package=person --output-dir=$(top_builddir)/examples/JavaBinding/schema-oriented/store/person

$(package_JAVA) : ../schema.odl
	if test -x $(EYEDBODL_exe) ; then \
		$(EYEDBODL) $(EYEDBODLFLAGS) $< ; \
	else \
		echo Warning: $(EYEDBODL) not found ; \
	fi

BUILT_SOURCES = $(package_JAVA)
