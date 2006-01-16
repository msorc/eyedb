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

#include <lib/compile_builtin.h>

namespace eyedblib {
  const char* CompileBuiltin::getArch()
  {
    return ARCH;
  }

  const char* CompileBuiltin::getBindir()
  {
    return BINDIR;
  }

  const char* CompileBuiltin::getDatadir()
  {
    return DATADIR;
  }

  const char* CompileBuiltin::getLibdir()
  {
    return LIBDIR;
  }

  const char* CompileBuiltin::getLocalstatedir()
  {
    return LOCALSTATEDIR;
  }

  const char* CompileBuiltin::getSysconfdir()
  {
    return SYSCONFDIR;
  }

}
