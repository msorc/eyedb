//print is a python keyword
%rename(outp) print;
//next is a ruby keyword
%rename(nextitem) next;
%rename(_NULL) NULLVAL;

%ignore class_info;
%ignore eyedb::InstanceInfo::name;

%ignore Database::queryOid;
%ignore Database::queryOids;
%ignore Database::queryObject;
%ignore Database::queryObjects;
%ignore Database::getProperty;
%ignore Database::getProperties;


%ignore Database::queryValue;
%ignore Database::queryValues;
%ignore Database::execOQL;
%ignore Database::updateSchema;
%ignore Agregat::getItemValue;
%ignore Agregat::setItemValue;

//swig 1.3.24 specific
%ignore eyedb::Database::remove;
//reflection specific
%ignore idb_agregatClassLoad;



%include <eyedblib/machtypes.h>
%ignore DbCreateDescription;
%ignore DbInfoDescription;
%ignore OpenHints;
%ignore TransactionParams;
%ignore NullString;
%ignore NilString;
#undef _eyedb_idbbase_

%include <eyedb/base.h>

%ignore idb__status__;
#ifdef SWIGJAVA
%ignore Exception::Exception;
%include <eyedb/status.h>
%rename(idbClone) clone;
%rename(getIdbClass) getClass;
%ignore Object::toString;
%ignore Value::getType;
//%ignore idbException::operator=;

#endif




// This tells SWIG to treat char ** as a special case

%typemap(php4, in) char ** {
	//zval *actarg=*$input;
	zval *tempav;
	int len=0;
	int i=0;
	zval  **tmp;
	HashPosition pos;
	char *c;
	if (Z_TYPE_PP($input)!=IS_ARRAY){
	   zend_error(E_WARNING,"Argument $argnum is not a reference.");
	   RETURN_NULL();
	}
	zend_hash_internal_pointer_reset_ex(Z_ARRVAL_PP($input),&pos);
	while(zend_hash_get_current_data_ex(Z_ARRVAL_PP($input),(void**)&tmp,&pos)==SUCCESS){
	  len++;
	  zend_hash_move_forward_ex(Z_ARRVAL_PP($input),&pos);
	}
	$1 = (char **) malloc((len+2)*sizeof(char *));
	zend_hash_internal_pointer_reset_ex(Z_ARRVAL_PP($input),&pos);
	while(zend_hash_get_current_data_ex(Z_ARRVAL_PP($input),(void**)&tmp,&pos)==SUCCESS){
	  convert_to_string_ex(tmp);
	  $1[i++] = estrndup(Z_STRVAL_PP(tmp),Z_STRLEN_PP(tmp));
	  zend_hash_move_forward_ex(Z_ARRVAL_PP($input),&pos);
        }
	$1[i] = NULL;
};

// This tells SWIG to treat char ** as a special case
%typemap(perl5, in) char ** {
	AV *tempav;
	I32 len;
	int i;
	SV  **tv;
	if (!SvROK($input))
	    croak("Argument $argnum is not a reference.");
        if (SvTYPE(SvRV($input)) != SVt_PVAV)
	    croak("Argument $argnum is not an array.");
        tempav = (AV*)SvRV($input);
	len = av_len(tempav);
	$1 = (char **) malloc((len+2)*sizeof(char *));
	for (i = 0; i <= len; i++) {
	    tv = av_fetch(tempav, i, 0);	
	    $1[i] = (char *) SvPV(*tv,PL_na);
        }
	$1[i] = NULL;
};


// This tells SWIG to treat char ** as a special case
%typemap(python, in) char ** {
  /* Check if is a list */
  if (PyList_Check($input)) {
    int size = PyList_Size($input);
    int i = 0;
    $1 = (char **) malloc((size+1)*sizeof(char *));
    for (i = 0; i < size; i++) {
      PyObject *o = PyList_GetItem($input,i);
      if (PyString_Check(o))
	$1[i] = PyString_AsString(PyList_GetItem($input,i));
      else {
	PyErr_SetString(PyExc_TypeError,"list must contain strings");
	free($1);
	return NULL;
      }
    }
    $1[i] = 0;
  } else {
    PyErr_SetString(PyExc_TypeError,"not a list");
    return NULL;
  }
}

// This tells SWIG to treat char ** as a special case
%typemap(ruby,in) char ** {
  /* Get the length of the array */
  int size = RARRAY($input)->len;     
  int i;
  $1 = (char **) malloc((size+1)*sizeof(char *));
  /* Get the first element in memory */
  VALUE *ptr = RARRAY($input)->ptr;   
  for (i=0; i < size; i++, ptr++)
    /* Convert Ruby Object String to char* */
    $1[i]= STR2CSTR(*ptr); 
  $1[i]=NULL;  /* End of list */
}

