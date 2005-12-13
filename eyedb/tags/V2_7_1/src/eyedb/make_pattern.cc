/* 
   EyeDB Object Database Management System
   Copyright (C) 1994-1999,2004,2005 SYSRA
   
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


static const char mkpattern[] =
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
