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


#include "eyedb/eyedb.h"

namespace eyedb {

static Architecture *arch_;

void Architecture::init()
{
#if defined(SOLARIS_SPARCV7_CC)
  arch_ = new Architecture("solaris-sparcv7-cc",
			     "sparcv7", "Solaris", "CC", True);

#elif defined(SOLARIS_SPARCV7_GCC)
  arch_ = new Architecture("solaris-sparcv7-gcc",
			     "sparcv7", "Solaris", "gcc", True);

#elif defined(SOLARIS_SPARCV9_CC)
  arch_ = new Architecture("solaris-sparcv9-cc",
			     "sparcv9", "Solaris", "CC", True);

#elif defined(SOLARIS_SPARCV9_GCC)
  arch_ = new Architecture("solaris-sparcv9-gcc",
			     "sparcv9", "Solaris", "gcc", True);

#elif defined(DEBUG_SOLARIS_SPARCV7_CC)
  arch_ = new Architecture("debug-solaris-sparcv7-cc",
			     "sparcv7", "Solaris", "CC", True);

#elif defined(DEBUG_SOLARIS_SPARCV7_GCC)
  arch_ = new Architecture("debug-solaris-sparcv7-gcc",
			     "sparcv7", "Solaris", "gcc", True);

#elif defined(DEBUG_SOLARIS_SPARCV9_CC)
  arch_ = new Architecture("debug-solaris-sparcv9-cc",
			     "sparcv9", "Solaris", "CC", True);

#elif defined(DEBUG_SOLARIS_SPARCV9_GCC)
  arch_ = new Architecture("debug-solaris-sparcv9-gcc",
			     "sparcv9", "Solaris", "gcc", True);

#elif defined(LINUX_X86)
  arch_ = new Architecture("linux-x86",
			     "x86", "Linux", "gcc", False);

#elif defined(DEBUG_LINUX_X86)
  arch_ = new Architecture("debug-linux-x86",
			     "x86", "Linux", "gcc", False);

#elif defined(CYGWIN_X86)
  arch_ = new Architecture("cygwin-x86",
			     "x86", "Cygwin", "gcc", False);

#elif defined(DEBUG_CYGWIN_X86)
  arch_ = new Architecture("debug-cygwin-x86",
			     "x86", "Cygwin", "gcc", False);

#elif defined(LINUX_X86_64)
  arch_ = new Architecture("linux-x86-64",
			     "x86_64", "Linux", "gcc", False);

#elif defined(DEBUG_LINUX_X86_64)
  arch_ = new Architecture("debug-linux-x86-64",
			     "x86_64", "Linux", "gcc", False);

#else
#error "UNKNOWN ARCHITECTURE"
#endif
}

void Architecture::_release()
{
  delete arch_;
}

Architecture *Architecture::getArchitecture()
{
  return arch_;
}
}
/*

eyedb::Architecture eyedb::getArchitecture()
{
  return eyedb::Architecture::getArchitecture();
}
*/
