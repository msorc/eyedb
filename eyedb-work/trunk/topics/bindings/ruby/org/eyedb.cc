extern "C"{
#include "ruby.h"
#include<stdio.h>
}
#include<iostream>
#include<eyedb/eyedb.h> 
VALUE eyedb_exception_class;
VALUE eyedb_class;
VALUE eyedb_vars_class;
VALUE eyedb_object_class;

/*
typedef struct _eyedbvars
{
	eyedb::Database * db;
	bool closed;	
} eyedbvars;
*/


static VALUE rb_eyedb_test(int argc, VALUE * argv , VALUE obj)
{
	VALUE val;
	rb_scan_args(argc,argv,"10",&val);
	//printf("Value : %s",StringValueCStr(val));

	return rb_funcall(rb_stdout,rb_intern("puts"),1,val);
};

static VALUE rb_eyedb_close(int argc, VALUE * argv , VALUE obj)
{


	eyedb::release();

	return Qtrue;


}

static VALUE rb_eyedb_new(int argc, VALUE * argv , VALUE obj)
{
	eyedb::init();
	VALUE server;
	VALUE user;
	VALUE pass;
	VALUE port;
	VALUE dbname;
	rb_scan_args(argc,argv,"14",&dbname,&user,&pass,&server,&port);
	if(NIL_P(dbname))
	{
		rb_raise(rb_eArgError,"Database name can't be nil");
	}

	if(NIL_P(server))
	{
		server=rb_str_new2("localhost");
	}

	if(NIL_P(user))
	{
		user=rb_str_new2("root");
	}

	if(NIL_P(pass))
	{
		pass=rb_str_new2("");
	}

	if(NIL_P(port) || !FIXNUM_P(port))
	{
		port=INT2NUM(6240);
	}
	/*
	   rb_funcall(rb_stdout,rb_intern("puts"),2,rb_str_new2("User : "),user);
	   rb_funcall(rb_stdout,rb_intern("puts"),2,rb_str_new2("Pass : "),pass);
	   rb_funcall(rb_stdout,rb_intern("puts"),2,rb_str_new2("Server : "),server);
	   rb_funcall(rb_stdout,rb_intern("puts"),2,rb_str_new2("Port : "),port);
	   */
	VALUE pt = rb_funcall(port,rb_intern("to_s"),0);

	try {
		eyedb::Exception::setMode(eyedb::Exception::ExceptionMode);

		eyedb::Connection *  conn=new eyedb::Connection();
		// connecting to the eyedb server
		conn->open(StringValueCStr(server),StringValueCStr(pt));
		eyedb::Database * db= new eyedb::Database(StringValueCStr(dbname));

		// opening database argv[1]
		(*db).open(conn, eyedb::Database::DBRW,StringValueCStr(user));
		//eyedbvars * dats;
		rb_iv_set(obj,"eyedbvars",Data_Wrap_Struct(eyedb_vars_class,0,NULL,db));

	} catch(eyedb::Exception &e) {
		rb_raise(eyedb_exception_class,"%s",e.getDesc());
		eyedb::release();
		return Qfalse;
	}


	return obj;
}

static VALUE rb_eyedb_query(int argc, VALUE * argv , VALUE obj)
{
	VALUE query;
	rb_scan_args(argc,argv,"10",&query);

	if(NIL_P(query))
	{
		rb_raise(rb_eArgError,"Query can't be nil");
	}

	try {
		//eyedbvars * dats;
		eyedb::Database * db;
		Data_Get_Struct(rb_iv_get(obj,"eyedbvars"),eyedb::Database ,db);
	//	std::cerr << db;
	//	puts(db->getName());
	//	puts("0");
		db->transactionBegin();
	//	puts("1");
	//	puts(StringValueCStr(query));
		// performing the OQL query argv[2] using the eyedb::OQL interface
		eyedb::OQL q(db,StringValueCStr(query));

		eyedb::ValueArray arr;
		q.execute(arr);
		VALUE ret=rb_funcall(rb_cArray,rb_intern("new"),1,INT2NUM(arr.getCount()));
		for (int i = 0; i < arr.getCount(); i++)
		{

		VALUE v=rb_funcall(eyedb_object_class,rb_intern("new"),0);
		rb_iv_set(v,"eyedbval",Data_Wrap_Struct(eyedb_object_class,0,NULL,(void *)&(arr[i])));
		rb_funcall(v,rb_intern("init"),0);
		rb_funcall(ret,rb_intern("[]="),2,INT2NUM(i),v);
		}



		db->transactionCommit();
		return ret;
	} catch(eyedb::Exception &e) {
		rb_raise(eyedb_exception_class,"%s",e.getDesc());
		return Qfalse;
	}
	return Qtrue;
}


