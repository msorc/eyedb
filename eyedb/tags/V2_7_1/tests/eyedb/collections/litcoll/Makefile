#
# Collection Tests Makefile
#
# Eric Viara copyright(c) SYSRA
#

include $(EYEDBROOT)/share/eyedb/Makefile.eyedb

all: colltest colltestlit

EXTRA_CXXFLAGS = -g

colltest: schema.o colltest.o
	$(PURIFY) $(LDXX) -o $@ $+ $(LDXXFLAGS) $(LDXXLIBS)

colltest.o schema.o: schema.h

colltestlit: schema.o colltestlit.o
	$(PURIFY) $(LDXX) -o $@ $+ $(LDXXFLAGS) $(LDXXLIBS)

colltestlit.o schema.o: schema.h

schema.cc: schema.odl
	eyedbodl --gencode=C++ --package=schema schema.odl

DB = litcoll

db:
	-eyedbdbdelete $(DB)
	eyedbdbcreate $(DB)
	eyedbodl -u -d $(DB) --package=schema schema.odl

clean:
	rm -f *.o colltest colltestlit schema*.cc schema*.h Makefile.schema
