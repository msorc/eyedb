/* 
   EyeDB Object Database Management System
   Copyright (C) 1994-2008 SYSRA
   
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


#ifndef _EYEDB_COLL_IMPL_H
#define _EYEDB_COLL_IMPL_H

#include "syscls.h"
#include "IndexImpl.h"

namespace eyedb {
  
  class CollImpl : public gbxObject {

  public:
    CollImpl() : impl_type(CollAttrImpl::Unknown), idximpl(0) { }

    CollImpl(CollAttrImpl::Type impl_type, const IndexImpl *idximpl = 0) :
      impl_type(impl_type), idximpl(idximpl) { }

    CollImpl(IndexImpl *_idximpl) : idximpl(_idximpl) {
      if (!idximpl) {
	impl_type = CollAttrImpl::NoIndex;
      }
      else if (idximpl->getType() == IndexImpl::Hash) {
	impl_type = CollAttrImpl::HashIndex;
      }
      else if (idximpl->getType() == IndexImpl::BTree) {
	impl_type = CollAttrImpl::BTreeIndex;
      }
      else {
	impl_type = CollAttrImpl::Unknown;
      }
    }

    CollAttrImpl::Type getType() const {return impl_type;}
    const IndexImpl *getIndexImpl() const {return idximpl;}
    
    void setType(CollAttrImpl::Type impl_type) {this->impl_type = impl_type;}
    void getIndexImpl(const IndexImpl *idximpl) {this->idximpl = idximpl;}
    
    bool compare(const CollImpl *collimpl) const {
      if (impl_type != collimpl->impl_type) {
	return false;
      }

      if ((idximpl == NULL && collimpl->idximpl != NULL) ||
	  (idximpl != NULL && collimpl->idximpl == NULL)) {
	return false;
      }

      if (idximpl != NULL && collimpl->idximpl != NULL) {
	return (bool)idximpl->compare(collimpl->idximpl);
      }

      return true;
    }

    CollImpl *clone() const;

  private:
    CollAttrImpl::Type impl_type;
    const IndexImpl *idximpl;
    // ready to add other information, such as hints for NoIndex
  };
}

#endif