static VALUE rb_eyedb_o_new(int argc, VALUE * argv , VALUE obj)
{
	rb_iv_set(obj,"initDone",Qfalse);
}

static VALUE rb_eyedb_o_init(int argc, VALUE * argv , VALUE obj)
{
	if(rb_iv_get(obj,"initDone")==Qtrue){return Qtrue;}
	try{
		eyedb::Value * v;
		Data_Get_Struct(rb_iv_get(obj,"eyedbval"),eyedb::Value,v);
		//printf("%d %s %s\n",v->getType(),v->getStringType(),v->getString());
		switch(v->getType())
		{
		case eyedb::Value::tOid :
			rb_iv_set(obj,"@oid",rb_str_new2(v->getString()));
			rb_iv_set(obj,"@rtype",rb_str_new2("OID"));
			break;
		case eyedb::Value::tString :
		case eyedb::Value::tChar :
			rb_iv_set(obj,"@string",rb_str_new2((const char *)v->getData()));
			rb_iv_set(obj,"@rtype",rb_str_new2("string"));
			break;
		case eyedb::Value::tInt :
		case eyedb::Value::tLong :
		case eyedb::Value::tShort :

			rb_iv_set(obj,"@int",rb_funcall(rb_str_new2((const char *)v->getString()),rb_intern("to_i"),0));
			rb_iv_set(obj,"@rtype",rb_str_new2("int"));
			break;
		case eyedb::Value::tDouble :

			rb_iv_set(obj,"@float",rb_funcall(rb_str_new2((const char *)v->getString()),rb_intern("to_f"),0));
			rb_iv_set(obj,"@rtype",rb_str_new2("float"));
			break;

		case eyedb::Value::tNull :
		case eyedb::Value::tNil :
			rb_iv_set(obj,"@rtype",rb_str_new2("nil"));

			break;

		
		}

	} catch(eyedb::Exception &e) {
		rb_raise(eyedb_exception_class,"%s",e.getDesc());
		return Qfalse;

	}

	rb_iv_set(obj,"initDone",Qtrue);
}

	extern "C"{
void Init_eyedb()
{
	eyedb_class=rb_define_class("EyeDB",rb_cObject);
	eyedb_object_class=rb_define_class("EyeDBObject",rb_cObject);
	eyedb_vars_class=rb_define_class("EyeDBVars",rb_cObject);
	eyedb_exception_class=rb_define_class("EyeDBException",rb_eStandardError);
	rb_define_method(eyedb_class,"initialize",(VALUE(*)(ANYARGS))rb_eyedb_new,-1);
	rb_define_method(eyedb_class,"query",(VALUE(*)(ANYARGS))rb_eyedb_query,-1);
	rb_define_method(eyedb_class,"close",(VALUE(*)(ANYARGS))rb_eyedb_close,-1);
	
	rb_define_private_method(eyedb_object_class,"init",(VALUE(*)(ANYARGS))rb_eyedb_o_init,-1);
	rb_define_method(eyedb_object_class,"initialize",(VALUE(*)(ANYARGS))rb_eyedb_o_new,-1);

	//rb_define_method()
}
}