%typemap(tcl8,in) char ** {
     Tcl_Obj **listobjv;
     int       nitems;
     int       i;
     if (Tcl_ListObjGetElements(interp, $input, &nitems, &listobjv) == TCL_ERROR) {
        return TCL_ERROR;
     }
     $1 = (char **) malloc((nitems+1)*sizeof(char *));
     for (i = 0; i < nitems; i++) {
        $1[i] = Tcl_GetStringFromObj(listobjv[i],0);
     }
     $1[i] = 0;
}


// This cleans up the char ** array after the function call
%typemap(freearg) char ** {
	free($1);
}

#ifdef SWIGJAVA
%include <various.i>
#endif





// idbBool
/*
%typemap(php4, in) Bool {
	//long temp = (long)$input->value.lval; //Z_LVAL($input);
	long temp = (long)Z_LVAL_PP($input);
	$1 = temp?True:False;
}


%typemap(perl5, in) Bool {
  SV *tempsv = $input ;
  if ( (!SvIOK(tempsv)) && (!SvPOK(tempsv))) {
	printf("Received %d\n", SvTYPE(tempsv));
	SWIG_croak("Expected a boolean.");
  }
  $1 = SvIV(tempsv)?True:False;
}

%typemap(python, in) Bool {
    bool b ;
    PyObject * obj0 = 0 ;
    b = (bool)SWIG_As_bool($input);
    if (PyErr_Occurred()) SWIG_fail;
    $1 = b?True:False;
}




%typemap(php4, out) Bool {
	ZVAL_LONG($result,$1);
}

%typemap(perl5, out) Bool {
	$result = sv_newmortal();
  	sv_setiv($result,(IV) $1==True?1:0);
  	argvi++;
}

%typemap(python, out) Bool {
	$result = SWIG_From_bool($1==True?true:false);
}


*/


%typemap(php4, in) Bool* {
    	convert_to_long_ex($input);
        $1 = (Bool*) (*$input)->value.lval;

/*
                arg2 = &intr2;
	//long temp = (long)$input->value.lval; //Z_LVAL($input);
	long temp = (long)Z_LVAL_PP($input);
	$1 = temp?True:False;
*/
}

%typemap(python,typecheck,precedence=SWIG_TYPECHECK_BOOL) Bool {
    bool b = (bool)SWIG_As_bool($input);
    $1 = (!PyErr_Occurred() || PyInt_Check($input))?1:0;
}

%typemap(python, in) Bool {
    bool b ;
    PyObject * obj0 = 0 ;
    b = (bool)SWIG_As_bool($input);
    if (PyErr_Occurred()) SWIG_fail;
    $1 = b?True:False;
}


%typemap(ruby,typecheck,precedence=SWIG_TYPECHECK_BOOL) Bool {
    //$1 = 1;
    $1 = ((TYPE($input) == T_FIXNUM) || (TYPE($input) == T_BIGNUM) || $input == Qtrue || $input == Qfalse) ? 1 : 0;	
}
%typemap(ruby, in) Bool {
    bool b ;
    b = RTEST($input);
    $1 = b?True:False;
}

%typemap(ruby, out) Bool {
	$result = $1==True?Qtrue:Qfalse;
}

%typemap(tcl8,typecheck,precedence=SWIG_TYPECHECK_BOOL) Bool {
    char *c = Tcl_GetStringFromObj($input, NULL);
    $1 = c ? 1 : 0;
}
%typemap(tcl8, in) Bool {
    bool b=false;
    char *c = Tcl_GetStringFromObj($input, NULL);
    if(!c) {
	   SWIG_fail;
    } else if(!strcmp("yes",c) || !strcmp("true",c)){
      $1 = True;
    } else  if(!strcmp("no",c) || !strcmp("false",c)){
      $1 = False;
    } else {
      char *ep=0;
      long res= strtol (c, &ep,0);
      if(ep && *ep) {
	     SWIG_fail;
      }
      $1=res?True:False;
    }

}




// Test functions

%inline %{
using namespace std;
/*
int print_args(char **argv) {
    int i = 0;
    while (argv[i]) {
         printf("argv[%d] = %s\n", i,argv[i]);
         i++;
    }
    return i;
}
*/
// Returns a char ** list 
char **get_args() {
    static char *values[] = { (char*)"Dave", (char*)"Mike", (char*)"Susan", (char*)"John", (char*)"Michelle", 0};
    return &values[0];
}

//bool retTrue() {return true;}
%}



