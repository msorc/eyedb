%inline %{
using namespace eyedb;
%}

%module eyedbreflect
%ignore idb_schemaClassLoad;
%ignore Schema::getClass;
//4java
%ignore Schema::generateCode;
%ignore Schema::displaySchemaDiff;
%ignore Schema::genIDL;
%ignore Schema::genORB;
%ignore Schema::genC_API;
%ignore Schema::genJava_API;
%ignore Schema::genODL;
%ignore Schema::checkODL;
%include <eyedbschema.i>

//Additional includes
%include <eyedb/Schema.h>
%include <eyedb/Class.h>
%ignore sizesCompute;
%ignore template_name;
%ignore log_item_entry_fmt;
%ignore log_comp_entry_fmt;
%ignore idb_agregatClassLoad;
%ignore idb_agregatLoad;
%ignore decodeIDR;


%include <eyedb/Attribute.h>


//Specific include
%{
#include <eyedbreflect_adapter.h>
template <> void delete_or_release(ClassList *o){delete o;}
template <> void delete_or_release(ClassListCursor *o){delete o;}
template <> void delete_or_release(AttributeList *o){delete o;}
template <> void delete_or_release(AttributeListCursor *o){delete o;}
template <> void delete_or_release(std::vector<eyedb::Attribute*, std::allocator<eyedb::Attribute*> > *o){delete o;}
template <> void delete_or_release(std::vector<eyedb::Class*, std::allocator<eyedb::Class*> > *o){delete o;}

/*
template <> void delete_or_release(std::list<Class*,std::allocator<Class*> > *o){delete o;}
template <> void delete_or_release(list_iterator<Class*> *o){delete o;}
template <> void delete_or_release(list_reverse_iterator<Class*> *o){delete o;}
*/
%}

%runtime %{

namespace eyedb {
class Attribute;
class Class;
}

#include <eyedbreflect_adapter.h>

%}


/* collections with templates : it works for ruby
%include <std_vector.i>
namespace std {
%template(ClassVector) vector<Class*>;
%template(AttributeVector) vector<Attribute*>;
}
*/


//collections without templates

%include <eyedb_collections.i>
wrap_std_vector_p(Attribute*,AttributeVector);
wrap_std_vector_p(Class*,ClassVector);



%{
template <> void delete_or_release(ClassVector *o){delete o;}
template <> void delete_or_release(AttributeVector *o){delete o;}

%}
//end collections

%include <eyedbreflect_adapter.h>
namespace eyedb {
%extend Class {
	std::vector<Attribute*> * getAttributeVector(){
	   return getAttributeVector(self);
	}
}


%extend Schema {
	std::vector<Class*> * getClassVector(){
	   return getClassVector(self);
	}

}
%extend Object {
	const char *getOidString(){
	return self->getOid().toString();
	}
	void setUserFlag(long fl){
	   self->setUserData((void *)fl);
	}
	long getUserFlag(){
	  return (long)self->getUserData();
        }
}
#ifdef SWIGJAVA
%extend Database {
	static 
	Database *create(char *name){
	       return new  Database(name,0);
	}
	Status openDB(Connection *conn,
			 Database::OpenFlag fl){
		return self->open(conn,fl,(const char*)0,(const char*)0);
	}
}
#endif
};
