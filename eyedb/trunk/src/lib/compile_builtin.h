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

namespace eyedblib {
  class CompileBuiltin {
  public:
    static const char* getArch();
    static const char* getCpu();
    static const char* getOs();
    static const char* getCompiler();
    static const char* getBindir();
    static const char* getSbindir();
    static const char* getDatadir();
    static const char* getLibdir();
    static const char* getDatabasedir();
    static const char* getPipedir();
    static const char* getTmpdir();
    static const char* getSysconfdir();
  };
}