%{
#include <eyedb/eyedb.h>
int force2=0,force3=0,force4=0,force5=0,force6=0,force7=0,force8=0,force9=0;
int force10=0,force11=0,force12=0,force13=0,force14=0,force15=0,force16=0,force17=0,force18=0,force19=0;
int intr2=0,intr3=0,intr4=0,intr5=0,intr6=0,intr7=0,intr8=0,intr9=0;
int intr10=0,intr11=0,intr12=0,intr13=0,intr14=0,intr15=0,intr16=0,intr17=0,intr18=0,intr19=0;
%}
#ifdef SWIGJAVA
%apply char **STRING_ARRAY { char **argv };
#endif
%inline %{

using namespace eyedb;

void dbInit(char **argv){
  int i = 0;
  const char *def[]={"pers-db",0};
  char **p_argv=(argv && *argv)?argv:(char**)def;
  while (p_argv[i])i++;
  ::init(i, p_argv);
  Exception::setMode( Exception::ExceptionMode);
}
bool is_void_obj(void *p){
  return p ? false : true;
}

void test_bool(bool b){
     bool b2=b;
}
%}

// Replace delete with release for eyedb objects

%inline %{
template <class T> void delete_or_release(T *o){o->release();}
template <> void delete_or_release(Connection *o){delete o;}
template <> void delete_or_release(ObjectArray *o){delete o;}
template <> void delete_or_release(ObjectList *o){delete o;}
template <> void delete_or_release(ObjectListCursor *o){delete o;}
template <> void delete_or_release(ObjectReleaser *o){delete o;}
template <> void delete_or_release(ServerMessageDisplayer *o){delete o;}
template <> void delete_or_release(StdServerMessageDisplayer *o){delete o;}
template <> void delete_or_release(TypeModifier *o){delete o;}
template <> void delete_or_release(Attribute *o){delete o;}
template <> void delete_or_release(Exception *o){delete o;}
template <> void delete_or_release(ClassIterator *o){delete o;}
template <> void delete_or_release(InstanceInfo *o){delete o;}
template <> void delete_or_release(gbxDeleter *o){delete o;}
template <> void delete_or_release(gbxAutoGarb *o){delete o;}
template <> void delete_or_release(gbxAutoGarbSuspender *o){delete o;}
template <> void delete_or_release(gbxTag *o){delete o;}
template <> void delete_or_release(ObjectListReleaser *o){delete o;}
template <> void delete_or_release(OQL *o){delete o;}
template <> void delete_or_release(OQLIterator *o){delete o;}
template <> void delete_or_release(Value *o){delete o;}
template <> void delete_or_release(ValueArray *o){delete o;}
template <> void delete_or_release(ValueList *o){delete o;}
template <> void delete_or_release(ValueListCursor *o){delete o;}
template <> void delete_or_release(CollectionIterator *o){delete o;}

%}

%include <eyedb/Object.h>
%include <eyedb/Instance.h>

%include <eyedb/Agregat.h>
%include <eyedb/Struct.h>
%ignore CollList::insertFirst;
%ignore CollList::insertAfter;
%ignore CollList::insertBefore;
%ignore Collection::move;
%ignore Collection::getLocations;

%ignore CollList;

%include <eyedb/Collection.h>
//%ignore Database::open;
%include <eyedb/Database.h>

%extend eyedb::Database{
	Status openDB(Connection *conn,
			 Database::OpenFlag fl=Database::DBRead,
			 const char *user=0,
			 const char *passwd=0){
		return self->open(conn,fl,user,passwd);
	}

}

%include <eyedb/Connection.h>
%include <eyedb/OQL.h>
%include <eyedb/Value.h>
%extend eyedb::Object{
	void storeFullRecurs(){
	self->store(FullRecurs);
	}
}

%extend eyedb::OQL {
  Status executeObjectArray(ObjectArray &oa){
	return self->execute(oa);
  }

  Status executeValue(Value &v){
	return self->execute(v);
  } 
  // get flattened results
  Status executeOidArray(OidArray &oa){
	return self->execute(oa);
  }
  Status executeValueArray(ValueArray &va){
	return self->execute(va);
  }
  Status getResultValue(Value &v){
	return self->getResult(v);
  }
  // get flattened results
  Status getResultObjectArray(ObjectArray &oa){
	return self->getResult(oa);
  }
  Status getResultOidArray(OidArray &oa){
	return self->getResult(oa);
  }
  Status getResultValueArray(ValueArray &va){
	return self->getResult(va);
  }

}
%extend eyedb::ObjectArray {
	  Object * getAt(int i){
		return (*self)[i];
	}
}
%extend eyedb::ValueArray {
	  Value * getAt(int i){
		return &((*self)[i]);
	}
}

%extend eyedb::Value {
  // flatten methods
  Status toObjectArray(Database *db, ObjectArray &oa){
	self->toArray(db,oa);
  }
  Status toValueArray(ValueArray &va){
	self->toArray(va);
  }
  Status toOidArray(OidArray &oa){
	self->toArray(oa);
  }

}

%define eyedb_java_exts(N)
%extend N##Database {
	static 
	N##Database *create(char *name){
	       return new  N##Database(name,0);
	}
	Status openDB(Connection *conn,
			 Database::OpenFlag fl){
		return self->open(conn,fl,(const char*)0,(const char*)0);
	}

}
%enddef

