packagedir= $(pkglibdir)/java/classes/org/eyedb/benchmark/petshop/eyedb
dist_package_JAVA= $(JAVA_ODL_FILES) 
JAVAROOT= $(top_builddir)/src/java
EYEDB_JAR=@EYEDB_JAR@
CLASSPATH_ENV= CLASSPATH=$(JAVAROOT):$(top_srcdir)/src/java:$(EYEDB_JAR)

EYEDB_BINDIR=@EYEDB_BINDIR@
ODL_PACKAGE= org.eyedb.benchmark.petshop.eyedb
EYEDBODLJAVA= $(EYEDB_BINDIR)/eyedbodl
#CPPFLAGS=--cpp-flags=-DUSE_INVERSE
EYEDBODLJAVAFLAGS= $(CPPFLAGS) --gencode=java --no-rootclass --package=$(ODL_PACKAGE) --output-dir=. 
JAVA_ODL_FILES= \
Account.java \
BannerData.java \
Category.java \
Database.java \
Item.java \
LineItem.java \
Order.java \
PersonInfo.java \
Product.java \
Sequence.java \
set_class_Account_ref.java \
set_class_Item_ref.java \
set_class_LineItem_ref.java \
set_class_Order_ref.java \
set_class_Product_ref.java \
Supplier.java 

ODL_FILE=$(top_srcdir)/src/java/org/eyedb/benchmark/petshop/eyedb/epetstore.odl

BUILT_SOURCES= $(JAVA_ODL_FILES)

$(JAVA_ODL_FILES): $(ODL_FILE)
	$(EYEDBODLJAVA) $(EYEDBODLJAVAFLAGS) $<

CLEANFILES= $(JAVA_ODL_FILES)

