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


#ifndef _COLLECTION_PEER_H
#define _COLLECTION_PEER_H

namespace eyedb {

  struct CollectionPeer {

    static int coherent, added, removed;
    static CollSet *collSet(const char *, const IndexImpl *);
    static CollSet *collSet(const char *, Class *, 
			    const Oid&, const Oid&,
			    int, int, int,
			    const IndexImpl *, Object *,
			    Bool, Data, Size);
    static CollBag *collBag(const char *, const IndexImpl *);
    static CollBag *collBag(const char *, Class *, 
			    const Oid&, const Oid&,
			    int, int, int,
			    const IndexImpl *, Object *,
			    Bool, Data, Size);
    static CollList *collList(const char *, const IndexImpl *);
    static CollList *collList(const char *, Class *, 
			      const Oid&, const Oid&,
			      int, int, int,
			      const IndexImpl *, Object *,
			      Bool, Data, Size);
    static CollArray *collArray(const char *, const IndexImpl *);
    static CollArray *collArray(const char *, Class *, 
				const Oid&, const Oid&,
				int, int, int,
				const IndexImpl *, Object *,
				Bool, Data, Size);
    static void setLock(Collection *, Bool);
    static void setInvOid(Collection *, const Oid&, int);
    static Bool isLocked(Collection *);
  };
}

#endif
