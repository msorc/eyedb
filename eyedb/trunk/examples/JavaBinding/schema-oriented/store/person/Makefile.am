JAVA_FILES=Address.java array_class_Person_ref.java Car.java CivilState.java Database.java Employee.java Person.java set_class_Car_ref.java 

JAVAROOT=$(top_builddir)/examples/JavaBinding/schema-oriented/store
CLASSPATH_ENV= CLASSPATH=$(JAVAROOT):$(top_builddir)/src/java/eyedb.jar

check-local: class.stamp
class.stamp: $(JAVA_FILES)
	$(CLASSPATH_ENV) $(JAVAC) -d $(JAVAROOT) $(AM_JAVACFLAGS) $(JAVACFLAGS) $?
	touch class.stamp

clean-local:
	-rm -f class.stamp *.class

#
# ODL to Java translation
#
EYEDBODL_exe=$(bindir)/eyedbodl
EYEDBODL = $(EYEDBODL_exe) --gencode=java
EYEDBODLFLAGS= --no-rootclass --package=person --output-dir=$(top_builddir)/examples/JavaBinding/schema-oriented/store/person

$(JAVA_FILES) : ../schema.odl
	if test -x $(EYEDBODL_exe) ; then \
		$(EYEDBODL) $(EYEDBODLFLAGS) $< ; \
	else \
		echo Warning: $(EYEDBODL) not found ; \
	fi

