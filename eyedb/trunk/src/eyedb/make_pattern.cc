/* 
   EyeDB Object Database Management System
   Copyright (C) 1994-1999,2004-2006 SYSRA
   
   EyeDB is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
   
   EyeDB is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA 
*/

/*
   Author: Eric Viara <viara@sysra.com>
*/


static const char old_make_pattern[] =
"#\n"
"# Makefile.%s\n"
"# \n"
"# %s package\n"
"#\n"
"# %s %s"
"#\n"
"\n"
"include $(EYEDBROOT)/share/eyedb/Makefile.eyedb\n"
"\n"
"all: %smthfe$(SO) %smthbe$(SO)\n"
"\n"
"%smthfe$(SO): %smthfe.o $(STUBSDIR)%sstubsfe.o $(STUBSDIR)%s.o $(EXTRALIBS)\n"
"\t$(CXX) $(SO_FLAGS) -o $@ $+"
"\n"
"\n"
"%smthbe$(SO): %smthbe.o $(STUBSDIR)%sstubsbe.o $(STUBSDIR)%s.o $(EXTRALIBS)\n"
"\t$(CXX) $(SO_FLAGS) -o $@ $+"
"\n"
"\n"
"install: all\n"
"\tcp -f %smthfe$(SO) %smthbe$(SO) %s"
"\n";

static const char make_pattern[] =
"#\n"
"# Makefile.%s\n"
"# \n"
"# %s package\n"
"#\n"
"# Example of template Makefile that can help you to compile\n"
"# the generated C++ file and the template program\n"
"# %s %s"
"#\n"
"\n"
"include %s/eyedb/Makefile.eyedb\n"
"\n"
"CXXFLAGS += $(EYEDB_CXXFLAGS) $(EYEDB_CPPFLAGS)\n"
"LDFLAGS  += ${EYEDB_LDFLAGS}\n"
"LDLIBS   += ${EYEDB_LDLIBS}\n"
"\n"
"# if you use gcc\n"
"GCC_FLAGS = -Wl,-R$(EYEDB_LIBDIR)\n"
"\n"
"# example for compiling the template program:\n"
"template_%s: %s.o template_%s.o\n"
"	$(CXX) $(LDFLAGS) $(GCC_FLAGS) -o $@ $^ $(LDLIBS)\n"
"\n";

static const char template_pattern[] =
"\n"
"/*\n"
" * Template program for %s package to be compiled with Makefile.%s\n"
" *\n"
" * %s %s"
" */\n"
"\n"
"#include <iostream>\n"
"#include \"%s.h\"\n"
"\n"
"int\n"
"main(int argc, char *argv[])\n"
"{\n"
"  eyedb::init(argc, argv);\n"
"  %s::init();\n"
"\n"
"  if (argc != 2) {\n"
"    std::cerr << \"usage: \" << argv[0] << \" <dbname>\" << std::endl;\n"
"    return 1;\n"
"  }\n"
"\n"
"  eyedb::Exception::setMode(eyedb::Exception::ExceptionMode);\n"
"\n"
"  try {\n"
"    eyedb::Connection conn;\n"
"\n"
"    conn.open();\n"
"\n"
"    %sDatabase db(argv[1]);\n"
"    db.open(&conn, eyedb::Database::DBRW);\n"
"\n"
"    db.transactionBegin();\n"
"\n"
"    // ----------------------------\n"
"    // perform your operations here\n"
"    // ----------------------------\n"
"\n"
"    db.transactionCommit();\n"
"\n"
"    db.close();\n"
"  }\n"
"  catch(eyedb::Exception &e) {\n"
"    e.print();\n"
"  }\n"
"\n"
"  %s::release();\n"
"  eyedb::release();\n"
"\n"
"  return 0;\n"
"}\n";
