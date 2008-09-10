%{
#include <stdexcept>
%}

%include std_common.i
%include exception.i

%define MY_OUTPUT_TYPEMAP(CTYPE, JNITYPE, JTYPE, JAVATYPE, JNIDESC, TYPECHECKTYPE)
%typemap(jni) CTYPE *OUTPUT %{JNITYPE##Array%}
%typemap(jtype) CTYPE *OUTPUT "JTYPE[]"
%typemap(jstype) CTYPE *OUTPUT "JTYPE[]"
%typemap(javain) CTYPE *OUTPUT "$javainput"
%typemap(javadirectorin) CTYPE *OUTPUT "$jniinput"
%typemap(javadirectorout) CTYPE *OUTPUT "$javacall"

%typemap(jni) CTYPE &OUTPUT %{JNITYPE##Array%}
%typemap(jtype) CTYPE &OUTPUT "JTYPE[]"
%typemap(jstype) CTYPE &OUTPUT "JTYPE[]"
%typemap(javain) CTYPE &OUTPUT "$javainput"
%typemap(javadirectorin) CTYPE &OUTPUT "$jniinput"
%typemap(javadirectorout) CTYPE &OUTPUT "$javacall"

%typemap(in) CTYPE *OUTPUT($*1_ltype temp), CTYPE &OUTPUT($*1_ltype temp)
{
  if (!$input) {
    SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "array null");
    return $null;
  }
  if (JCALL1(GetArrayLength, jenv, $input) == 0) {
    SWIG_JavaThrowException(jenv, SWIG_JavaIndexOutOfBoundsException, "Array must contain at least 1 element");
    return $null;
  }
  $1 = &temp; 
}

%typemap(directorin,descriptor=JNIDESC) CTYPE &OUTPUT
%{ *(($&1_ltype) $input = &$1; %}

%typemap(directorin,descriptor=JNIDESC) CTYPE *OUTPUT
%{
#error "Need to provide OUT directorin typemap, CTYPE array length is unknown"
%}

%typemap(argout) CTYPE *OUTPUT, CTYPE &OUTPUT 
{
  JNITYPE jvalue = (JNITYPE)temp$argnum;
  JCALL4(Set##JAVATYPE##ArrayRegion, jenv, $input, 0, 1, &jvalue);
}

%typemap(typecheck) CTYPE *INOUT = TYPECHECKTYPE;
%typemap(typecheck) CTYPE &INOUT = TYPECHECKTYPE;
%enddef


%define wrap_std_vector_p(T,N)
%{
    class N {
        // add generic typemaps here
      private:
	std::vector<T> *pimpl_;
      public:
        N(std::vector<T> *v) : pimpl_(v) {}
        N() : pimpl_(new std::vector<T>()) {}
	std::vector<T> *getVec(){return pimpl_;}
        unsigned int size() const {return pimpl_->size();}
        bool empty() const {return pimpl_->empty();}
        void clear(){pimpl_->clear();}
        void push_back(const T& x){pimpl_->push_back(const_cast<T&>(x));}
            T pop() {
                if (pimpl_->size() == 0)
                    throw std::out_of_range("pop from empty vector");
                T x = pimpl_->back();
                pimpl_->pop_back();
                return x;
            }
            T get(int i) {
                int size = int(pimpl_->size());
                if (i>=0 && i<size)
                    return (*pimpl_)[i];
                else
                    throw std::out_of_range("vector index out of range");
            }
            void set(int i, const T& x) {
                int size = int(pimpl_->size());
                if (i>=0 && i<size)
                    (*pimpl_)[i] = const_cast<T&>(x);
                else
                    throw std::out_of_range("vector index out of range");
            }
       
    };
%}
    class N {
      public:
        N();
        unsigned int size() const;
        bool empty() const;
        void clear();
        void push_back(const T& x);
        T pop();
        T get(int i);
        void set(int i, const T& x);
       
    };

namespace std {
%typemap(ruby, in) vector<T>* {
	     void *ptr;
		//zobin
	     SWIG_ConvertPtr($input, &ptr, $&1_descriptor, 1);
	     $1_type o = ($1_type)ptr;
	     $1 = $o->getVec();

        }
%typemap(ruby, out) vector<T>* {
	//zobout
	N *n = new N($1); 
	$result = SWIG_NewPointerObj((void *) n, $descriptor(N *), 0);
}
%typemap(perl5, in) vector<T>* {
	     void *ptr;
		//zobin
	     SWIG_ConvertPtr($input, &ptr, $&1_descriptor, 1);
	     $1_type o = ($1_type)ptr;
	     $1 = $o->getVec();

        }
%typemap(perl5, out) vector<T>* {
	//zobout
	SV *res = sv_newmortal();
	N *n = new N($1); 
	SWIG_MakePtr(res, (void *) n,$descriptor(N *), SWIG_SHADOW|0);
	$result=res;
	argvi++;
}	

%typemap(php4, out) vector<T>* {
	N *n = new N($1); 
	//zobout
	if (!$1) {
		ZVAL_NULL(return_value);
	} else {
		SWIG_SetPointerZval(return_value, (void *)n, $descriptor(N*),0);
	}
    /* Wrap this return value */
    {
        /* ALTERNATIVE Constructor, make an object wrapper */

        zval *obj, *_cPtr;
        MAKE_STD_ZVAL(obj);
        MAKE_STD_ZVAL(_cPtr);
        *_cPtr = *return_value;
        INIT_ZVAL(*return_value);
        object_init_ex(obj,ptr_ce_swig_##N);
        add_property_zval(obj,"_cPtr",_cPtr);
        *return_value=*obj;
    }
}	

%typemap(python, out) vector<T>* {
     	N *n = new N($1);
    $result = SWIG_NewPointerObj((void*)(n),$descriptor(N *) , 0);

}

%typemap(tcl8, out) vector<T>* {
     	N *n = new N($1);
    	Tcl_SetObjResult(interp,SWIG_NewInstanceObj((void *) n, $descriptor(N *),0));

}

//OUTPUT_TYPEMAP(unsigned long long, jobject, java.math.BigInteger, NOTUSED, "[Ljava/lang/BigInteger;", SWIGBIGINTEGERARRAY);

//MY_OUTPUT_TYPEMAP(vector<T>*, jobject, N##Vector, NOTUSED, "[LN##Vector;", SWIGBIGINTEGERARRAY);
%typemap(jni) vector<T>* "jlong"
//%typemap(jtype) vector<T>* "N"
%typemap(jtype) vector<T>* "long"
%typemap(jstype) vector<T>* "N"
%typemap(javadirectorin) vector<T>* "$jniinput"
%typemap(javadirectorout) vector<T>* "$javacall"


%typemap(javaout) vector<T>* {
    long cPtr = $jnicall;
    return (cPtr == 0) ? null : new N(cPtr, false);      
	//return $jnicall ;
}

%typemap(java, out) vector<T>* {
     	N *n = new N($1);
	$result = (long)n;
}




/*
%typemap(javaout) vector<T>& {
	return new N($jnicall, $owner);
}
*/


%typemap(freearg) vector<T>* {
	delete $1;
}

}
%enddef
