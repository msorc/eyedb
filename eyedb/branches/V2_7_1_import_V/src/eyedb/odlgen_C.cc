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


#include "eyedb_p.h"
#include "CollectionBE.h"
#include "Attribute_p.h"
#include "odl.h"
#include <string.h>

//#define DEF_PREFIX "idb"
#define DEF_PREFIX ""
//#define DEF_PREFIX2 "idb"
#define DEF_PREFIX2 ""

#define NEW_ARGARR // 8/02/03
#define NO_COMMENTS
#define NEW_COMP_POLICY
#define IDBBOOL_STR(X) ((X) ? "True" : "False")

#define ATTR_CACHE_DIRECT

//#define NO_DIRECT_SET // added the 22/09/98
#define SET_COUNT_DIRECT // added the 27/10/98

namespace eyedb {

  static void
  gbx_suspend(GenContext *ctx)
  {
    fprintf(ctx->getFile(), "%sgbxAutoGarbSuspender _gbxsusp_;\n",
	    ctx->get());
  }

  static const char classSuffix[] = "_Class";
  static const char __agritems[] = "getClass()->getAttributes()";
  static const char __agrdyn[] = "_attr";
  static const char downCastPrefix[] = "as";
  static Bool class_enums;
  static Bool attr_cache;
  static const char enum_type[] = "Type";

#define STRBOOL(X) ((X) ? "True" : "False")

#define IS_STRING() (typmod.ndims == 1 && \
                     !strcmp(cls->getName(), char_class_name) && \
                     !isIndirect())

#define IS_RAW() (typmod.ndims == 1 && \
		  !strcmp(cls->getName(), byte_class_name) && \
		  !isIndirect())

#define SCLASS()  (is_string ? "char" : "unsigned char")
#define SARGIN()  (is_string ? "" : ", unsigned int len")
#define SARGOUT() ((is_raw && isVarDim()) ? "unsigned int *len, " : "")

  static char *
  purge(const char *x)
  {
    char *s = (char *)malloc(2 * strlen(x) + 1);
    char *p = s;
    char c;

    while (c = *x++)
      {
	if (c == '"')
	  {
	    *p++ = '\\';
	    *p++ = '"';
	  }
	else if (c == '\\')
	  {
	    *p++ = '\\';
	    *p++ = '\\';
	  }
	else if (c == '\n')
	  *p++ = ' ';
	else
	  *p++ = c;
      }

    *p = 0;
    return s;
  }

  static const char *
  atc_set(const char *name)
  {
    static char _set[128];
    sprintf(_set, "__%s_isset__", name);
    return _set;
  }

  static const char *
  atc_cnt(const char *name)
  {
    static char _cnt[128];
    sprintf(_cnt, "__%s_cnt__", name);
    return _cnt;
  }

  static const char *
  atc_name(const char *name)
  {
    static char _name[128];
    sprintf(_name, "__%s__", name);
    return _name;
  }

  static const char *
  atc_this(const char *classname)
  {
    static char _this[128];
    sprintf(_this, "((%s *)this)", classname);
    //  return "__this__";
    return _this;
  }

#define RETURN_ERROR(FD, CTX, MCLASS, INDENT) \
do { \
  if ((MCLASS)->asEnumClass()) \
    fprintf((FD), "%s%sif (s) {if (rs) *rs = s; return (%s)0;}\n", (CTX)->get(), INDENT, \
	    className(MCLASS, False)); \
  else \
    fprintf((FD), "%s%sif (s) {if (rs) *rs = s; return 0;}\n", (CTX)->get(), INDENT); \
} while(0)

#define ATTRNAME(NAME, OPTYPE, HINTS) \
  (HINTS).style->getString(GenCodeHints::OPTYPE, NAME)

#define ATTRNAME_1(NAME, OPTYPE, HINTS) \
  (HINTS).style->getString(OPTYPE, NAME)

#define ATTRGET(CL) ((CL)->asCollectionClass() ? \
		     GenCodeHints::GETCOLL : GenCodeHints::GET)

#define ATTRSET(CL) ((CL)->asCollectionClass() ? \
		     GenCodeHints::SETCOLL : GenCodeHints::SET)

  static const GenCodeHints *phints;

  static const char *
  make_int(int i)
  {
    static char x[16];
    sprintf(x, "%d", i);
    return x;
  }

  static int
  pre(char *name, const char *pre)
  {
    int len = strlen(pre);
    if (!strncmp(name, pre, len))
      {
	name[len] += 'A' - 'a';
	return 1;
      }
    return 0;
  }

  static const char *className(const Class *cls,
			       Bool makeC = True,
			       Bool makeAlias = False)
  {
    const char *name = (makeAlias ? cls->getAliasName() : cls->getName());

    if (!strncmp(name, "set<", 4))
      return "CollSet";
  
    if (!strncmp(name, "bag<", 4))
      return "CollBag";
  
    if (!strncmp(name, "array<", 6))
      return "CollArray";
  
    if (!strncmp(name, "list<", 5))
      return "CollList";

    for (int i = 0; i < idbLAST_Type; i++)
      if (!strcmp(name, class_info[i].name))
	return Class::classNameToCName(name);

    if (makeC) {
      if (!strcmp(name, char_class_name))
	return "Char";

      if (!strcmp(name, int32_class_name))
	return "Int32";
    
      if (!strcmp(name, int64_class_name))
	return "Int64";
      
      if (!strcmp(name, int16_class_name))
	return "Int16";
      
      if (!strcmp(name, "float"))
	return "Float";

      if (!strcmp(name, "oid"))
	return "OidP";

      if (!strcmp(name, "byte"))
	return "Byte";
    }
    else {
      if (!strcmp(name, int32_class_name))
	return "eyedblib::int32";
    
      if (!strcmp(name, int64_class_name))
	return "eyedblib::int64";
    
      if (!strcmp(name, int16_class_name))
	return "eyedblib::int16";
    
      if (!strcmp(name, "oid"))
	return "Oid";
    
      if (!strcmp(name, "byte"))
	return "unsigned char";
    
      if (!strcmp(name, "float"))
	return "double";
    }

    if (cls->asEnumClass() && class_enums) {
      if (Class::isBoolClass(cls)) {
	return cls->getCName(True);
      }
      static std::string enum_s;
      enum_s = std::string(name) + "::" + enum_type;
      return enum_s.c_str();
    }

    const char *sCName = Class::getSCName(name);
    return sCName ? sCName : name;
  }

  static void dimArgsGen(FILE *fd, int ndims, Bool named = True)
  {
    for (int i = 0; i < ndims; i++)
      {
	if (i)
	  fprintf(fd, ", ");
	fprintf(fd, "unsigned int");
	if (named)
	  fprintf(fd, " a%d", i);
      }
  }

  Status Attribute::generateClassDesc_C(GenContext *ctx)
  {
    FILE *fd = ctx->getFile();
    int ndims = typmod.ndims;

    fprintf(fd, "\n");
    if (ndims)
      {
	fprintf(fd, "%sdims = new int[%d];\n", ctx->get(), ndims);
	for (int i = 0; i < ndims; i++)
	  fprintf(fd, "%sdims[%d] = %d;\n", ctx->get(), i, typmod.dims[i]);
      }
    else
      fprintf(fd, "%sdims = 0;\n", ctx->get());

    fprintf(fd, "%sattr[%d] = new Attribute(", ctx->get(), num);

    if (cls->asCollectionClass())
      fprintf(fd, "(m ? m->getClass(\"%s\") : %s%s), \"%s\", ",
	      cls->getAliasName(), cls->getCName(), classSuffix, name);
    else if (cls->asEnumClass())
      fprintf(fd, "(m ? m->getClass(\"%s\") : %s%s), \"%s\", ",
	      cls->getAliasName(), cls->getCName(False), classSuffix, name);
    else
      fprintf(fd, "(m ? m->getClass(\"%s\") : %s%s), \"%s\", ",
	      cls->getAliasName(), className(cls), classSuffix, name);

    fprintf(fd, "%s, %d, dims);\n", (isIndirect() ? "True" : "False"), ndims);

    /*
      fprintf(fd, "%sattr[%d]->setMagOrder(%d);\n", ctx->get(), num,
      getMagOrder());
    */

    if (ndims)
      fprintf(fd, "%sdelete[] dims;\n", ctx->get());

    if (inv_spec.clsname)
      fprintf(fd, "%sattr[%d]->setInverse(\"%s\", \"%s\");\n", ctx->get(), num,
	      inv_spec.clsname, inv_spec.fname);
    else if (inv_spec.item)
      fprintf(fd, "%sattr[%d]->setInverse(\"%s\", \"%s\");\n", ctx->get(), num,
	      inv_spec.item->getClassOwner()->getName(), inv_spec.item->getName());

    return Success;
  }

  enum OP {
    op_GET = 1,
    op_SET,
    op_OTHER
  };

  static void
  dynamic_attr_gen(FILE *fd, GenContext *ctx,
		   const Attribute *attr,
		   OP set,
		   Bool isoid = False,
		   Bool is_string = False,
		   const char *cast = 0)
  {
    fprintf(fd, "%sconst Attribute *%s = getClass()->getAttribute(\"%s\");\n",
	    ctx->get(), __agrdyn, attr->getName());
    fprintf(fd, "%sif (!%s) {\n", ctx->get(), __agrdyn);
    ctx->push();
    if (set == op_GET && !isoid)
      fprintf(fd, "%sif (isnull) *isnull = True;\n", ctx->get());
    fprintf(fd, "%sif (dyn%s_error_policy) {\n", ctx->get(), (set ? "set" : "get"));
    fprintf(fd, "%s  Status s = Exception::make(IDB_ATTRIBUTE_ERROR, "
	    "\"object %%s: attribute %%s::%%s not found\", oid.toString(), getClass()->getName(), \"%s\");\n",
	    ctx->get(), attr->getName());
    if (set == op_SET) {
      fprintf(fd, "%s  return s;\n", ctx->get());
      fprintf(fd, "%s}\n", ctx->get());
      fprintf(fd, "%sreturn Success;\n", ctx->get());
    }
    else {
      fprintf(fd, "%s  if (rs) *rs = s;\n", ctx->get());
      fprintf(fd, "%s}\n", ctx->get());
      if (isoid)
	fprintf(fd, "%sreturn nulloid;\n", ctx->get());
      else if (is_string)
	fprintf(fd, "%sreturn \"\";\n", ctx->get());
      else if (cast)
	fprintf(fd, "%sreturn (%s)0;\n", ctx->get(), cast);
      else
	fprintf(fd, "%sreturn 0;\n", ctx->get());
    }
    ctx->pop();
    fprintf(fd, "%s}\n\n", ctx->get());
  }

  Status
  Attribute::generateCollRealizeClassMethod_C(Class *own,
					      GenContext *ctx,
					      const GenCodeHints &hints,
					      Bool isoid,
					      int xacctype)
  {
    FILE *fd = ctx->getFile();
    int ndims = typmod.ndims, i;
    const char *comma = (ndims ? ", " : "");
    Bool _isref;
    eyedblib::int16 _dim;
    Class *cl =
      ((CollectionClass *)cls)->getCollClass(&_isref, &_dim);
    GenCodeHints::OpType acctype = (GenCodeHints::OpType)xacctype;
    const char *etc = (acctype == GenCodeHints::ADD_ITEM_TO_COLL ?
		       ", const IndexImpl *idximpl" : "");
    const char *accmth = (acctype == GenCodeHints::ADD_ITEM_TO_COLL ? "insert" : "suppress");
    const char *oclassname = _isref ?
      className(cl, True) : className(cl, False);

    if (isoid && cl->asBasicClass())
      return Success;

    const char *cast = (cl->asBasicClass() ? "(Value)" : "");

    const char *classname;
    Bool ordered;
    CollectionClass *mcoll = (CollectionClass *)cls;
    if (mcoll->asCollSetClass())
      {
	classname = "CollSet";
	ordered = False;
      }
    else if (mcoll->asCollBagClass())
      {
	classname = "CollBag";
	ordered = False;
      }
    else if (mcoll->asCollArrayClass())
      {
	classname = "CollArray";
	ordered = True;
      }
    else if (mcoll->asCollListClass())
      {
	classname = "CollList";
	ordered = True;
      }

    const char *At = ((acctype == GenCodeHints::ADD_ITEM_TO_COLL ||
		       acctype == GenCodeHints::RMV_ITEM_FROM_COLL)
		      && ordered ? "At" : "");

    const char *etc1 = "";
    const char *etc2 = "";
    Bool rmv_from_array = (acctype == GenCodeHints::RMV_ITEM_FROM_COLL)
      && ordered ? True : False;

    if (rmv_from_array && isoid)
      return Success;

    GenCodeHints::OpType nacctype;
    if (ordered)
      nacctype = (acctype == GenCodeHints::RMV_ITEM_FROM_COLL ?
		  GenCodeHints::UNSET_ITEM_IN_COLL :
		  GenCodeHints::SET_ITEM_IN_COLL);
    else
      nacctype = acctype;

    if (!*At)
      {
	if (acctype == GenCodeHints::ADD_ITEM_TO_COLL)
	  {
	    etc1 = ", Bool noDup";
	    etc2 = ", noDup";
	  }
	else if (acctype == GenCodeHints::RMV_ITEM_FROM_COLL)
	  {
	    etc1 = ", Bool checkFirst";
	    etc2 = ", checkFirst";
	  }
      }

    if (_dim == 1)
      {
	fprintf(fd, "Status %s::%s(%s%s", className(own),
		ATTRNAME_1(name, nacctype, hints),
		(*At ? "int where" : ""),
		(*At && (!rmv_from_array || ndims >= 1) ? ", " : ""));
	dimArgsGen(fd, ndims);
	if (rmv_from_array)
	  fprintf(fd, ")\n{\n");
	else if (isoid)
	  fprintf(fd, "%sconst Oid &_oid%s)\n{\n", comma, etc);
	else
	  fprintf(fd, "%s%s %s_%s%s%s)\n{\n", comma, oclassname,
		  (_isref || !cl->asBasicClass() ? "*" : ""), name,
		  etc1, etc);
      }
    else if (!strcmp(cl->getName(), char_class_name) && _dim > 1)
      {
	fprintf(fd, "Status %s::%s(%s", className(own),
		ATTRNAME_1(name, nacctype, hints),
		(*At ? "int where, " : ""));
	dimArgsGen(fd, ndims);
	fprintf(fd, "%sconst char *_%s%s%s)\n{\n", comma, name, etc1, etc);
      }
    else
      return Success;

    gbx_suspend(ctx);

    fprintf(fd, "%sStatus status;\n", ctx->get());

    fprintf(fd, "%s%s *_coll;\n", ctx->get(), classname);
    fprintf(fd, "%sBool _not_set = False;\n", ctx->get());
    if (ndims > 1)
      {
	fprintf(fd, "%sSize from = a%d;\n", ctx->get(), ndims-1);
	for (int i = ndims - 2 ; i >= 0; i--)
	  fprintf(fd, "%sfrom += a%d * %d;\n", ctx->get(), i, typmod.dims[i]);
      }
    else
      fprintf(fd, "%sSize from = 0;\n", ctx->get());
  
    if (odl_dynamic_attr) {
      dynamic_attr_gen(fd, ctx, this, op_SET, isoid);
      fprintf(fd, "\n%sstatus = %s->getValue(this, (Data *)&_coll, 1, from);\n",
	      ctx->get(), __agrdyn, name);
    }
    else
      fprintf(fd, "\n%sstatus = %s[%d]->getValue(this, (Data *)&_coll, 1, from);\n",
	      ctx->get(), __agritems, num, name);
    fprintf(fd, "%sif (status)\n%s  return status;\n\n", ctx->get(), ctx->get());
    fprintf(fd, "%sif (!_coll)\n", ctx->get());
    fprintf(fd, "%s  {\n", ctx->get());
    ctx->push();
    fprintf(fd, "%s _not_set = True;\n", ctx->get());
    fprintf(fd, "%s Oid _coll_oid;\n", ctx->get());
    if (odl_dynamic_attr)
      fprintf(fd, "%s status = %s->getOid(this, &_coll_oid, 1, from);\n",
	      ctx->get(), __agrdyn, name);
    else
      fprintf(fd, "%s status = %s[%d]->getOid(this, &_coll_oid, 1, from);\n",
	      ctx->get(), __agritems, num, name);
    fprintf(fd, "%s if (status)\n%s  return status;\n\n", ctx->get(), ctx->get());
    fprintf(fd, "%s if (_coll_oid.isValid())\n", ctx->get());
    fprintf(fd, "%s   {\n", ctx->get());
    ctx->push();
    fprintf(fd, "%s status = db->loadObject(&_coll_oid, (Object **)&_coll);\n",
	    ctx->get());
    fprintf(fd, "%s if (status)\n%s  return status;\n", ctx->get(), ctx->get());
    ctx->pop();
    fprintf(fd, "%s   }\n", ctx->get());
    fprintf(fd, "%s else\n", ctx->get());
    if (acctype == GenCodeHints::ADD_ITEM_TO_COLL)
      {
	fprintf(fd, "%s   {\n", ctx->get());
	fprintf(fd, "%s     _coll = new %s(db, \"\", "
		"db->getSchema()->getClass(\"%s\"), %s, idximpl);\n",
		ctx->get(), classname,
		cl->getAliasName(), (_isref ? "True" : make_int(_dim)));
	//fprintf(fd, "%s     _coll->setMagOrder(mag_order);\n", ctx->get());
	fprintf(fd, "%s   }\n", ctx->get());
      }
    else
      fprintf(fd, "%s    return Exception::make(IDB_ERROR, \"no valid collection in attribute %s::%s\");\n\n", ctx->get(), class_owner->getName(), name);

    ctx->pop();
    fprintf(fd, "%s  }\n", ctx->get());
    if (rmv_from_array)
      fprintf(fd, "\n%sstatus = _coll->suppressAt(where);\n", ctx->get());
    else if (isoid)
      fprintf(fd, "\n%sstatus = _coll->%s%s(%s_oid);\n", ctx->get(),
	      accmth, At, (*At ? "where, " : ""));
    else
      fprintf(fd, "\n%sstatus = _coll->%s%s(%s%s_%s%s);\n", ctx->get(),
	      accmth, At, (*At ? "where, " : ""),  cast, name,
	      etc2);
    fprintf(fd, "%sif (status || !_not_set)\n%s  return status;\n\n", ctx->get(), ctx->get());
    if (odl_dynamic_attr)
      fprintf(fd, "\n%sstatus = %s->setValue(this, (Data)&_coll, 1, from);\n",
	      ctx->get(), __agrdyn);
    else
      fprintf(fd, "\n%sstatus = %s[%d]->setValue(this, (Data)&_coll, 1, from);\n",
	      ctx->get(), __agritems, num);

    fprintf(fd, "%s   _coll->release();\n", ctx->get());
    fprintf(fd, "\n%sreturn status;\n", ctx->get());
    fprintf(fd, "}\n\n");
    return Success;
  }

  Status
  Attribute::generateCollInsertClassMethod_C(Class *own,
					     GenContext *ctx,
					     const GenCodeHints &hints,
					     Bool isoid)
  {
    return generateCollRealizeClassMethod_C(own, ctx, hints,
					    isoid,
					    GenCodeHints::ADD_ITEM_TO_COLL);
  }

  Status
  Attribute::generateCollSuppressClassMethod_C(Class *own,
					       GenContext *ctx,
					       const GenCodeHints &hints,
					       Bool isoid)
  {
    return generateCollRealizeClassMethod_C(own, ctx, hints,
					    isoid, 
					    GenCodeHints::RMV_ITEM_FROM_COLL);
  }

  Status
  Attribute::generateSetMethod_C(Class *own, GenContext *ctx,
				 const GenCodeHints &hints)
  {
    if (cls->asCollectionClass())
      {
	(void)generateCollInsertClassMethod_C(own, ctx, hints, False);
	(void)generateCollInsertClassMethod_C(own, ctx, hints, True);

	(void)generateCollSuppressClassMethod_C(own, ctx, hints, False);
	(void)generateCollSuppressClassMethod_C(own, ctx, hints, True);
      }
    return Success;
  }

#define NOT_BASIC() \
	(isIndirect() || (!cls->asBasicClass() && !cls->asEnumClass()))

  Status
  Attribute::generateSetMethod_C(Class *own, GenContext *ctx,
				 Bool isoid,
				 const GenCodeHints &hints)
  {
    FILE *fd = ctx->getFile();
    int ndims = typmod.ndims, i;
    int not_basic = NOT_BASIC();
    const char *ref = (not_basic ? "*" : "");
    const char *comma = (ndims ? ", " : "");
    const char *classname = isIndirect() ?
      className(cls, True) : className(cls, False);

    if (isoid)
      fprintf(fd, "Status %s::%s(", className(own),
	      ATTRNAME(name, SETOID, hints));
    else
      fprintf(fd, "Status %s::%s(", className(own),
	      ATTRNAME_1(name, ATTRSET(cls), hints));

    dimArgsGen(fd, ndims, True);

    if (isoid)
      fprintf(fd, "%sconst Oid &_oid)\n{\n", comma);
    else if (cls->asEnumClass())
      fprintf(fd, "%s%s %s_%s, Bool _check_value)\n{\n",
	      comma, classname, ref, name);
    else
      fprintf(fd, "%s%s %s_%s)\n{\n", comma, classname, ref, name);

    GenCodeHints::OpType optype = (isoid ? GenCodeHints::SETOID:
				   GenCodeHints::SET);

    if (attr_cache)
      genAttrCacheSetPrologue(ctx, optype);

    if (odl_dynamic_attr)
      dynamic_attr_gen(fd, ctx, this, op_SET, isoid);

    gbx_suspend(ctx);

    fprintf(fd, "%sStatus status;\n", ctx->get());

    if (ndims)
      {
	fprintf(fd, "%sSize from = a%d;\n", ctx->get(), ndims-1);
	for (int i = ndims - 2 ; i >= 0; i--)
	  fprintf(fd, "%sfrom += a%d * %d;\n", ctx->get(), i, typmod.dims[i]);

	if (isVarDim())
	  {
	    fprintf(fd, "\n%sSize size;\n", ctx->get());
	    if (odl_dynamic_attr)
	      fprintf(fd, "%sstatus = %s->getSize(this, size);\n",
		      ctx->get(), __agrdyn);
	    else
	      fprintf(fd, "%sstatus = %s[%d]->getSize(this, size);\n",
		      ctx->get(), __agritems, num);
	    fprintf(fd, "%sif (status)\n%s  return status;\n\n", ctx->get(),
		    ctx->get());
	    fprintf(fd, "%sif (size <= from)\n", ctx->get());
	    ctx->push();
	    if (odl_dynamic_attr)
	      fprintf(fd, "%sstatus = %s->setSize(this, from+1);\n",
		      ctx->get(), __agrdyn);
	    else
	      fprintf(fd, "%sstatus = %s[%d]->setSize(this, from+1);\n",
		      ctx->get(), __agritems, num);
	    ctx->pop();
	    fprintf(fd, "%sif (status)\n%s  return status;\n", ctx->get(),
		    ctx->get());
	  }

	if (isoid)
	  {
	    if (odl_dynamic_attr)
	      fprintf(fd, "\n%sstatus = %s->setOid(this, &_oid, 1, from, oid_check);\n",
		      ctx->get(), __agrdyn);
	    else
	      fprintf(fd, "\n%sstatus = %s[%d]->setOid(this, &_oid, 1, from, oid_check);\n",
		      ctx->get(), __agritems, num);
	    if (attr_cache)
	      genAttrCacheSetEpilogue(ctx, optype);
	    fprintf(fd, "%sreturn status;\n", ctx->get());
	  }

	else if (cls->asEnumClass())
	  {
	    fprintf(fd, "%seyedblib::int32 __tmp = _%s;\n", ctx->get(), name);
	    if (odl_dynamic_attr)
	      fprintf(fd, "\n%sstatus = %s->setValue(this, (Data)&__tmp, 1, from, _check_value);\n",
		      ctx->get(), __agrdyn, name);
	    else
	      fprintf(fd, "\n%sstatus = %s[%d]->setValue(this, (Data)&__tmp, 1, from, _check_value);\n",
		      ctx->get(), __agritems, num, name);
	    if (attr_cache)
	      genAttrCacheSetEpilogue(ctx, optype);
	    fprintf(fd, "%sreturn status;\n", ctx->get());
	  }
	else
	  {
	    if (odl_dynamic_attr)
	      fprintf(fd, "\n%sstatus = %s->setValue(this, (Data)&_%s, 1, from);\n",
		      ctx->get(), __agrdyn, name);
	    else
	      fprintf(fd, "\n%sstatus = %s[%d]->setValue(this, (Data)&_%s, 1, from);\n",
		      ctx->get(), __agritems, num, name);
	    if (attr_cache)
	      genAttrCacheSetEpilogue(ctx, optype);
	    fprintf(fd, "%sreturn status;\n", ctx->get());
	  }
      }
    else if (isoid)
      {
	if (odl_dynamic_attr)
	  fprintf(fd, "\n%sstatus = %s->setOid(this, &_oid, 1, 0, oid_check);\n",
		  ctx->get(), __agrdyn);
	else
	  fprintf(fd, "\n%sstatus = %s[%d]->setOid(this, &_oid, 1, 0, oid_check);\n",
		  ctx->get(), __agritems, num);
	if (attr_cache)
	  genAttrCacheSetEpilogue(ctx, optype);
	fprintf(fd, "%sreturn status;\n", ctx->get());
      }
    else if (cls->asEnumClass())
      {
	fprintf(fd, "%seyedblib::int32 __tmp = _%s;\n", ctx->get(), name);
	if (odl_dynamic_attr)
	  fprintf(fd, "\n%sstatus = %s->setValue(this, (Data)&__tmp, 1, 0, _check_value);\n",
		  ctx->get(), __agrdyn, name);
	else
	  fprintf(fd, "\n%sstatus = %s[%d]->setValue(this, (Data)&__tmp, 1, 0, _check_value);\n",
		  ctx->get(), __agritems, num, name);
	if (attr_cache)
	  genAttrCacheSetEpilogue(ctx, optype);
	fprintf(fd, "%sreturn status;\n", ctx->get());
      }
    else
      {
	if (odl_dynamic_attr)
	  fprintf(fd, "\n%sstatus = %s->setValue(this, (Data)&_%s, 1, 0);\n",
		  ctx->get(), __agrdyn, name);
	else
	  fprintf(fd, "\n%sstatus = %s[%d]->setValue(this, (Data)&_%s, 1, 0);\n",
		  ctx->get(), __agritems, num, name);
	if (attr_cache)
	  genAttrCacheSetEpilogue(ctx, optype);
	fprintf(fd, "%sreturn status;\n", ctx->get());
      }

    fprintf(fd, "}\n\n");

    return Success;
  }


#define STATUS_ARG(HINTS, COMMA) \
(((HINTS).error_policy == GenCodeHints::StatusErrorPolicy) ? \
 ((COMMA) ? ", Status *rs" : "Status *rs") : "")

#define STATUS_PROTO(HINTS, COMMA) \
(((HINTS).error_policy == GenCodeHints::StatusErrorPolicy) ? \
 ((COMMA) ? ", Status * = 0" : "Status * = 0") : "")

  Status
  Attribute::generateCollGetMethod_C(Class *own, GenContext *ctx,
				     Bool isoid,
				     const GenCodeHints &hints,
				     const char *_const)
  {
    FILE *fd = ctx->getFile();
    int ndims = typmod.ndims, i;
    Bool _isref;
    eyedblib::int16 _dim;
    Class *cl = const_cast<Class *>(cls->asCollectionClass()->getCollClass(&_isref, &_dim));
    int not_basic = _isref || (!cl->asBasicClass() && !cl->asEnumClass());
    const char *ref = (not_basic ? "*" : "");
    const char *starg    = STATUS_ARG(hints, False);
    const char *stargcom = STATUS_ARG(hints, True);
    const char *classname = isIndirect() ?
      className(cls, True) : className(cls, False);
    const char *oclassname = _isref ?
      className(cl, True) : className(cl, False);
    const char *comma = (ndims ? ", " : "");

    Bool ordered = (cls->asCollArrayClass() || cls->asCollListClass()) ? True : False;
    if (isoid) {
      if (!_isref)
	return Success;

      if (ordered) { // 24/09/05
	fprintf(fd, "Oid %s::%s(unsigned int ind, ",
		own->getName(),
		ATTRNAME_1(name, (ordered ? GenCodeHints::RETRIEVEOID_ITEM_AT : GenCodeHints::GETOID_ITEM_AT), hints));
	dimArgsGen(fd, ndims, True);
	fprintf(fd, "%sStatus *rs) const\n", comma);
	fprintf(fd, "{\n");

	fprintf(fd, "%sOid tmp;;\n", ctx->get());
	/*
	  fprintf(fd, "%sStatus s, *ps;\n", ctx->get());
	  fprintf(fd, "%sps = (rs ? rs : &s);\n\n", ctx->get());
	*/
	fprintf(fd, "%sStatus s;\n", ctx->get());
      
	fprintf(fd, "%sconst Collection *coll = %s(", ctx->get(),
		ATTRNAME(name, GETCOLL, hints));

	for (i = 0; i < ndims; i++)
	  fprintf(fd, "a%d, ", i);

	fprintf(fd, "(Bool *)0, rs);\n\n", ctx->get());
	fprintf(fd, "%sif (!coll || (rs && *rs))\n", ctx->get());
	fprintf(fd, "%s  return tmp;\n\n", ctx->get());
      
	if (cls->asCollArrayClass() || cls->asCollListClass())
	  fprintf(fd, "%ss = coll->asCollArray()->retrieveAt(ind, tmp);\n", ctx->get());
	else
	  fprintf(fd, "%ss = coll->getOidAt(ind, tmp);\n", ctx->get());
	fprintf(fd, "%sif (s && rs) *rs = s;\n", ctx->get());
	fprintf(fd, "%sreturn tmp;\n", ctx->get());
	fprintf(fd, "}\n\n");
	return Success;
      }
    }

    if (!*_const && cl->asBasicClass())
      return Success;

    // 24/09/05
    if (!ordered)
      return Success;
      
    if (!strcmp(cl->getName(), char_class_name) && _dim > 1) {
      fprintf(fd, "const char *%s::%s(unsigned int ind, ",
	      own->getName(),
	      ATTRNAME_1(name, (ordered ? GenCodeHints::RETRIEVE_ITEM_AT : GenCodeHints::GET_ITEM_AT), hints));
    }
    else if (_dim == 1)
      fprintf(fd, "%s%s %s%s::%s(unsigned int ind, ",
	      _const, oclassname, ref, own->getName(),
	      ATTRNAME_1(name, (ordered ? GenCodeHints::RETRIEVE_ITEM_AT : GenCodeHints::GET_ITEM_AT), hints));
    else {
      //fprintf(stderr, "collection of dimensional item is not implemented: %s::%s\n", class_owner->getName(), name);
      return Success;
    }

    dimArgsGen(fd, ndims, True);
    fprintf(fd, "%sBool *isnull, Status *rs) %s\n", comma, _const);
    fprintf(fd, "{\n");

    /*
      fprintf(fd, "%sStatus s, *ps;\n", ctx->get());
      fprintf(fd, "%sps = (rs ? rs : &s);\n\n", ctx->get());
    */
    fprintf(fd, "%sStatus s;\n", ctx->get());

    fprintf(fd, "%sconst Collection *coll = %s(", ctx->get(),
	    ATTRNAME(name, GETCOLL, hints));

    for (i = 0; i < ndims; i++)
      fprintf(fd, "a%d, ", i);

    fprintf(fd, "isnull, rs);\n\n", ctx->get());
    fprintf(fd, "%sif (!coll || (rs && *rs))\n", ctx->get());
    if (not_basic)
      {
	fprintf(fd, "%s  return (%s *)0;\n\n", ctx->get(), oclassname);
	fprintf(fd, "%s%s *tmp = 0;\n", ctx->get(), oclassname);
	if (cls->asCollArrayClass() || cls->asCollListClass())
	  fprintf(fd, "%ss = coll->asCollArray()->retrieveAt(ind, (Object*&)tmp);\n", ctx->get());
	else
	  fprintf(fd, "%ss = coll->getObjectAt(ind, (Object*&)tmp);\n", ctx->get());
      }
    else
      {
	fprintf(fd, "%s  return 0;\n\n", ctx->get(), oclassname);
	fprintf(fd, "%sValue tmp;\n", ctx->get());
	if (cls->asCollArrayClass() || cls->asCollListClass())
	  fprintf(fd, "%ss = coll->asCollArray()->retrieveAt(ind, tmp);\n", ctx->get());
	else
	  fprintf(fd, "%ss = coll->getValueAt(ind, tmp);\n", ctx->get());
      }

    if (not_basic)
      {
	fprintf(fd, "%sif (s) {if (rs) *rs = s; return (%s *)0;}\n", ctx->get(), oclassname);
	fprintf(fd, "%sreturn tmp;\n", ctx->get());
      }
    else
      {
	fprintf(fd, "%sif (s) {if (rs) *rs = s; return 0;}\n", ctx->get());
	fprintf(fd, "%sreturn tmp.%s;\n", ctx->get(),
		Value::getAttributeName(cl, (_isref || (_dim > 1)) ?
					True : False));
      }

    fprintf(fd, "}\n\n");
    return Success;
  }

  Status
  Attribute::generateGetMethod_C(Class *own, GenContext *ctx,
				 Bool isoid,
				 const GenCodeHints &hints,
				 const char *_const)
  {
    FILE *fd = ctx->getFile();
    int ndims = typmod.ndims, i;
    int not_basic = NOT_BASIC();
    const char *ref = (not_basic ? "*" : "");
    const char *starg    = STATUS_ARG(hints, False);
    const char *stargcom = STATUS_ARG(hints, True);
    const char *classname = isIndirect() ?
      className(cls, True) : className(cls, False);

    if (isoid)
      fprintf(fd, "Oid %s::%s(", className(own),
	      ATTRNAME(name, GETOID, hints));
    else
      fprintf(fd, "%s%s %s%s::%s(", _const, classname, ref,
	      className(own), ATTRNAME_1(name, ATTRGET(cls), hints));

    dimArgsGen(fd, ndims, True);

    if (isoid)
      fprintf(fd, "%s) %s\n{\n", (ndims ? stargcom : starg), _const);
    else
      fprintf(fd, "%sBool *isnull%s) %s\n{\n", (ndims ? ", " : ""),
	      stargcom, (!not_basic ? "const" : _const));

    int const_obj = (!isoid && not_basic);

    GenCodeHints::OpType optype = (isoid ? GenCodeHints::GETOID:
				   GenCodeHints::GET);
    if (attr_cache)
      genAttrCacheGetPrologue(ctx, optype);

    if (odl_dynamic_attr)
      dynamic_attr_gen(fd, ctx, this, op_GET, isoid, False,
		       cls->asEnumClass() ?
		       className(cls, True) : 0);

    gbx_suspend(ctx);

    if (isoid)
      fprintf(fd, "%sOid __tmp;\n", ctx->get());
    else
      {
	if (const_obj)
	  fprintf(fd, "%sObject *__o = 0, *__go;\n", ctx->get());
	else if (cls->asEnumClass())
	  fprintf(fd, "%seyedblib::int32 %s__tmp%s;\n", ctx->get(),
		  ref,
		  (!strcmp(cls->getName(), "oid") ? "" : " = 0"));
	else
	  fprintf(fd, "%s%s %s__tmp%s;\n", ctx->get(),
		  classname, ref,
		  (!strcmp(cls->getName(), "oid") ? "" : " = 0"));
      }

    const char *psret = "";
    if (hints.error_policy == GenCodeHints::StatusErrorPolicy)
      {
	/*
	  fprintf(fd, "%sStatus s, *ps;\n", ctx->get());
	  fprintf(fd, "%sps = (rs ? rs : &s);\n", ctx->get());
	  psret = "*ps = ";
	*/
	fprintf(fd, "%sStatus s;\n", ctx->get());
	psret = "s = ";
      }

    if (ndims)
      {
	fprintf(fd, "%sSize from = a%d;\n", ctx->get(), ndims-1);
	for (int i = ndims - 2 ; i >= 0; i--)
	  fprintf(fd, "%sfrom += a%d * %d;\n", ctx->get(), i, typmod.dims[i]);

	if (odl_dynamic_attr) {
	  if (isoid)
	    fprintf(fd, "\n%s%s%s->getOid(this, &__tmp, 1, from);\n",
		    ctx->get(), psret, __agrdyn);
	  else
	    fprintf(fd, "\n%s%s%s->getValue(this, (Data *)&%s, 1, from, isnull);\n",
		    ctx->get(), psret, __agrdyn, (const_obj ? "__o" : "__tmp"));
	}
	else {
	  if (isoid)
	    fprintf(fd, "\n%s%s%s[%d]->getOid(this, &__tmp, 1, from);\n",
		    ctx->get(), psret, __agritems, num);
	  else
	    fprintf(fd, "\n%s%s%s[%d]->getValue(this, (Data *)&%s, 1, from, isnull);\n",
		    ctx->get(), psret, __agritems, num, (const_obj ? "__o" : "__tmp"));
	}
      }
    else if (isoid) {
      if (odl_dynamic_attr)
	fprintf(fd, "\n%s%s%s->getOid(this, &__tmp, 1, 0);\n",
		ctx->get(), psret, __agrdyn);
      else
	fprintf(fd, "\n%s%s%s[%d]->getOid(this, &__tmp, 1, 0);\n",
		ctx->get(), psret, __agritems, num);
    }
    else {
      if (odl_dynamic_attr)
	fprintf(fd, "\n%s%s%s->getValue(this, (Data *)&%s, 1, 0, isnull);\n",
		ctx->get(), psret, __agrdyn, (const_obj ? "__o" : "__tmp"));
      else
	fprintf(fd, "\n%s%s%s[%d]->getValue(this, (Data *)&%s, 1, 0, isnull);\n",
		ctx->get(), psret, __agritems, num, (const_obj ? "__o" : "__tmp"));
    }

    if (hints.error_policy == GenCodeHints::StatusErrorPolicy)
      {
	if (isoid || !strcmp(cls->getName(), "oid"))
	  fprintf(fd, "%sif (s) {if (rs) *rs = s; return nulloid;}\n\n",
		  ctx->get());
	else
	  RETURN_ERROR(fd, ctx, cls, "");
      }

    if (const_obj)
      {
	if (isIndirect())
	  {
	    fprintf(fd, "\n%sif (__o)\n", ctx->get());
	    fprintf(fd, "%s  {\n", ctx->get());
	    if (!cls->asCollectionClass()) // added the 23/08/99
	      {
		fprintf(fd, "   %sif (ObjectPeer::isGRTObject(__o)) {\n", ctx->get());
		if (attr_cache)
		  {
		    ctx->push();
		    ctx->push();
		    genAttrCacheGetEpilogue(ctx, optype);
		    ctx->pop();
		    ctx->pop();
		  }
	      
		fprintf(fd, "     %sreturn (%s *)__o;\n", ctx->get(),
			classname);
		fprintf(fd, "   %s}\n", ctx->get());
		fprintf(fd, "   %s__go = (%s *)make_object(__o, False);\n", ctx->get(), classname);
		fprintf(fd, "   %sif (__go)\n", ctx->get());
		fprintf(fd, "   %s {\n", ctx->get());
		ctx->push();
		fprintf(fd, "   %s__o = __go;\n", ctx->get());
		if (odl_dynamic_attr)
		  fprintf(fd, "   %s%s%s->setValue((Agregat *)this, (Data)&__o, 1, %s);\n",
			  ctx->get(), psret, __agrdyn, (ndims ? "from" : "0"));
		else
		  fprintf(fd, "   %s%s%s[%d]->setValue((Agregat *)this, (Data)&__o, 1, %s);\n",
			  ctx->get(), psret, __agritems, num, (ndims ? "from" : "0"));

		fprintf(fd, "   %sObjectPeer::decrRefCount(__o);\n", ctx->get());
	  
		if (hints.error_policy == GenCodeHints::StatusErrorPolicy)
		  RETURN_ERROR(fd, ctx, cls, "   ");
	      
		ctx->pop();
		fprintf(fd, "   %s }\n", ctx->get());

		if (attr_cache)
		  {
		    ctx->push();
		    genAttrCacheGetEpilogue(ctx, optype);
		    ctx->pop();
		  }
	      }

	    fprintf(fd, "   %sreturn (%s %s)__o;\n", ctx->get(),
		    classname, ref);
	    fprintf(fd, "%s  }\n\n", ctx->get());

	    fprintf(fd, "%sBool wasnull = (!__o ? True : False);\n", ctx->get());
	    fprintf(fd, "%sif (!__o && db)\n", ctx->get());
	    fprintf(fd, "%s  {\n", ctx->get());
	    fprintf(fd, "%s    Oid toid;\n", ctx->get());
	    if (odl_dynamic_attr)
	      fprintf(fd, "%s    %s%s->getOid(this, &toid, 1, %s);\n",
		      ctx->get(), psret, __agrdyn, (ndims ? "from" : "0"));
	    else
	      fprintf(fd, "%s    %s%s[%d]->getOid(this, &toid, 1, %s);\n",
		      ctx->get(), psret, __agritems, num, (ndims ? "from" : "0"));
	    if (hints.error_policy == GenCodeHints::StatusErrorPolicy)
	      RETURN_ERROR(fd, ctx, cls, "    ");

	    fprintf(fd, "%s    if (toid.isValid())\n", ctx->get());
	    fprintf(fd, "%s      {\n", ctx->get());
	    fprintf(fd, "%s        %sdb->loadObject(&toid, &__o);\n",
		    ctx->get(), psret);
	    if (hints.error_policy == GenCodeHints::StatusErrorPolicy)
	      RETURN_ERROR(fd, ctx, cls, "        ");

	    // ---- added the 4/05/99
	    if (!cls->asCollectionClass()) // added the 23/08/99
	      {
		fprintf(fd, "%s        if (!ObjectPeer::isGRTObject(__o))\n",
			ctx->get());
		fprintf(fd, "%s         {\n", ctx->get());
		fprintf(fd, "%s           __go = (%s *)make_object"
			"(__o, False);\n", ctx->get(), classname);
		fprintf(fd, "%s           if (__go) __o = __go;\n", ctx->get());
		fprintf(fd, "%s         }\n", ctx->get());
	      }
	    // ----
	    fprintf(fd, "%s      }\n", ctx->get());
	    fprintf(fd, "%s  }\n", ctx->get());
	  
	    fprintf(fd, "\n%sif (__o && wasnull)\n", ctx->get());
	    fprintf(fd, "%s  {\n", ctx->get());
	    if (odl_dynamic_attr)
	      fprintf(fd, "   %s%s%s->setValue((Agregat *)this, (Data)&__o, 1, %s);\n",
		      ctx->get(), psret, __agrdyn, (ndims ? "from" : "0"));
	    else
	      fprintf(fd, "   %s%s%s[%d]->setValue((Agregat *)this, (Data)&__o, 1, %s);\n",
		      ctx->get(), psret, __agritems, num, (ndims ? "from" : "0"));
	    if (hints.error_policy == GenCodeHints::StatusErrorPolicy)
	      RETURN_ERROR(fd, ctx, cls, "   ");

	    // change the 23/08/99
	    fprintf(fd, "   %s__o->release();\n", ctx->get());
	    fprintf(fd, "%s  }\n", ctx->get());
	  }
	else if (!cls->asCollectionClass()) // added the 25/11/99
	  {
	    fprintf(fd, "\n%sif (__o)\n", ctx->get());
	    fprintf(fd, "%s  {\n", ctx->get());
	    fprintf(fd, "   %sif (ObjectPeer::isGRTObject(__o)) {\n", ctx->get());
	    if (attr_cache)
	      {
		ctx->push();
		ctx->push();
		genAttrCacheGetEpilogue(ctx, optype);
		ctx->pop();
		ctx->pop();
	      }

	    fprintf(fd, "     %sreturn (%s *)__o;\n", ctx->get(),
		    classname);
	    fprintf(fd, "   %s}\n", ctx->get());
	    fprintf(fd, "   %s__go = (%s *)make_object(__o, False);\n", ctx->get(), classname);
	    fprintf(fd, "   %sif (__go)\n", ctx->get());
	    fprintf(fd, "   %s {\n", ctx->get());
	    ctx->push();
	    fprintf(fd, "   %s__o = __go;;\n", ctx->get());
	    if (odl_dynamic_attr)
	      fprintf(fd, "   %s%s%s->setValue((Agregat *)this, (Data)&__o, 1, %s);\n",
		      ctx->get(), psret, __agrdyn, (ndims ? "from" : "0"));
	    else
	      fprintf(fd, "   %s%s%s[%d]->setValue((Agregat *)this, (Data)&__o, 1, %s);\n",
		      ctx->get(), psret, __agritems, num, (ndims ? "from" : "0"));
	    fprintf(fd, "   %sObjectPeer::decrRefCount(__o);\n", ctx->get());
	  
	    if (hints.error_policy == GenCodeHints::StatusErrorPolicy)
	      RETURN_ERROR(fd, ctx, cls, "   ");

	    ctx->pop();
	    fprintf(fd, "   %s }\n", ctx->get());
	    fprintf(fd, "%s  }\n", ctx->get());
	  }
      }

    if (attr_cache)
      genAttrCacheGetEpilogue(ctx, optype);

    if (const_obj)
      fprintf(fd, "%sreturn (%s %s)__o;\n", ctx->get(),
	      classname, ref);
    else if (cls->asEnumClass())
      fprintf(fd, "%sreturn (%s)__tmp;\n", ctx->get(), className(cls, True));
    else
      fprintf(fd, "%sreturn __tmp;\n", ctx->get());

    fprintf(fd, "}\n\n");

    // WARNING: changed '*_const || IS_RAW()' to
    // '!isoid && !IS_STRING()' the 11/1/99
    if (isVarDim() && !isoid && !*_const && !IS_STRING())
      {
	fprintf(fd, "unsigned int %s::%s(%s) const\n{\n", className(own),
		ATTRNAME(name, GETCOUNT, hints), starg);

	if (attr_cache)
	  genAttrCacheGetPrologue(ctx, GenCodeHints::GETCOUNT);

	if (odl_dynamic_attr)
	  dynamic_attr_gen(fd, ctx, this, op_OTHER, isoid);

	gbx_suspend(ctx);

	fprintf(fd, "%sSize size;\n", ctx->get());
	if (hints.error_policy == GenCodeHints::StatusErrorPolicy)
	  {
	    /*
	      fprintf(fd, "%sStatus s, *ps;\n", ctx->get());
	      fprintf(fd, "%sps = (rs ? rs : &s);\n", ctx->get());
	    */
	    fprintf(fd, "%sStatus s;\n", ctx->get());
	  }
	if (odl_dynamic_attr)
	  fprintf(fd, "%s%s%s->getSize(this, size);\n", ctx->get(), psret,
		  __agrdyn);
	else
	  fprintf(fd, "%s%s%s[%d]->getSize(this, size);\n", ctx->get(), psret,
		  __agritems, num);
	if (hints.error_policy == GenCodeHints::StatusErrorPolicy)
	  fprintf(fd, "%sif (s) {if (rs) *rs = s; return 0;}\n", ctx->get());

	if (attr_cache)
	  genAttrCacheGetEpilogue(ctx, GenCodeHints::GETCOUNT);

	fprintf(fd, "%sreturn (int)size;\n", ctx->get());
	fprintf(fd, "}\n\n");
      }

    if (cls->asCollectionClass())
      return generateCollGetMethod_C(own, ctx, isoid,
				     hints, _const);
    return Success;
  }
  
  Status Attribute::generateBody_C(Class *own,
				   GenContext *ctx,
				   const GenCodeHints &hints)
  {
    FILE *fd = ctx->getFile();
    int ndims = typmod.ndims, i;
    int maxdims = typmod.maxdims;
    int not_basic = NOT_BASIC();
    const char *ref = (not_basic ? "*" : "");
    int is_string = IS_STRING();
    int is_raw = IS_RAW();
    const char *starg    = STATUS_ARG(hints, False);
    const char *stargcom = STATUS_ARG(hints, True);
    const char *psret = "";

    // set methods
    if (is_string || is_raw)
      {
	const char *sclass = SCLASS();
	const char *sarg = SARGIN();
	fprintf(fd, "Status %s::%s(const %s *_%s%s)\n{\n",
		className(own), ATTRNAME(name, SET, hints), sclass, name,
		sarg);

	if (attr_cache)
	  genAttrCacheSetPrologue(ctx, GenCodeHints::SET, True);

	if (odl_dynamic_attr)
	  dynamic_attr_gen(fd, ctx, this, op_SET);

	gbx_suspend(ctx);
	fprintf(fd, "%sStatus status;\n", ctx->get());
      
	if (isVarDim())
	  {
	    fprintf(fd, "%sSize size;\n", ctx->get());
	    if (is_string)
	      fprintf(fd, "%sSize len = ::strlen(_%s) + 1;\n\n", ctx->get(), name);
	    if (odl_dynamic_attr)
	      fprintf(fd, "%sstatus = %s->getSize(this, size);\n", ctx->get(),
		      __agrdyn);
	    else
	      fprintf(fd, "%sstatus = %s[%d]->getSize(this, size);\n", ctx->get(),
		      __agritems, num);
	    fprintf(fd, "%sif (status)\n%s  return status;\n\n", ctx->get(),
		    ctx->get());
	    fprintf(fd, "%sif (size != len)\n", ctx->get());
	    ctx->push();
	    if (odl_dynamic_attr)
	      fprintf(fd, "%sstatus = %s->setSize(this, len);\n", ctx->get(),
		      __agrdyn);
	    else
	      fprintf(fd, "%sstatus = %s[%d]->setSize(this, len);\n", ctx->get(),
		      __agritems, num);
	    ctx->pop();
	    fprintf(fd, "%sif (status)\n%s  return status;\n\n", ctx->get(),
		    ctx->get());
	    if (odl_dynamic_attr)
	      fprintf(fd, "%sstatus = %s->setValue(this, (Data)_%s, len, 0);\n",
		      ctx->get(), __agrdyn, name);
	    else
	      fprintf(fd, "%sstatus = %s[%d]->setValue(this, (Data)_%s, len, 0);\n",
		      ctx->get(), __agritems, num, name);

	    if (attr_cache)
	      genAttrCacheSetEpilogue(ctx, GenCodeHints::SET, True);
	    fprintf(fd, "%sreturn status;\n\n", ctx->get());
	  }
	else
	  {
	    if (is_string)
	      {
		fprintf(fd, "%sunsigned char data[%d];\n", ctx->get(), maxdims);
		fprintf(fd, "%sSize len = ::strlen(_%s);\n", ctx->get(), name);
	      }
	    fprintf(fd, "%sif (len >= %d)\n", ctx->get(), maxdims);
	    fprintf(fd, "%s  return Exception::make(IDB_ERROR, "
		    "\"string `%%s' [%%d] too long for attribute %s::%s, maximum "
		    "is %d\\n\", _%s, len);\n", ctx->get(),
		    class_owner->getName(), name, maxdims, name);
	    if (is_string)
	      {
		fprintf(fd, "%smemset(data, 0, %d);\n", ctx->get(), maxdims);
		fprintf(fd, "%sstrncpy((char *)data, _%s, min(%d, len));\n",
			ctx->get(), name, maxdims-1);
		if (odl_dynamic_attr)
		  fprintf(fd, "%sstatus = %s->setValue(this, data, %d, 0);\n",
			  ctx->get(), __agrdyn, maxdims);
		else
		  fprintf(fd, "%sstatus = %s[%d]->setValue(this, data, %d, 0);\n",
			  ctx->get(), __agritems, num, maxdims);
	      }
	    else {
	      if (odl_dynamic_attr)
		fprintf(fd, "%sstatus = %s->setValue(this, (Data)_%s, len, 0);\n",
			ctx->get(), __agrdyn, name);
	      else
		fprintf(fd, "%sstatus = %s[%d]->setValue(this, (Data)_%s, len, 0);\n",
			ctx->get(), __agritems, num, name);
	    }

	    if (attr_cache)
	      genAttrCacheSetEpilogue(ctx, GenCodeHints::SET, True);
	    fprintf(fd, "%sreturn status;\n\n", ctx->get());
	  }
	fprintf(fd, "}\n\n");
      }

    /* warning:
     * the methods setXxx() should be avoided when xxx is a non basic
     * and direct, because the direct Object.has already been allocated
     * in the object construction.
     * The problem is for the vardim: the setXxx(int w, Xxx *x) method sets
     * the size and sets the `x' object.
     * So if we suppress the setXxx() method, one should had a method for
     * vardim: setXxx_cnt() or setXxx_size to set the size.
     */

#ifdef NO_DIRECT_SET
    if (isIndirect() || !not_basic) // added 22/09/98
#endif
      generateSetMethod_C(own, ctx, False, hints);
#ifdef NO_DIRECT_SET
    else
#endif
#if defined(SET_COUNT_DIRECT) || defined(NO_DIRECT_SET)
      if (isVarDim() && !isIndirect() && !is_string) /*&& not_basic*/
	{
	  fprintf(fd, "Status %s::%s(", className(own),
		  ATTRNAME(name, SETCOUNT, hints));
      
	  dimArgsGen(fd, ndims, True);
      
	  fprintf(fd, ")\n{\n");

	  if (attr_cache)
	    genAttrCacheSetPrologue(ctx, GenCodeHints::SETCOUNT);

	  if (odl_dynamic_attr)
	    dynamic_attr_gen(fd, ctx, this, op_SET);

	  gbx_suspend(ctx);

	  fprintf(fd, "%sStatus status;\n", ctx->get());

	  fprintf(fd, "%sSize from = a%d;\n", ctx->get(), ndims-1);
	  for (int i = ndims - 2 ; i >= 0; i--)
	    fprintf(fd, "%sfrom += a%d * %d;\n", ctx->get(), i, typmod.dims[i]);

	  // disconnected the 3/09/01: it is not useful to forbid to
	  // decrement count!
#if 0
	  fprintf(fd, "\n%sSize size;\n", ctx->get());
	  if (odl_dynamic_attr)
	    fprintf(fd, "%sstatus = %s->getSize(this, size);\n",
		    ctx->get(), __agrdyn);
	  else
	    fprintf(fd, "%sstatus = %s[%d]->getSize(this, size);\n",
		    ctx->get(), __agritems, num);
	  fprintf(fd, "%sif (status)\n%s  return status;\n\n", ctx->get(),
		  ctx->get());
	  // changed the 24/08/99
	  fprintf(fd, "%sif (size < from)\n", ctx->get());
	  ctx->push();
#endif
	  if (odl_dynamic_attr)
	    fprintf(fd, "%sstatus = %s->setSize(this, from);\n",
		    ctx->get(), __agrdyn);
	  else
	    fprintf(fd, "%sstatus = %s[%d]->setSize(this, from);\n",
		    ctx->get(), __agritems, num);

#if 0
	  ctx->pop();
#endif

	  if (attr_cache)
	    genAttrCacheSetEpilogue(ctx, GenCodeHints::SETCOUNT);

	  fprintf(fd, "%sreturn status;\n", ctx->get(),
		  ctx->get());

	  fprintf(fd, "}\n\n");
	}
#endif

    // get methods
    if (is_string || is_raw)
      {
	const char *sclass = SCLASS();
	const char *sarg = SARGOUT();
	fprintf(fd, "const %s *%s::%s(%sBool *isnull%s) const\n{\n", 
		sclass, className(own), ATTRNAME(name, GET, hints), sarg,
		stargcom);

	if (attr_cache)
	  genAttrCacheGetPrologue(ctx, GenCodeHints::GET, True);

	if (odl_dynamic_attr)
	  dynamic_attr_gen(fd, ctx, this, op_GET, False, IDBBOOL(is_string));

	gbx_suspend(ctx);

	fprintf(fd, "%sData data;\n", ctx->get());
	if (hints.error_policy == GenCodeHints::StatusErrorPolicy)
	  {
	    /*
	      fprintf(fd, "%sStatus s, *ps;\n", ctx->get());
	      fprintf(fd, "%sps = (rs ? rs : &s);\n", ctx->get());
	      psret = "*ps = ";
	    */
	    fprintf(fd, "%sStatus s;\n", ctx->get());
	    psret = "s = ";
	  }
	if (odl_dynamic_attr)
	  fprintf(fd, "\n%s%s%s->getValue(this, (Data *)&data, Attribute::directAccess, 0, isnull);\n",
		  ctx->get(), psret, __agrdyn);
	else
	  fprintf(fd, "\n%s%s%s[%d]->getValue(this, (Data *)&data, Attribute::directAccess, 0, isnull);\n",
		  ctx->get(), psret, __agritems, num);

	if (hints.error_policy == GenCodeHints::StatusErrorPolicy)
	  RETURN_ERROR(fd, ctx, cls, "");

	if (isVarDim())
	  fprintf(fd, "%sif (!data) data = nulldata;\n", ctx->get());

	if (attr_cache)
	  genAttrCacheGetEpilogue(ctx, GenCodeHints::GET, True);

	if (is_raw && isVarDim())
	  fprintf(fd, "%sif (len) *len = %s();\n", ctx->get(),
		  ATTRNAME(name, GETCOUNT, hints));
	//fprintf(fd, "%sif (s && rs) *rs = s;\n", ctx->get());
	if (attr_cache)
	  fprintf(fd, "%sreturn (const %s *)%s;\n", ctx->get(), sclass,
		  atc_name(name));
	else
	  fprintf(fd, "%sreturn (const %s *)data;\n", ctx->get(), sclass);
	fprintf(fd, "}\n\n");
      }
    else if (not_basic)
      generateGetMethod_C(own, ctx, False, hints, "const ");

    generateGetMethod_C(own, ctx, False, hints, "");

    if (isIndirect())
      {
	generateSetMethod_C(own, ctx, True, hints);
	generateGetMethod_C(own, ctx, True, hints, "");
      }

    generateSetMethod_C(own, ctx, hints);

    return Success;
  }

  //
  // Attribute Cache Methods
  //
  // on peut se limiter dans un premier temps à:
  //
  // - type de dimensions fixes:
  //   + get/set
  //   + getoid/setoid
  //
  // - type string:
  //   + get/set
  //
  // - type de dimension variable:
  //   + getcount/setcount

#define ATC_NOVD // acronym for ATtribute Cache NO VarDim

  void
  Attribute::genAttrCacheDecl(GenContext *ctx)
  {
    if (isNative()) return;

#ifdef ATC_NOVD
    //  if (isVarDim() && !IS_STRING()) return;
#endif
    FILE *fd = ctx->getFile();

    int is_string = IS_STRING();
    int is_raw = IS_RAW();
    int i;

    if (isVarDim())
      fprintf(fd, "%sunsigned int %s;\n", ctx->get(), atc_cnt(name));
#ifdef ATC_NOVD
    if (isVarDim() && !(is_string || is_raw))
      return;
#endif

    fprintf(fd, "%sunsigned char ", ctx->get());

#ifndef ATC_NOVD
    if (isVarDim())
      fprintf(fd, "*");
#endif

    fprintf(fd, atc_set(name));
    if (!is_string && !is_raw)
      {
	for (i = 0; i < typmod.ndims; i++)
	  if (typmod.dims[i] > 0)
	    fprintf(fd, "[%d]", typmod.dims[i]);
      }
    fprintf(fd, ";\n");

    if (is_string || is_raw)
      {
#ifdef ATTR_CACHE_DIRECT
	fprintf(fd, "%sconst %s *%s;\n", ctx->get(), SCLASS(), atc_name(name));
#else
	fprintf(fd, "%s%s *%s;\n", ctx->get(), SCLASS(), atc_name(name));
#endif
	return;
      }
    else
      fprintf(fd, "%s%s %s%s%s",
	      ctx->get(), className(cls, (isIndirect() ? True : False)),
	      (isVarDim() ? "*" : ""), (NOT_BASIC() ? "*" : ""), atc_name(name));

    for (i = 0; i < typmod.ndims; i++)
      if (typmod.dims[i] > 0)
	fprintf(fd, "[%d]", typmod.dims[i]);

    fprintf(fd, ";\n");
  }

  void
  Attribute::genAttrCacheEmpty(GenContext *ctx)
  {
    if (isNative()) return;

#ifdef ATC_NOVD
    //  if (isVarDim() && !IS_STRING()) return;
#endif

    int is_string = IS_STRING();
    int is_raw = IS_RAW();

    FILE *fd = ctx->getFile();
    if (isVarDim())
      fprintf(fd, "%s%s = ~0;\n", ctx->get(), atc_cnt(name));

#ifdef ATC_NOVD
    if (isVarDim() && !(is_string || is_raw)) return;
#endif

    if (!typmod.ndims || is_string || is_raw)
      fprintf(fd, "%s%s = 0;\n", ctx->get(), atc_set(name));
    else
      fprintf(fd, "%smemset(%s, 0, %d);\n", ctx->get(), atc_set(name),
	      typmod.pdims);
  }

  void
  Attribute::genAttrCacheGarbage(GenContext *ctx)
  {
    if (isNative()) return;
#ifdef ATC_NOVD
    if (isVarDim() && !(IS_STRING() || IS_RAW())) return;
#endif

    //  if (isNative() || !isVarDim()) return;
    FILE *fd = ctx->getFile();

    if (IS_STRING() || IS_RAW())
      {
#ifndef ATTR_CACHE_DIRECT
	fprintf(fd, "%sif (%s) free(%s);\n", ctx->get(), atc_set(name),
		atc_name(name));
#endif
	return;
      }

    if (!isVarDim()) return;

    // TODO: NOTE: si les precedents malloc() sont bons pour les vardim, alors
    // les free qui suivent ne sont pas corrects; en effet, il faut
    // liberer tous les pointeurs d'un tableau. -> il me semble qu'il y a
    // un peu de confusion!

    fprintf(fd, "%sif (%s) {free(%s); free(%s);}\n", ctx->get(), atc_cnt(name),
	    atc_set(name), atc_name(name));
  }

  void
  Attribute::genAttrCacheGetPrologue(GenContext *ctx, int optype,
				     Bool is_raw_string)
  {
#ifdef ATC_NOVD
    if (optype != GenCodeHints::GETCOUNT)
      {
	if (isVarDim() && !is_raw_string)
	  return;
	if ((IS_STRING() || IS_RAW()) && !is_raw_string)
	  return;
      }
#endif

    if (optype == GenCodeHints::GETOID && !isIndirect()) return;

    if (optype == GenCodeHints::GETOID) return;

    FILE *fd = ctx->getFile();

    if (optype == GenCodeHints::GETCOUNT)
      {
	fprintf(fd, "%sif (%s != (unsigned int)~0) return %s;\n", ctx->get(), atc_cnt(name),
		atc_cnt(name));
	return;
      }

    // TODO: WARNING: must check dimensions before returning from cache!

    if (is_raw_string)
      {
	if (IS_RAW() && isVarDim())
	  {
	    fprintf(fd, "%sif (%s) {if (len) *len = %s; return %s;}\n",
		    ctx->get(), atc_set(name), atc_cnt(name), atc_name(name));
	  }
	else
	  fprintf(fd, "%sif (%s) return %s;\n", ctx->get(), atc_set(name),
		  atc_name(name));
	return;
      }

    if (isVarDim())
      fprintf(fd, "%sif (%s > a%d && %s",
	      ctx->get(), atc_cnt(name), typmod.ndims-1, atc_set(name));
    else
      fprintf(fd, "%sif (%s", ctx->get(), atc_set(name));

    int i;
    for (i = 0; i < typmod.ndims; i++)
      fprintf(fd, "[a%d]", i);
    fprintf(fd, ") return %s", atc_name(name));

    for (i = 0; i < typmod.ndims; i++)
      fprintf(fd, "[a%d]", i);
    fprintf(fd, ";\n\n");
  }

  void
  Attribute::genAttrCacheGetEpilogue(GenContext *ctx, int optype,
				     Bool is_raw_string)
  {
#ifdef ATC_NOVD
    if (optype != GenCodeHints::GETCOUNT)
      {
	if (isVarDim() && !is_raw_string)
	  return;
	if ((IS_STRING() || IS_RAW()) && !is_raw_string)
	  return;
      }
#endif

    if (optype == GenCodeHints::GETOID && !isIndirect()) return;

    if (optype == GenCodeHints::GETOID) return;

    FILE *fd = ctx->getFile();

    const char *classname = className(class_owner);

    if (optype == GenCodeHints::GETCOUNT)
      {
	fprintf(fd, "%s%s->%s = size;\n", ctx->get(), atc_this(classname),
		atc_cnt(name));
	return;
      }

    if (is_raw_string)
      {
	fprintf(fd, "%s%s->%s = 1;\n", ctx->get(), atc_this(classname),
		atc_set(name));
	// TODO: faux dans le cas d'un raw data.
	if (IS_RAW())
	  {
#ifdef ATTR_CACHE_DIRECT
	    if (isVarDim())
	      {
		const char *scnt = ATTRNAME(name, GETCOUNT, *phints);
		fprintf(fd, "%s%s->%s = %s();\n", ctx->get(),
			atc_this(classname), atc_cnt(name), scnt);
	      }

	    fprintf(fd, "%s%s->%s = data;\n", ctx->get(),
		    atc_this(classname), atc_name(name));
#else
	    if (isVarDim())
	      {
		const char *scnt = ATTRNAME(name, GETCOUNT, *phints);
		fprintf(fd, "%s%s->%s = rawdup(data, %s());\n", ctx->get(),
			atc_this(classname), atc_name(name), scnt);
		fprintf(fd, "%s%s->%s = %s();\n", ctx->get(),
			atc_this(classname), atc_cnt(name), scnt);

	      }
	    else
	      fprintf(fd, "%s%s->%s = rawdup(data, %d);\n", ctx->get(),
		      atc_this(classname), atc_name(name), typmod.pdims);
#endif
	  }
#ifdef ATTR_CACHE_DIRECT
	else
	  fprintf(fd, "%s%s->%s = (const char *)data;\n", ctx->get(),
		  atc_this(classname), atc_name(name));
#else
	else
	  fprintf(fd, "%s%s->%s = strdup((const char *)data);\n", ctx->get(),
		  atc_this(classname), atc_name(name));
#endif
	return;
      }

    if (isVarDim())
      {
	// TODO: FAUX: on doit plutot faire un realloc dans le cas ou
	// le _cnt n'etait pas null!
	fprintf(fd, "%sif (%s) {free(%s->%s); free(%s->%s);}\n",
		ctx->get(), atc_cnt(name), atc_this(classname), atc_set(name),
		atc_this(classname), atc_name(name));
	fprintf(fd, "%s%s->%s = a%d+1;\n", ctx->get(), atc_this(classname), atc_cnt(name),
		typmod.ndims-1);
	fprintf(fd, "%s%s->%s",
		ctx->get(), atc_this(classname), atc_set(name));
	int i;
	for (i = 0; i < typmod.ndims; i++)
	  if (typmod.dims[i] > 0) fprintf(fd, "[a%d]", i);

	fprintf(fd, " = (unsigned char *)malloc(a%d+1);\n", typmod.ndims-1);

	fprintf(fd, "%s%s->%s",
		ctx->get(), atc_this(classname), atc_name(name));
	for (i = 0; i < typmod.ndims; i++)
	  if (typmod.dims[i] > 0) fprintf(fd, "[a%d]", i);
	fprintf(fd, " = (%s *%s)malloc((a%d+1)*sizeof(%s *));\n",
		className(cls, (isIndirect() ? True : False)),
		(NOT_BASIC() ? "*" : ""),
		typmod.ndims-1, className(cls, True));
      }

    fprintf(fd, "%s%s->%s", ctx->get(), atc_this(classname), atc_set(name));
    int i;
    for (i = 0; i < typmod.ndims; i++)
      fprintf(fd, "[a%d]", i);

    fprintf(fd, " = 1;\n");
    fprintf(fd, "%s%s->%s", ctx->get(), atc_this(classname), atc_name(name));
    for (i = 0; i < typmod.ndims; i++)
      fprintf(fd, "[a%d]", i);

    fprintf(fd, " = ");

    if (NOT_BASIC())
      fprintf(fd, "(%s *)__o", className(cls, True));
    else
      fprintf(fd, "(%s)__tmp", className(cls, False));
    fprintf(fd, ";\n");
  }

  void
  Attribute::genAttrCacheSetPrologue(GenContext *ctx, int optype,
				     Bool is_raw_string)
  {
    // nothing to do!
  }

  void
  Attribute::genAttrCacheSetEpilogue(GenContext *ctx, int optype,
				     Bool is_raw_string)
  {
#ifdef ATC_NOVD
    if (optype != GenCodeHints::SETCOUNT)
      {
	if (isVarDim() && !is_raw_string)
	  return;
	if ((IS_STRING() || IS_RAW()) && !is_raw_string)
	  return;
      }
#endif
    if (optype == GenCodeHints::SETOID && !isIndirect()) return;

    if (optype == GenCodeHints::SETOID) return;

    FILE *fd = ctx->getFile();

    if (optype == GenCodeHints::SETCOUNT)
      {
	fprintf(fd, "%sif (!status) %s = size;\n", ctx->get(), atc_cnt(name));
	return;
      }

    const char *classname = className(class_owner);

    fprintf(fd, "%sif (!status) {\n", ctx->get());
    ctx->push();

    if (is_raw_string)
      {
	if (IS_RAW() && !isVarDim())
	  {
	    fprintf(fd, "%sif (len == %d) {\n", ctx->get(), typmod.pdims);
	    ctx->push();
	  }

#ifndef ATTR_CACHE_DIRECT
	fprintf(fd, "%sif (%s) free(%s->%s);\n", ctx->get(), atc_set(name),
		atc_this(classname), atc_name(name));
#endif
	fprintf(fd, "%s%s->%s = 1;\n", ctx->get(), atc_this(classname),
		atc_set(name));

	if (IS_RAW())
	  {
#ifdef ATTR_CACHE_DIRECT
	    fprintf(fd, "%s%s->%s = %s(%s);\n", ctx->get(),
		    atc_this(classname), atc_name(name),
		    ATTRNAME(name, GET, *phints),
		    (isVarDim() ? "(unsigned int *)0" : ""));
#else
	    fprintf(fd, "%s%s->%s = rawdup(_%s, len);\n", ctx->get(),
		    atc_this(classname), atc_name(name), name);
#endif
	    if (isVarDim())
	      fprintf(fd, "%s%s->%s = len;\n", ctx->get(),
		      atc_this(classname), atc_cnt(name));
	    else
	      {
		ctx->pop();
		fprintf(fd, "%s}\n", ctx->get());
	      }
	  }
	else
	  {
#ifdef ATTR_CACHE_DIRECT
	    fprintf(fd, "%s%s->%s = %s();\n", ctx->get(),
		    atc_this(classname), atc_name(name),
		    ATTRNAME(name, GET, *phints));
#else
	    if (isVarDim())
	      fprintf(fd, "%s%s->%s = strdup((const char *)_%s);\n", ctx->get(),
		      atc_this(classname), atc_name(name), name);
	    else
	      fprintf(fd, "%s%s->%s = strdup((const char *)data);\n", ctx->get(),
		      atc_this(classname), atc_name(name));
#endif
	  }
	ctx->pop();
	fprintf(fd, "%s}\n", ctx->get());
	return;
      }

    fprintf(fd, "%s%s->%s", ctx->get(), atc_this(classname), atc_set(name));
    int i;
    for (i = 0; i < typmod.ndims; i++)
      fprintf(fd, "[a%d]", i);

    fprintf(fd, " = 1;\n");
    fprintf(fd, "%s%s->%s", ctx->get(), atc_this(classname), atc_name(name));
    for (i = 0; i < typmod.ndims; i++)
      fprintf(fd, "[a%d]", i);

    fprintf(fd, " = ");

    /*
      if (NOT_BASIC())
      fprintf(fd, "(%s *)__o", className(cls, True));
      else
      fprintf(fd, "(%s)__tmp", className(cls, False));
    */

    if (NOT_BASIC())
      fprintf(fd, "(%s *)_%s", className(cls, True), name);
    else
      fprintf(fd, "(%s)_%s", className(cls, False), name);

    fprintf(fd, ";\n");
    ctx->pop();
    fprintf(fd, "%s}\n", ctx->get());
  }

  Status Attribute::generateCode_C(Class *own,
				   const GenCodeHints &hints,
				   GenContext *ctxH,
				   GenContext *ctxC)
  {
    FILE *fdh = ctxH->getFile();
    int ndims = typmod.ndims, i;
    int not_basic = NOT_BASIC();
    const char *ref1 = (not_basic ? "*" : "");
    const char *ref2 = (not_basic ? " *" : " ");
    const char *comma = (ndims ? ", " : "");
    int is_string = IS_STRING();
    int is_raw = IS_RAW();
    const char *starg    = STATUS_PROTO(hints, False);
    const char *stargcom = STATUS_PROTO(hints, True);
    const char *classname = isIndirect() ?
      className(cls, True) : className(cls, False);

    // set method
    fprintf(fdh, "\n");
    if (is_string || is_raw)
      fprintf(fdh, "%sStatus %s(const %s *%s);\n", ctxH->get(),
	      ATTRNAME(name, SET, hints), SCLASS(), SARGIN());

#ifdef NO_DIRECT_SET
    if (isIndirect() || !not_basic) // added 22/09/98
#endif
      {
	fprintf(fdh, "%sStatus %s(", ctxH->get(),
		ATTRNAME_1(name, ATTRSET(cls), hints));
	dimArgsGen(fdh, ndims);
	if (cls->asEnumClass())
	  fprintf(fdh, "%s%s%s, Bool _check_value = True);\n",
		  comma, classname, ref1);
	else
	  fprintf(fdh, "%s%s%s);\n", comma, classname, ref1);
      }
#ifdef NO_DIRECT_SET
    else
#endif
#if defined(SET_COUNT_DIRECT) || defined(NO_DIRECT_SET)
      if (isVarDim() && !isIndirect() && !is_string) /*&& not_basic*/
	{
	  fprintf(fdh, "%sStatus %s(",
		  ctxH->get(), ATTRNAME(name, SETCOUNT, hints));

	  dimArgsGen(fdh, ndims);
	  fprintf(fdh, ");\n");
	}
#endif

    // get methods
    if (is_string || is_raw)
      fprintf(fdh, "%sconst %s *%s(%sBool *isnull = 0%s) const;\n",
	      ctxH->get(), SCLASS(), ATTRNAME(name, GET, hints), SARGOUT(),
	      stargcom);

    fprintf(fdh, "%s%s%s%s(", ctxH->get(), classname, ref2,
	    ATTRNAME_1(name, ATTRGET(cls), hints));
    dimArgsGen(fdh, ndims);
    fprintf(fdh, "%sBool *isnull = 0%s) %s;\n", comma,
	    stargcom, (!not_basic ? " const" : ""));

    if (cls->asCollectionClass()) {
      fprintf(fdh, "%sunsigned int %s(", ctxH->get(),
	      ATTRNAME(name, GETCOUNT, hints));
      dimArgsGen(fdh, ndims);
      fprintf(fdh, "%sBool *isnull = 0, Status *rs = 0) const "
	      "{const Collection *_coll = %s(", comma, ATTRNAME(name, GETCOLL, hints));
      for (i = 0; i < ndims; i++)
	fprintf(fdh, "a%d, ", i);
      fprintf(fdh, "isnull, rs); ");
      fprintf(fdh, "return (_coll ? _coll->getCount() : 0);}\n");
    }

    if (not_basic) {
      fprintf(fdh, "%sconst %s%s%s(", ctxH->get(), classname, ref2,
	      ATTRNAME_1(name, ATTRGET(cls), hints));
  
      dimArgsGen(fdh, ndims);
      fprintf(fdh, "%sBool *isnull = 0%s) const;\n", comma,
	      stargcom);
    }

    if (isIndirect()) {
      fprintf(fdh, "%sOid %s(",
	      ctxH->get(), ATTRNAME(name, GETOID, hints));
      dimArgsGen(fdh, ndims);
      fprintf(fdh, "%s);\n", (ndims ? stargcom : starg));
      fprintf(fdh, "%sStatus %s(",
	      ctxH->get(), ATTRNAME(name, SETOID, hints));
      dimArgsGen(fdh, ndims);
      fprintf(fdh, "%sconst Oid &);\n", comma);
    }

    if (cls->asCollectionClass()) {
      Bool _isref;
      eyedblib::int16 _dim;
      Class *cl = const_cast<Class *>
	(cls->asCollectionClass()->getCollClass(&_isref, &_dim));
      const char *oclassname = _isref ?
	className(cl, True) : className(cl, False);
      Bool ordered;

      if (cls->asCollSetClass() || cls->asCollBagClass())
	ordered = False;
      else
	ordered = True;

      const char *where = (ordered ? "int where, " : "");

      if (_dim == 1) {
	fprintf(fdh, "%sStatus %s(%s", ctxH->get(),
		ATTRNAME_1(name, (ordered ? GenCodeHints::SET_ITEM_IN_COLL : GenCodeHints::ADD_ITEM_TO_COLL), hints), where);
	dimArgsGen(fdh, ndims);
	fprintf(fdh, "%s%s%s%s, const IndexImpl * = 0);\n", comma,
		oclassname, (_isref || !cl->asBasicClass() ? "*" : ""),
		(!*where ? ", Bool noDup = False" : ""));

	if (ordered) {
	  fprintf(fdh, "%sStatus %s(int where%s", ctxH->get(),
		  ATTRNAME(name, UNSET_ITEM_IN_COLL, hints),
		  (ndims >= 1 ? ", " : ""));
	  dimArgsGen(fdh, ndims);
	  fprintf(fdh, ");\n");
	}
	else
	  {
	    fprintf(fdh, "%sStatus %s(%s", ctxH->get(),
		    ATTRNAME(name, RMV_ITEM_FROM_COLL, hints), where);
	    dimArgsGen(fdh, ndims);
	    fprintf(fdh, "%s%s%s%s);\n", comma, oclassname,
		    (_isref || !cl->asBasicClass() ? "*" : ""),
		    (!*where ? ", Bool checkFirst = False" : ""));
	  }
      }
      else if (!strcmp(cl->getName(), char_class_name) && _dim > 1) {
	fprintf(fdh, "%sStatus %s(%s", ctxH->get(),
		ATTRNAME_1(name, (ordered ? GenCodeHints::SET_ITEM_IN_COLL : GenCodeHints::ADD_ITEM_TO_COLL), hints), where);

	dimArgsGen(fdh, ndims);
	fprintf(fdh, "%sconst char *%s, const IndexImpl * = 0);\n",
		comma,
		(!*where ? ", Bool noDup = False" : ""));
	if (ordered)
	  fprintf(fdh, "%sStatus %s(", ctxH->get(),
		  ATTRNAME(name, UNSET_ITEM_IN_COLL, hints));
	else
	  fprintf(fdh, "%sStatus %s(", ctxH->get(),
		  ATTRNAME(name, RMV_ITEM_FROM_COLL, hints));

	dimArgsGen(fdh, ndims);
	fprintf(fdh, "%sconst char *%s);\n", comma,
		(!*where ? ", Bool checkFirst = False" : ""));
      }

      if (!cl->asBasicClass()) {
	fprintf(fdh, "%sStatus %s(%s", ctxH->get(),
		ATTRNAME_1(name, (ordered ? GenCodeHints::SET_ITEM_IN_COLL : GenCodeHints::ADD_ITEM_TO_COLL), hints), where);
	dimArgsGen(fdh, ndims);
	fprintf(fdh, "%sconst Oid &, const IndexImpl * = 0);\n",
		comma);
	fprintf(fdh, "%sStatus %s(", ctxH->get(),
		ATTRNAME_1(name, (ordered ? GenCodeHints::UNSET_ITEM_IN_COLL : GenCodeHints::RMV_ITEM_FROM_COLL), hints));
	dimArgsGen(fdh, ndims);
	fprintf(fdh, "%sconst Oid &);\n", comma);
      }

      if (ordered) { // 24/09/05
	if (!strcmp(cl->getName(), char_class_name) && _dim > 1) {
	  fprintf(fdh, "%sconst char *%s(unsigned int ind, ",
		  ctxH->get(),
		  ATTRNAME_1(name, (ordered ? GenCodeHints::RETRIEVE_ITEM_AT : GenCodeHints::GET_ITEM_AT), hints));
	  dimArgsGen(fdh, ndims);
	  fprintf(fdh, "%sBool *isnull = 0, Status *rs = 0) const;\n", comma);
	}
	else if (_dim == 1) {
	  fprintf(fdh, "%sconst %s %s%s(unsigned int ind, ",
		  ctxH->get(),
		  oclassname, (_isref || !cl->asBasicClass() ? "*" : ""), 
		  ATTRNAME_1(name, (ordered ? GenCodeHints::RETRIEVE_ITEM_AT : GenCodeHints::GET_ITEM_AT), hints));
	  dimArgsGen(fdh, ndims);
	  fprintf(fdh, "%sBool *isnull = 0, Status *rs = 0) const;\n", comma);
	  
	  if (!cl->asBasicClass()) {
	    fprintf(fdh, "%s%s %s%s(unsigned int ind, ",
		    ctxH->get(),
		    oclassname, (_isref || !cl->asBasicClass() ? "*" : ""),
		    ATTRNAME_1(name, (ordered ? GenCodeHints::RETRIEVE_ITEM_AT : GenCodeHints::GET_ITEM_AT), hints));
	    dimArgsGen(fdh, ndims);
	    fprintf(fdh, "%sBool *isnull = 0, Status *rs = 0);\n", comma);
	  }
	}
      
	if (_isref) {
	  fprintf(fdh, "%sOid %s(unsigned int ind, ",
		  ctxH->get(),
		  ATTRNAME_1(name, (ordered ? GenCodeHints::RETRIEVEOID_ITEM_AT : GenCodeHints::GETOID_ITEM_AT) , hints));
	  dimArgsGen(fdh, ndims);
	  fprintf(fdh, "%sStatus *rs = 0) const;\n", comma);
	}
      }
    }

    if (isVarDim() && !is_string)
      fprintf(fdh, "%sunsigned int %s(%s) const;\n", ctxH->get(),
	      ATTRNAME(name, GETCOUNT, hints), starg);

    generateBody_C(own, ctxC, hints);

    return Success;
  }

  static void const_f0(FILE *fd, Class *parent, const char *var,
		       Bool fill = False)
  {
    if (!strcmp(parent->getName(), "struct"))
      fprintf(fd, "Struct(%s)", var);
    else if (!strcmp(parent->getName(), "union"))
      fprintf(fd, "Union(%s)", var);
    else
      fprintf(fd, "%s(%s, 1)", className(parent), var);

    if (fill)
      fprintf(fd, "\n{\n");
  }

  static void const_f01(FILE *fd, Class *parent, const char *var,
			Bool fill = False)
  {
    if (!strcmp(parent->getName(), "struct"))
      fprintf(fd, "Struct(%s, share)", var);
    else if (!strcmp(parent->getName(), "union"))
      fprintf(fd, "Union(%s, share)", var);
    else
      fprintf(fd, "%s(%s, share, 1)", className(parent), var);

    if (fill)
      fprintf(fd, "\n{\n");
  }

  static void const_f1(FILE *fd, GenContext *ctx, const char *name, Bool share)
  {
    if (share)
      fprintf(fd, "%sif (!share)\n%s  {\n", ctx->get(), ctx->get());

    fprintf(fd, "%s%sheaderCode(_Struct_Type, idr_psize, IDB_XINFO_LOCAL_OBJ);\n",
	    ctx->get(), (share ? "    " : ""), name);

    fprintf(fd, "%s%sClassPeer::newObjRealize(getClass(), this);\n", ctx->get(), (share ? "    " : ""));
    if (share)
      fprintf(fd, "%s  }\n\n", ctx->get());

    fprintf(fd, "%sObjectPeer::setGRTObject(this, True);\n", ctx->get());
  }

  Status AgregatClass::generateConstructors_C(GenContext *ctx)
  {
    FILE *fd = ctx->getFile();

    fprintf(fd, "%s::%s(Database *_db, const Dataspace *_dataspace) : ", name, name);

    const_f0(fd, parent, "_db, _dataspace", True);

    if (attr_cache && !isRootClass())
      fprintf(fd, "%sattrCacheEmpty();\n", ctx->get());
    fprintf(fd, "%sinitialize(_db);\n", ctx->get());
    fprintf(fd, "}\n\n");

    if (attr_cache && !isRootClass())
      {
	fprintf(fd, "void %s::attrCacheEmpty()\n{\n", name);
	if (strcmp(getParent()->getName(), "struct") &&
	    !getParent()->isRootClass())
	  fprintf(fd, "%s%s::attrCacheEmpty();\n", ctx->get(),
		  getParent()->getCName());

	int i;
	for (i = 0; i < items_cnt; i++)
	  if (items[i]->getClassOwner()->compare(this))
	    items[i]->genAttrCacheEmpty(ctx);
	fprintf(fd, "}\n\n");

	fprintf(fd, "void %s::garbage()\n{\n", name);
	for (i = 0; i < items_cnt; i++)
	  if (items[i]->getClassOwner()->compare(this))
	    items[i]->genAttrCacheGarbage(ctx);
	fprintf(fd, "%s%s::garbage();\n", ctx->get(), getParent()->getCName());
	fprintf(fd, "}\n\n");
      }

    // changed the 12/10/00 because of a memory leaks
    /*
      fprintf(fd, "%s::%s(const Class *_cls, Data _idr)\n{\n",
      name, name);
    */
    fprintf(fd, "%s::%s(const Class *_cls, Data _idr)", name, name);
    if (strcmp(parent->getName(), "struct"))
      fprintf(fd, ": %s((Database *)0, (const Dataspace *)0, 1)", className(parent));
    fprintf(fd, "\n{\n");
    fprintf(fd, "%ssetClass((Class *)_cls);\n\n", ctx->get());

    fprintf(fd, "%sSize idr_psize;\n", ctx->get());
    fprintf(fd, "%sSize idr_tsize = getClass()->getIDRObjectSize(&idr_psize);\n", ctx->get(), name);
    fprintf(fd, "%sif (_idr)\n", ctx->get());
    fprintf(fd, "%s  idr->setIDR(idr_tsize, _idr);\n", ctx->get());
    fprintf(fd, "%selse\n", ctx->get());
    fprintf(fd, "%s  {\n", ctx->get());
    // changed the 5/02/01
    //  fprintf(fd, "%s    idr->setIDR(idr->getSize());\n", ctx->get());
    fprintf(fd, "%s    idr->setIDR(idr_tsize);\n", ctx->get());
    fprintf(fd, "%s    memset(idr->getIDR() + IDB_OBJ_HEAD_SIZE, 0, idr->getSize() - IDB_OBJ_HEAD_SIZE);\n", ctx->get());
    fprintf(fd, "%s  }\n", ctx->get());

    fprintf(fd, "%sheaderCode(_Struct_Type, idr_psize, IDB_XINFO_LOCAL_OBJ);\n", ctx->get(), name);
    fprintf(fd, "%sClassPeer::newObjRealize(getClass(), this);\n", ctx->get());
    fprintf(fd, "%sObjectPeer::setGRTObject(this, True);\n", ctx->get());
    fprintf(fd, "%suserInitialize();\n", ctx->get());
    fprintf(fd, "}\n\n");

    fprintf(fd, "void %s::initialize(Database *_db)\n{\n",
	    name);
    fprintf(fd, "%ssetClass((_db ? _db->getSchema()->getClass(\"%s\") : %s%s));\n\n", ctx->get(), getAliasName(), name, classSuffix);
    fprintf(fd, "%sSize idr_psize;\n", ctx->get());
    //fprintf(fd, "%sidr->idr_sz = getClass()->getIDRObjectSize(&idr_psize);\n", ctx->get(), name);
    fprintf(fd, "%sidr->setIDR(getClass()->getIDRObjectSize(&idr_psize));\n",
	    ctx->get(), name);
  
    //fprintf(fd, "%sidr->idr = (unsigned char *)malloc(idr->idr_sz * sizeof(unsigned char));\n\n", ctx->get());
    fprintf(fd, "%smemset(idr->getIDR() + IDB_OBJ_HEAD_SIZE, 0, idr->getSize() - IDB_OBJ_HEAD_SIZE);\n",
	    ctx->get());

    const_f1(fd, ctx, name, False);
    if (attr_cache && !isRootClass())
      fprintf(fd, "%sattrCacheEmpty();\n", ctx->get());
    fprintf(fd, "%suserInitialize();\n", ctx->get());
    fprintf(fd, "}\n\n");

    fprintf(fd, "%s::%s(const %s& x) : %s(x)\n{\n",
	    name, name, name, parent->getCName(), ctx->get());
    if (attr_cache && !isRootClass())
      fprintf(fd, "%sattrCacheEmpty();\n", ctx->get());
    fprintf(fd, "%suserCopy(x);\n", ctx->get());
    fprintf(fd, "}\n\n");
    fprintf(fd, "%s& %s::operator=(const %s& x)\n{\n", name, name, name);
    fprintf(fd, "%s*(%s *)this = %s::operator=((const %s &)x);\n",
	    ctx->get(), parent->getCName(), parent->getCName(),  parent->getCName());
    if (attr_cache && !isRootClass())
      fprintf(fd, "%sattrCacheEmpty();\n", ctx->get());
    fprintf(fd, "%suserCopy(x);\n", ctx->get());
    fprintf(fd, "%sreturn *this;\n", ctx->get());
    fprintf(fd, "}\n\n", ctx->get());

    fprintf(fd, "%s::%s(const Struct *x, Bool share) : ",
	    name, name, className(parent));

    const_f01(fd, parent, "x", True);

    fprintf(fd, "%ssetClass((db ? db->getSchema()->getClass(\"%s\") : %s%s));\n\n", ctx->get(), getAliasName(), name, classSuffix);
    fprintf(fd, "%sSize idr_psize;\n", ctx->get());
    fprintf(fd, "%sgetClass()->getIDRObjectSize(&idr_psize);\n", ctx->get(), name);

    const_f1(fd, ctx, name, True);
    if (attr_cache && !isRootClass())
      fprintf(fd, "%sattrCacheEmpty();\n", ctx->get());
    fprintf(fd, "%suserCopy(*x);\n", ctx->get());
    fprintf(fd, "}\n\n");

    fprintf(fd, "%s::%s(const %s *x, Bool share) : ", name, name,
	    name, className(parent));

    const_f01(fd, parent, "x", True);

    fprintf(fd, "%ssetClass((db ? db->getSchema()->getClass(\"%s\") : %s%s));\n\n", ctx->get(), getAliasName(), name, classSuffix);

    fprintf(fd, "%sSize idr_psize;\n", ctx->get());
    fprintf(fd, "%sgetClass()->getIDRObjectSize(&idr_psize);\n", ctx->get(), name);
    const_f1(fd, ctx, name, True);
    if (attr_cache && !isRootClass())
      fprintf(fd, "%sattrCacheEmpty();\n", ctx->get());
    fprintf(fd, "%suserCopy(*x);\n", ctx->get());

    fprintf(fd, "}\n\n");
    return Success;
  }

  Status AgregatClass::generateDownCasting_C(GenContext *ctx,
					     Schema *m)
  {
    const LinkedList *list = m->getClassList();
    LinkedListCursor *curs = list->startScan();
    FILE *fd = ctx->getFile();

    Class *cl;

    while (list->getNextObject(curs, (void *&)cl))
      {
	Bool is;
	if (isSuperClassOf(cl, &is) == Success && is)
	  {
	    const char *s = className(cl);
	    char body[128], body_const[128];

	    if (cl == this)
	      {
		sprintf(body, " {return this;}\n");
		strcpy(body_const, body);
	      }
	    else
	      {
		sprintf(body, " {return (%s *)0;}\n", s);
		sprintf(body_const, " {return (const %s *)0;}\n", s);
	      }

	    fprintf(fd, "%svirtual %s *%s%s()%s", ctx->get(),
		    s, downCastPrefix, cl->getCanonicalName(), body);
	  
	    fprintf(fd, "%svirtual const %s *%s%s() const%s", ctx->get(),
		    s, downCastPrefix, cl->getCanonicalName(), body_const);
	  }
      }

    return Success;
  }

  static int
  true_items_count(const Class *cls, Attribute **items, int items_cnt)
  {
    for (int i = 0; i < items_cnt; i++)
      if (items[i]->getClassOwner() == cls && !items[i]->isNative())
	return items_cnt - i;

    return 0;
  }

  Status
  AgregatClass::generateClassDesc_C(GenContext *ctx,
				    const char *)
  {
    FILE *fd = ctx->getFile();
    Status status;
    const char *_type = (asStructClass() ? "Struct" : "Union");

    fprintf(fd, "static const Attribute **%s_agritems;\n", name);
    fprintf(fd, "static Size %s_idr_objsz, %s_idr_psize;\n\n", name, name);

    fprintf(fd, "static " DEF_PREFIX "%sClass *%s_make(" DEF_PREFIX "%sClass *%s_class = 0, Schema *m = 0)\n{\n", _type, name, _type, name);
    ctx->push();

    fprintf(fd, "%sif (!%s_class)\n", ctx->get(), name);
    Class *parent_cl = parent->isRootClass() ? parent->getParent() : parent;
    fprintf(fd,"%s  return new " DEF_PREFIX "%sClass(\"%s\", (m ? m->getClass(\"%s\") : %s%s));\n", ctx->get(), _type, getAliasName(),
	    (parent_cl->getAliasName() ?  parent_cl->getAliasName() : parent_cl->getName()),
	    className(parent_cl), classSuffix);
    // changed the 30/05/01: was:
    // className(parent_cl, True, True), className(parent_cl), classSuffix);

    if (items_cnt)
      {
	fprintf(fd, "%sAttribute *attr[%d];\n", ctx->get(), items_cnt);
	int dims_declare = 0;
      
	int i;
	for (i = 0; i < items_cnt; i++)
	  if (items[i]->class_owner == (void *)this && !items[i]->isNative())
	    {
	      if (!dims_declare)
		{
		  dims_declare = 1;
		  fprintf(fd, "%sint *dims;\n", ctx->get());
		}
	      status = items[i]->generateClassDesc_C(ctx);
	      if (status)
		return status;
	    }
      
	int count = true_items_count(this, items, items_cnt);
	fprintf(fd, "\n%s%s_class->setAttributes(&attr[%d], %d);\n", ctx->get(),
		name, items_cnt - count, count);

	fprintf(fd, "\n");

	for (i = items_cnt - count; i < items_cnt; i++)
	  fprintf(fd, "%sdelete attr[%d];\n", ctx->get(), i);

	fprintf(fd, "\n");
      }
    else
      fprintf(fd, "\n%s%s_class->setAttributes(0, 0);\n", ctx->get(), name);

    // should set implementation. Really necessary ?? Bof.
    /*
      if (idximpl->getType() == IndexImpl::Hash)
      fprintf(fd, "%sidximpl = new IndexImpl(IndexImpl::Hash, 0, "
      "%d, idximp
    */

    if (isSystem() || odl_system_class)
      fprintf(fd, "%sClassPeer::setMType(%s_class, Class::System);\n",
	      ctx->get(), name);

    fprintf(fd, "\n%sreturn %s_class;\n}\n\n", ctx->get(),
	    name);

    fprintf(fd, "Object *%s_construct_x(const Class *cls, Data idr)\n{\n", name);
    fprintf(fd, "%sreturn new %s(cls, idr);\n", ctx->get(), name);
    fprintf(fd, "}\n\n");

    fprintf(fd, "Object *%s_construct(const Object *o, Bool share)\n{\n", name);
    fprintf(fd, "%sreturn new %s((const Struct *)o, share);\n", ctx->get(), name);
    fprintf(fd, "}\n\n");

    fprintf(fd, "static void %s_init_p()\n{\n", name);
    fprintf(fd, "%s%s%s = %s_make();\n", ctx->get(), name, classSuffix, name);
    fprintf(fd, "%sconstructors_x[class_ind] = %s_construct_x;\n", ctx->get(),
	    name);
    fprintf(fd, "%sconstructors[class_ind] = %s_construct;\n", ctx->get(),
	    name);
    fprintf(fd, "%shash->insert(\"%s\", class_ind++);\n", ctx->get(),
	    getAliasName());
    fprintf(fd, "}\n\n");

    fprintf(fd, "static void %s_init()\n{\n", name);
    fprintf(fd, "%s%s_make(%s%s);\n\n", ctx->get(), name, name, classSuffix);
    fprintf(fd, "%s%s_agritems = %s%s->getAttributes();\n", ctx->get(),
	    name, name, classSuffix);
    fprintf(fd, "%s%s_idr_objsz = %s%s->getIDRObjectSize(&%s_idr_psize, 0);\n\n",
	    ctx->get(), name, name, classSuffix, name);

    fprintf(fd, "%sObjectPeer::setUnrealizable(%s%s, True);\n",
	    ctx->get(), name, classSuffix);
    fprintf(fd, "}\n\n");
    ctx->pop();
    return Success;
  }

#define NEW_ARGTYPE

  static inline void
  argTypeDump(GenContext *ctx, ArgType *argtype, const char *str)
  {
    FILE *fd = ctx->getFile();

#ifdef NEW_ARGTYPE
    fprintf(fd, "%sargtype = %s;\n", ctx->get(), str);
#else
    fprintf(fd, "%sargtype = new ArgType();\n", ctx->get());
#endif

    fprintf(fd, "%sargtype->setType((ArgType_Type)%d, False);\n",
	    ctx->get(), argtype->getType());

    /*
      if ((argtype->getType() & ~(INOUT_ARG_TYPE|ARRAY_TYPE)) == OBJ_TYPE)
    */
    fprintf(fd, "%sargtype->setClname(\"%s\");\n", ctx->get(),
	    argtype->getClname());
  }

  static void
  sign_declare_realize(FILE *fd, GenContext *ctx)
  {
    fprintf(fd, "%sSignature *sign;\n", ctx->get());
    fprintf(fd, "%sArgType *argtype;\n\n", ctx->get());
  }

#define dataspace_prologue(ctx, fd, x) \
do { \
  Bool isnull; \
  short dspid = x->getDspid(&isnull); \
  if (!isnull) { \
    fprintf(fd, "%sstatus = db->getDataspace(%d, dataspace);\n", \
	    ctx->get(), dspid); \
    fprintf(fd, "%sif (status) return status;\n", ctx->get()); \
  } \
  else \
    fprintf(fd, "%sdataspace = 0;;\n", ctx->get()); \
} while(0)

#define hints_prologue(ctx, fd, x) \
do { \
  int cnt = x->getImplHintsCount(); \
  if (cnt) { \
    fprintf(fd, "%simpl_hints = new int[%d];\n", \
	    ctx->get(), cnt); \
    for (int i = 0; i < cnt; i++) \
      fprintf(fd, "%simpl_hints[%d] = %d;\n", ctx->get(), i, \
	      x->getImplHints(i)); \
  } \
} while(0)


#define hints_epilogue(ctx, fd, x) \
do { \
  int cnt = x->getImplHintsCount(); \
  if (cnt) { \
    fprintf(fd, ", impl_hints, %d);\n", cnt); \
    fprintf(fd, "%sdelete [] impl_hints;\n", ctx->get());\
  } \
  else \
    fprintf(fd, "0, 0);\n"); \
} while(0)

  void
  Class::compAddGenerate(GenContext *ctx, FILE *fd)
  {
    if (asAgregatClass()) {
      /*
	fprintf(fd, "%sif (cls->getOid().isValid()) {\n", ctx->get());
	fprintf(fd, "%s  if (status = comp->realize()) return status;\n", ctx->get());
	fprintf(fd, "%s} else\n", ctx->get());
      */
      fprintf(fd, "%scls->add(comp->getInd(), comp);\n\n", ctx->get());
    }
    else
      fprintf(fd, "%sif (status = comp->realize()) return status;\n\n", ctx->get());
  }

  Status Class::generateAttrComponent_C(GenContext *ctx)
  {
    if (!getUserData(odlGENCODE) && !getUserData(odlGENCOMP)) {
      printf("generateAttrComponent_C -> is system %s %p\n", name, this);
      return Success;
    }

    const LinkedList *list;
    Status s = getAttrCompList(list);
    if (s) return s;

    FILE *fd = ctx->getFile();

    fprintf(fd, "static Status %s_attrcomp_realize(Database *db, Class *cls)\n{\n", name);

    ctx->push();
    if (list->getCount()) {
      fprintf(fd, "%sAttributeComponent *comp;\n", ctx->get());
      fprintf(fd, "%sconst Dataspace *dataspace;\n", ctx->get());
      fprintf(fd, "%sClassComponent *clcomp;\n", ctx->get());
      fprintf(fd, "%sStatus status;\n", ctx->get());
      fprintf(fd, "%sint *impl_hints;\n", ctx->get());
    }

    LinkedListCursor c(list);

    AttributeComponent *comp;
    Bool setup_done = False;
    while (c.getNext((void *&)comp)){
      if (comp->asNotNullConstraint()) {
	NotNullConstraint *notnull = comp->asNotNullConstraint();
	fprintf(fd, "%scomp = new NotNullConstraint(db, cls, \"%s\", %s);\n",
		ctx->get(), notnull->getAttrpath(),
		IDBBOOL_STR(notnull->getPropagate()));
      }
      else if (comp->asUniqueConstraint()) {
	UniqueConstraint *unique = comp->asUniqueConstraint();
	fprintf(fd, "%scomp = new UniqueConstraint(db, cls, \"%s\", %s);\n",
		ctx->get(), unique->getAttrpath(),
		IDBBOOL_STR(unique->getPropagate()));
      }
      /*
	else if (comp->asCardinalityConstraint()) {
	CardinalityConstraint *card = comp->asCardinalityConstraint();
	CardinalityDescription *card_desc = card->getCardDesc();
	fprintf(fd, "%scomp = new CardinalityConstraint(db, cls, \"%s\", %d, %d, %d, %d);\n",
	ctx->get(), card->getAttrname(), card_desc->getBottom(),
	card_desc->getBottomExcl(),
	card_desc->getTop(), card_desc->getTopExcl());
	}
      */
      else if (comp->asBTreeIndex()) {
	BTreeIndex *idx = comp->asBTreeIndex();
	hints_prologue(ctx, fd, idx);
	dataspace_prologue(ctx, fd, idx);
	fprintf(fd, "%scomp = new BTreeIndex(db, cls, \"%s\", %s, %s, dataspace, %d",
		ctx->get(), idx->getAttrpath(),
		IDBBOOL_STR(idx->getPropagate()),
		IDBBOOL_STR(idx->getIsString()),
		idx->getDegree());
	hints_epilogue(ctx, fd, idx);
      }
      else if (comp->asHashIndex()) {
	HashIndex *idx = comp->asHashIndex();
	if (idx->getHashMethod()) {
	  // hack
	  if (!setup_done) {
	    fprintf(fd, "%sgetClass()->setSetupComplete(True);\n", ctx->get());
	    setup_done = True;
	  }
	  fprintf(fd, "%sstatus = getClass()->getComp(\"%s\", clcomp);\n",
		  ctx->get(), idx->getHashMethod()->getName());
	  fprintf(fd, "%sif (status) return status;\n", ctx->get());
	}
	hints_prologue(ctx, fd, idx);
	dataspace_prologue(ctx, fd, idx);
	fprintf(fd, "%scomp = new HashIndex(db, cls, \"%s\", %s, %s, dataspace, %d",
		ctx->get(), idx->getAttrpath(),
		IDBBOOL_STR(idx->getPropagate()),
		IDBBOOL_STR(idx->getIsString()),
		idx->getKeyCount());
      
	if (idx->getHashMethod())
	  fprintf(fd, ", clcomp->asBEMethod_C()");
	else
	  fprintf(fd, ", 0");
	hints_epilogue(ctx, fd, idx);
      }
      else if (comp->asCollAttrImpl()) {
	CollAttrImpl *collimpl = comp->asCollAttrImpl();
	if (collimpl->getHashMethod()) {
	  // hack
	  if (!setup_done) {
	    fprintf(fd, "%sgetClass()->setSetupComplete(True);\n", ctx->get());
	    setup_done = True;
	  }
	  fprintf(fd, "%sstatus = getClass()->getComp(\"%s\", clcomp);\n",
		  ctx->get(), collimpl->getHashMethod()->getName());
	  fprintf(fd, "%sif (status) return status;\n", ctx->get());
	}
	hints_prologue(ctx, fd, collimpl);
	dataspace_prologue(ctx, fd, collimpl);
	fprintf(fd, "%scomp = new CollAttrImpl(db, cls, \"%s\", %s, dataspace, %s, %d",
		ctx->get(), collimpl->getAttrpath(),
		IDBBOOL_STR(collimpl->getPropagate()),
		(collimpl->getIdxtype() == IndexImpl::Hash ?
		 "IndexImpl::Hash" : "IndexImpl::BTree"),
		collimpl->getKeyCountOrDegree());
      
	if (collimpl->getHashMethod())
	  fprintf(fd, ", clcomp->asBEMethod_C()");
	else
	  fprintf(fd, ", 0");
	hints_epilogue(ctx, fd, collimpl);
      }
      else
	abort();
    
      compAddGenerate(ctx, fd);
    }

    fprintf(fd, "%sreturn Success;\n", ctx->get());

    ctx->pop();
    fprintf(fd, "}\n\n");
    return Success;
  }

  Status Class::generateClassComponent_C(GenContext *ctx)
  {
    if (!getCompList() || !getCompList()->getCount())
      return Success;

    if (!getUserData(odlGENCODE) && !getUserData(odlGENCOMP)) {
      printf("generateClassComponent_C -> is system %s %p\n", name, this);
      return Success;
    }

    FILE *fd = ctx->getFile();

    Status status;
    //const char *_type = (asStructClass() ? "Struct" : "Union");

    fprintf(fd, "static Status %s_comp_realize(Database *db, Class *cls)\n{\n", name);

    ctx->push();
    if (complist->getCount())
      {
	fprintf(fd, "%sClassComponent *comp;\n", ctx->get());
	fprintf(fd, "%sStatus status;\n", ctx->get());
      }

    int sign_declare = 0;

    LinkedListCursor c(complist);

    ClassComponent *comp;
    while (complist->getNextObject(&c, (void *&)comp))
      {
	if (comp->asCardinalityConstraint())
	  {
	    CardinalityConstraint *card = comp->asCardinalityConstraint();
	    CardinalityDescription *card_desc = card->getCardDesc();
	    fprintf(fd, "%scomp = new CardinalityConstraint(db, cls, \"%s\", %d, %d, %d, %d);\n",
		    ctx->get(), card->getAttrname(), card_desc->getBottom(),
		    card_desc->getBottomExcl(),
		    card_desc->getTop(), card_desc->getTopExcl());
	  }
	else if (comp->asTrigger())
	  {
	    Trigger *trig = comp->asTrigger();
	    char *x = purge(trig->getEx()->getExtrefBody());
	    fprintf(fd, "%scomp = new Trigger(db, cls, (TriggerType)%d, (ExecutableLang)%d, %s, \"%s\", %s, \"%s\");\n",
		    ctx->get(), trig->getType(),
		    (trig->getEx()->getLang() & ~SYSTEM_EXEC),
		    STRBOOL(trig->getEx()->getLang() & SYSTEM_EXEC),
		    trig->getSuffix(),
		    trig->getLight() ? "True" : "False",
		    x);
	    free(x);
	  }
	else if (comp->asMethod())
	  {
	    Method *mth = comp->asMethod();
	    Executable *ex = mth->getEx();

	    if ((ex->getLang() & (C_LANG|SYSTEM_EXEC)) ==
		(C_LANG|SYSTEM_EXEC) && !odl_system_class)
	      continue;

	    if (!sign_declare)
	      {
		sign_declare = 1;
		sign_declare_realize(fd, ctx);
	      }

	    fprintf(fd, "%ssign = new Signature();\n",
		    ctx->get());
	    Signature *sign = ex->getSign();
	    const char *prefix = ((ex->getLoc()&~STATIC_EXEC) == FRONTEND ? "FE" : "BE");
	    const char *lang = ((ex->getLang() & C_LANG) ? "C"  : "OQL");
	    char *extref = purge(ex->getExtrefBody());

#ifdef NEW_ARGTYPE
	    argTypeDump(ctx, sign->getRettype(), "sign->getRettype()");
#else
	    argTypeDump(ctx, sign->getRettype());
	    fprintf(fd, "%ssign->setRettype(argtype);\n", ctx->get());
	    fprintf(fd, "%sargtype->release();\n\n", ctx->get());
#endif

	    int nargs = sign->getNargs();
	    fprintf(fd, "%ssign->setNargs(%d);\n", ctx->get(), nargs);
	    fprintf(fd, "%ssign->setTypesCount(%d);\n", ctx->get(), nargs);
	    int i;
	    for (i = 0; i < nargs; i++)
	      {
#ifdef NEW_ARGTYPE
		char tok[128];
		sprintf(tok, "sign->getTypes(%d)", i);
		argTypeDump(ctx, sign->getTypes(i), tok);
#else
		argTypeDump(ctx, sign->getTypes(i));
		fprintf(fd, "%ssign->setTypes(%d, argtype);\n", ctx->get(),
			i);
		fprintf(fd, "%sargtype->release();\n\n", ctx->get());
#endif
	      }

	    fprintf(fd,
		    "%scomp = new " DEF_PREFIX2 "%sMethod_%s(db, cls, \"%s\", sign, %s, %s, \"%s\");\n",
		    ctx->get(), prefix, lang, ex->getExname(),
		    STRBOOL(ex->isStaticExec()),
		    STRBOOL(ex->getLang() & SYSTEM_EXEC),
		    extref);
	    free(extref);
	  }
	else
	  abort();

	compAddGenerate(ctx, fd);

	if (comp->asMethod())
	  fprintf(fd, "%ssign->release();\n\n", ctx->get());
      }

    fprintf(fd, "%sreturn Success;\n", ctx->get());

    ctx->pop();
    fprintf(fd, "}\n\n");
    return Success;
  }

  Status AgregatClass::generateConstructors_C(GenContext *ctxH,
					      GenContext *ctxC)
  {
    FILE *fdh = ctxH->getFile();

    fprintf(fdh, "%s%s(Database * = 0, const Dataspace * = 0);\n", ctxH->get(), name);

    /*
      fprintf(fdh, "%s%s(const %s& x) : %s(x) {attrCacheEmpty();}\n",
      ctxH->get(), name, name, parent->getCName());

      fprintf(fdh, "%s%s& operator=(const %s& x) {\n", ctxH->get(), name, name);
      fprintf(fdh, "%s  *(%s *)this = %s::operator=((const %s &)x);\n",
      ctxH->get(),parent->getCName(), parent->getCName());
      fprintf(fdh, "%s  attrCacheEmpty();\n", ctxH->get());
      fprintf(fdh, "%s  return *this;\n", ctxH->get());
      fprintf(fdh, "%s}\n\n", ctxH->get());
    */

    fprintf(fdh, "%s%s(const %s& x);\n\n", ctxH->get(), name, name);
    fprintf(fdh, "%svirtual Object *clone() const "
	    "{return new %s(*this);};\n\n", ctxH->get(), name);

    fprintf(fdh, "%s%s& operator=(const %s& x);\n\n", ctxH->get(), name, name);

    generateConstructors_C(ctxC);

    return Success;
  }

  Status
  AgregatClass::generateMethodDecl_C(Schema *m, GenContext *ctx)
  {
    FILE *fd = ctx->getFile();
    LinkedListCursor c(complist);

    ClassComponent *comp;

    while (c.getNext((void *&)comp))
      if (comp->asMethod())
	{
	  Method *mth = comp->asMethod();
	  Executable *ex = mth->getEx();

	  if (!(ex->getLang() & C_LANG))
	    continue;

	  if (!mth->getUserData())
	    continue;

	  Signature *sign = ex->getSign();
	
	  if (sign->getUserData()) {
	    odlMethodHints *mth_hints = ((odlSignUserData *)sign->getUserData())->mth_hints;
	    if (mth_hints->calledFrom != odlMethodHints::ANY_HINTS &&
		mth_hints->calledFrom != odlMethodHints::C_HINTS)
	      continue;
	  }

	  if (ex->isStaticExec()) {
	    fprintf(fd, "%sstatic Status %s(Database *db", ctx->get(),
		    ex->getExname());
	    if (sign->getNargs() || !Signature::isVoid(sign->getRettype()))
	      fprintf(fd, ", ");
	  }
	  else
	    fprintf(fd, "%svirtual Status %s(", ctx->get(), ex->getExname());

	  sign->declArgs(fd, m);
	  fprintf(fd, ");\n\n");
	}

    return Success;
  }

  Status
  AgregatClass::generateMethodFetch_C(GenContext *ctx, Method *mth)
  {
    FILE *fd = ctx->getFile();
    fprintf(fd, "%sif (!mth)\n", ctx->get());
    fprintf(fd, "%s{\n", ctx->get());
    ctx->push();
    fprintf(fd, "%sstatus = Method::get(db, "
	    "db->getSchema()->getClass(\"%s\"), \"%s\", %s, mth);\n",
	    ctx->get(), (getAliasName() ? getAliasName() : getName()),
	    mth->getPrototype(False),
	    STRBOOL(mth->getEx()->isStaticExec()));

    fprintf(fd, "%sif (status) return status;\n", ctx->get());

    fprintf(fd, "%sif (!mth)\n", ctx->get());
    fprintf(fd, "%s  return Exception::make(IDB_ERROR, "
	    "\"method '%s' not found\");\n", ctx->get(), mth->getPrototype());
    ctx->pop();
    fprintf(fd, "%s}\n\n", ctx->get());

    return Success;
  }

  Status
  AgregatClass::generateMethodBodyFE_C(Schema *m,
				       GenContext *ctx, Method *mth)
  {
    FILE *fd = ctx->getFile();
    Executable *ex = mth->getEx();
    Signature *sign = ex->getSign();

    fprintf(fd, "Status %s::%s(", name, ex->getExname());
    if (ex->isStaticExec()) {
      fprintf(fd, "Database *db");
      if (sign->getNargs() || !Signature::isVoid(sign->getRettype()))
	fprintf(fd, ", ");
    }
    sign->declArgs(fd, m);
    fprintf(fd, ")\n{\n");
    ctx->push();

    fprintf(fd, "%sstatic Method *mth;\n", ctx->get());
    fprintf(fd, "%sStatus status;\n", ctx->get());

    generateMethodFetch_C(ctx, mth);

#ifdef NEW_ARGARR
    fprintf(fd, "%sstatic ArgArray *argarr = new ArgArray(%d, Argument::AutoFullGarbage);\n\n",
	    ctx->get(), sign->getNargs());
    sign->setArgs(fd, m, IN_ARG_TYPE, "(*argarr)[%d]->", "retarg.", ctx->get());
    fprintf(fd, "\n%sArgument __retarg;\n", ctx->get());
    fprintf(fd, "%sstatus = mth->applyTo(db, %s, *argarr, __retarg, False);\n\n",
	    ctx->get(), (mth->getEx()->isStaticExec() ? "0" : "this"));
    fprintf(fd, "%sif (status) return status;\n\n", ctx->get());
    sign->retArgs(fd, m, "(*argarr)[%d]->", "__retarg.", ctx->get());
#else
    fprintf(fd, "%sArgArray argarr(%d, Argument::AutoFullGarbage);\n\n",
	    ctx->get(), sign->getNargs());
    sign->setArgs(fd, m, IN_ARG_TYPE, "argarr[%d]->", "retarg.", ctx->get());
    fprintf(fd, "\n%sArgument __retarg;\n", ctx->get());
    fprintf(fd, "%sstatus = mth->applyTo(db, %s, argarr, __retarg, False);\n\n",
	    ctx->get(), (mth->getEx()->isStaticExec() ? "0" : "this"));
    fprintf(fd, "%sif (status) return status;\n\n", ctx->get());
    sign->retArgs(fd, m, "argarr[%d]->", "__retarg.", ctx->get());
#endif

    fprintf(fd, "%sreturn Success;\n}\n\n", ctx->get());
    ctx->pop();

    return Success;
  }

  static void
  userImpl(FILE *fd)
  {
    fprintf(fd, "  _packageInit(_db);\n");
    fprintf(fd, "\n  //\n  // User Implementation\n  //\n\n");
    fprintf(fd, "  return Success;\n");
  }

  Status
  Class::generateMethodBodyFE_C(Schema *m,
				GenContext *ctx, Method *mth)
  {
    return Success;
  }

  Status
  Class::generateMethodBodyBE_C(Schema *m,
				GenContext *ctxStubs,
				GenContext *ctxMth,
				Method *mth)
  {
    FILE *fdstubs = ctxStubs->getFile();
    FILE *fdmth = ctxMth->getFile();
    Executable *ex = mth->getEx();

    Signature *sign = ex->getSign();
    const char *intname =
      Executable::makeInternalName(ex->getExname(),
				   sign,
				   (mth->getEx()->isStaticExec() ? True:
				    False),
				   (getAliasName() ?
				    getAliasName() : getName()));
  
    //const char *prefix = (mth->asFEMethod_C() ? "FE" : "BE");
    const char *prefix = ((ex->getLoc()&~STATIC_EXEC) == FRONTEND ? "FE" : "BE");
    const char *lang = ((ex->getLang() & C_LANG) ? "C"  : "OQL");

    fprintf(fdmth, "//\n// %s [%s.cc]\n//\n\n", mth->getPrototype(),
	    mth->getEx()->getExtrefBody());
    fprintf(fdmth, "Status\n");
    fprintf(fdmth, "__%s(Database *_db, " DEF_PREFIX2 "%sMethod_%s *_m%s",
	    intname,
	    prefix, lang,
	    (ex->isStaticExec() ? "" : ", Object *_o"));
    
    if (sign->getNargs() || !Signature::isVoid(sign->getRettype()))
      fprintf(fdmth, ", ");

    sign->declArgs(fdmth, m);

    fprintf(fdmth, ")\n{\n");
    userImpl(fdmth);
    fprintf(fdmth, "}\n\n");

    fprintf(fdstubs, "extern Status __%s(Database *_db, "
	    DEF_PREFIX2 "%sMethod_%s *_m%s",
	    intname,
	    prefix, lang,
	    (ex->isStaticExec() ? "" : ", Object *_o"));

    if (sign->getNargs() || !Signature::isVoid(sign->getRettype()))
      fprintf(fdstubs, ", ");

    sign->declArgs(fdstubs, m);
    fprintf(fdstubs, ");\n\n");
  
    fprintf(fdstubs, "extern \"C\" Status\n");
    fprintf(fdstubs, "%s(Database *_db, " DEF_PREFIX2 "%sMethod_%s *_m, "
	    "Object *_o, ArgArray &_array, Argument &__retarg)\n{\n",
	    intname,
	    prefix, lang);

    ctxStubs->push();
    fprintf(fdstubs, "%sStatus status;\n\n", ctxStubs->get());

    sign->initArgs(fdstubs, m, "_array[%d]->", "_retarg", ctxStubs->get());

    fprintf(fdstubs, "\n%sstatus = __%s(_db, _m%s", ctxStubs->get(), intname,
	    (ex->isStaticExec() ? "" : ", _o"));

    if (sign->getNargs() || !Signature::isVoid(sign->getRettype()))
      fprintf(fdstubs, ", ");

    sign->listArgs(fdstubs, m);

    fprintf(fdstubs, ");\n");

    fprintf(fdstubs, "%sif (status) return status;\n\n", ctxStubs->get());

    sign->setArgs(fdstubs, m, OUT_ARG_TYPE, "_array[%d]->", "__retarg.",
		  ctxStubs->get());

    fprintf(fdstubs, "%sreturn Success;\n}\n\n", ctxStubs->get());
    ctxStubs->pop();

    return Success;
  }

  Status
  Class::generateTriggerBody_C(Schema *m,
			       GenContext *ctxMth,
			       Trigger *trig)
  {
    FILE *fdmth = ctxMth->getFile();

    fprintf(fdmth, "//\n// %s\n//\n\n", trig->getPrototype());
    fprintf(fdmth, "extern \"C\"\n");
    fprintf(fdmth, "Status %s(TriggerType type, Database *_db, "
	    "const Oid &oid, Object *o)\n{\n",
	    trig->getCSym());
    userImpl(fdmth);
    fprintf(fdmth, "}\n\n");
    return Success;
  }

  Status
  Class::generateMethodBody_C(Schema *m,
			      GenContext *ctx,
			      GenContext *ctxStubsFe,
			      GenContext *ctxStubsBe,
			      GenContext *ctxMthFe,
			      GenContext *ctxMthBe)
  {
    if (!getUserData(odlGENCODE) && !getUserData(odlGENCOMP))
      return Success;

    LinkedListCursor c(complist);

    ClassComponent *comp;

    while (c.getNext((void *&)comp))
      if (comp->asMethod()) {
	Method *mth = comp->asMethod();
	if (!(mth->getEx()->getLang() & C_LANG))
	  continue;

	Signature *sign = mth->getEx()->getSign();

	if (mth->asBEMethod())
	  generateMethodBodyBE_C(m, ctxStubsBe, ctxMthBe, mth);
	else
	  generateMethodBodyBE_C(m, ctxStubsFe, ctxMthFe, mth);

	if (!mth->getUserData())
	  continue;

	if (sign->getUserData()) {
	  odlMethodHints *mth_hints = ((odlSignUserData *)sign->getUserData())->mth_hints;
	  if (mth_hints->calledFrom != odlMethodHints::ANY_HINTS &&
	      mth_hints->calledFrom != odlMethodHints::C_HINTS)
	    continue;
	}
	generateMethodBodyFE_C(m, ctx, mth);
      }
      else if (comp->asTrigger())
	generateTriggerBody_C(m, ctxMthBe, comp->asTrigger());

    return Success;
  }

  Status
  Class::generateCode_C(Schema *m,
			const char *prefix,
			const GenCodeHints &hints,
			const char *stubs,
			FILE *fdh,
			FILE *fdc, FILE *fdstubsfe,
			FILE *fdstubsbe,
			FILE *fdmthfe,
			FILE *fdmthbe)
  {
    GenContext ctxH(fdh), ctxC(fdc), ctxStubsFe(fdstubsfe),
      ctxStubsBe(fdstubsbe), ctxMthFe(fdmthfe), ctxMthBe(fdmthbe);

    ctxC.push();
    generateMethodBody_C(m, &ctxC, &ctxStubsFe, &ctxStubsBe,
			 &ctxMthFe, &ctxMthBe);
    ctxC.pop();

    generateClassComponent_C(&ctxC);
    generateAttrComponent_C(&ctxC);

    return Success;
  }

  Status
  AgregatClass::generateCode_C(Schema *m,
			       const char *prefix,
			       const GenCodeHints &hints,
			       const char *stubs,
			       FILE *fdh,
			       FILE *fdc, FILE *fdstubsfe,
			       FILE *fdstubsbe,
			       FILE *fdmthfe,
			       FILE *fdmthbe)
  {
    GenContext ctxH(fdh), ctxC(fdc), ctxStubsFe(fdstubsfe),
      ctxStubsBe(fdstubsbe), ctxMthFe(fdmthfe), ctxMthBe(fdmthbe);

    int i;
    Status status;

    phints = &hints;
    class_enums = hints.class_enums;
    attr_cache  = hints.attr_cache;

    if (getUserData(odlGENCODE))
      generateClassDesc_C(&ctxC, "");

    generateClassComponent_C(&ctxC);
    generateAttrComponent_C(&ctxC);

    if (!getUserData(odlGENCODE))
      return Success;

    fprintf(fdh, "class %s : public %s {\n", name, className(parent));

    ctxH.push();

    if (stubs)
      fprintf(fdh, "#include \"%s\"\n", stubs);

#ifndef NO_COMMENTS
    fprintf(fdh, "\n%s// ----------------------------------------------------------------------\n", ctxH.get());
    fprintf(fdh, "%s// %s Interface\n", ctxH.get(), name);
    fprintf(fdh, "%s// ----------------------------------------------------------------------\n", ctxH.get());
#else
    fprintf(fdh, "\n");
#endif
    fprintf(fdh, " public:\n");
  
    ctxC.push();

    generateConstructors_C(&ctxH, &ctxC);

    if (hints.gen_down_casting)
      generateDownCasting_C(&ctxH, m);

    for (i = 0; i < items_cnt; i++)
      {
	if (items[i]->class_owner == (void *)this)
	  {
	    status = items[i]->generateCode_C(this, hints, &ctxH, &ctxC);
	    if (status)
	      return status;
	  }
      }

    ctxC.pop();
    generateMethodBody_C(m, &ctxC, &ctxStubsFe, &ctxStubsBe,
			 &ctxMthFe, &ctxMthBe);
    ctxC.push();

    /* include file */

    generateMethodDecl_C(m, &ctxH);

    fprintf(fdh, "%svirtual ~%s() {garbageRealize();}\n", ctxH.get(),
	    name);

    if (attr_cache)
      fprintf(fdh, "\n%svoid attrCacheEmpty();\n", ctxH.get());

    if (getTiedCode())
      {
#ifndef NO_COMMENTS
	fprintf(fdh, "\n%s// ----------------------------------------------------------------------\n", ctxH.get());
#endif
	fprintf(fdh, "%s// %s User Part\n", ctxH.get(), name);
#ifndef NO_COMMENTS
	fprintf(fdh, "%s// ----------------------------------------------------------------------\n", ctxH.get());
#endif
	fprintf(fdh, "%s\n", getTiedCode());
      }

#ifndef NO_COMMENTS
    fprintf(fdh, "\n%s// ----------------------------------------------------------------------\n", ctxH.get());
    fprintf(fdh, "%s// %s Protected Part\n", ctxH.get(), name);
    fprintf(fdh, "%s// ----------------------------------------------------------------------\n", ctxH.get());
#else
    fprintf(fdh, "\n");
#endif
    fprintf(fdh, " protected:\n");

    if (attr_cache && !isRootClass())
      fprintf(fdh, "%svirtual void garbage();\n", ctxH.get());

    //  fprintf(fdh, "%s%s(Database *_db, int) : ", ctxH.get(), name);
    fprintf(fdh, "%s%s(Database *_db, const Dataspace *_dataspace, int) : ", ctxH.get(), name);

    const_f0(fdh, parent, "_db, _dataspace");

    fprintf(fdh, " {}\n");
    fprintf(fdh, "%s%s(const Struct *x, Bool share, int) : ",
	    ctxH.get(), name);
    const_f01(fdh, parent, "x");
    fprintf(fdh, " {}\n");

    fprintf(fdh, "%s%s(const %s *x, Bool share, int) : ", ctxH.get(), name, name);
    const_f01(fdh, parent, "x");

    fprintf(fdh, " {}\n");

#ifndef NO_COMMENTS
    fprintf(fdh, "\n%s// ----------------------------------------------------------------------\n", ctxH.get());
    fprintf(fdh, "%s// %s Private Part\n", ctxH.get(), name);
    fprintf(fdh, "%s// ----------------------------------------------------------------------\n", ctxH.get());
#else
    fprintf(fdh, "\n");
#endif

    fprintf(fdh, " private:\n", ctxH.get());
  
    fprintf(fdh, "%svoid initialize(Database *_db);\n", ctxH.get());

    if (attr_cache)
      {
	fprintf(fdh, "\n");
	int i;
	for (i = 0; i < items_cnt; i++)
	  if (items[i]->getClassOwner()->compare(this))
	    items[i]->genAttrCacheDecl(&ctxH);
      }

    fprintf(fdh, "\n public: /* restricted */\n", ctxH.get());
    fprintf(fdh, "%s%s(const Struct *, Bool = False);\n",
	    ctxH.get(), name);
    fprintf(fdh, "%s%s(const %s *, Bool = False);\n",
	    ctxH.get(), name, name);
    fprintf(fdh, "%s%s(const Class *, Data);\n", ctxH.get(), name);

    fprintf(fdh, "};\n\n");

    ctxH.pop();
    ctxC.pop();

    return Success;
  }

  Status EnumClass::generateClassDesc_C(GenContext *ctx)
  {
    FILE *fd = ctx->getFile();
    Status status;

    fprintf(fd, "static Size %s_idr_objsz, %s_idr_psize;\n\n", name, name);

    fprintf(fd, "static EnumClass *%s_make(EnumClass *%s_class = 0, Schema *m = 0)\n{\n", name, name);

    ctx->push();
    fprintf(fd, "%sif (!%s_class)\n", ctx->get(), name);
    fprintf(fd, "%s  return new EnumClass(\"%s\");\n", ctx->get(), getAliasName());

    fprintf(fd, "%sEnumItem *en[%d];\n", ctx->get(), items_cnt);

    int i;
    for (i = 0; i < items_cnt; i++)
      fprintf(fd, "%sen[%d] = new EnumItem(\"%s\", \"%s\", (unsigned int)%d);\n",
	      ctx->get(),
	      i, items[i]->getName(), items[i]->getAliasName(),
	      items[i]->getValue());
  
    fprintf(fd, "\n%s%s_class->setEnumItems(en, %d);\n", ctx->get(),
	    name, items_cnt);

    fprintf(fd, "\n");
    for (i = 0; i < items_cnt; i++)
      fprintf(fd, "%sdelete en[%d];\n", ctx->get(), i);
    fprintf(fd, "\n");

    if (isSystem() || odl_system_class)
      fprintf(fd, "%sClassPeer::setMType(%s_class, Class::System);\n",
	      ctx->get(), name);

    fprintf(fd, "\n%sreturn %s_class;\n}\n\n", ctx->get(), name);

    fprintf(fd, "static void %s_init_p()\n{\n", name);
    fprintf(fd, "%s%s%s = %s_make();\n", ctx->get(), name, classSuffix, name);
    fprintf(fd, "}\n\n");

    fprintf(fd, "static void %s_init()\n{\n", name);
    fprintf(fd, "%s%s_make(%s%s);\n\n", ctx->get(), name, name, classSuffix);

    fprintf(fd, "%s%s_idr_objsz = %s%s->getIDRObjectSize(&%s_idr_psize, 0);\n\n",
	    ctx->get(), name, name, classSuffix, name);

    fprintf(fd, "%sObjectPeer::setUnrealizable(%s%s, True);\n",
	    ctx->get(), name, classSuffix);
    fprintf(fd, "}\n\n");
    ctx->pop();

    return Success;
  }

  Status
  EnumClass::generateCode_C(Schema *,
			    const char *prefix,
			    const GenCodeHints &hints,
			    const char *stubs,
			    FILE *fdh, FILE *fdc, FILE *,
			    FILE *, FILE *, FILE *)
  {
    GenContext ctxH(fdh), ctxC(fdc);
    int i;
    Status status;

    phints = &hints;
    class_enums = hints.class_enums;
    attr_cache  = hints.attr_cache;

    generateClassDesc_C(&ctxC);

    ctxH.push();
    ctxC.push();

    //  fprintf(fdh, "\n");

    const char *indent_str;

    if (class_enums) {
      fprintf(fdh, "class %s {\n", name);
      fprintf(fdh, "\npublic:\n");
      fprintf(fdh, "  enum %s {\n", enum_type);
      indent_str = "    ";
    }
    else {
      fprintf(fdh, "enum %s {\n", name);
      indent_str = "  ";
    }

    EnumItem **it;
    for (i = 0, it = items; i < items_cnt; i++, it++)
      {
	if (i) fprintf(fdh, ",\n");
	fprintf(fdh, "%s%s = %d", indent_str, 
		(*it)->getAliasName(), (*it)->getValue());
      }

    if (class_enums)
      fprintf(fdh, "\n  };");

    fprintf(fdh, "\n};\n\n");

    ctxH.pop();
    ctxC.pop();

    return Success;
  }

  Status CollectionClass::generateCode_C(Schema *,
					 const char *prefix,
					 const GenCodeHints &hints,
					 const char *stubs,
					 FILE *fdh,
					 FILE *fdc, FILE *, FILE *,
					 FILE *, FILE *)
  {
    GenContext ctxH(fdh), ctxC(fdc);
    int i;
    Status status;
    const char *c_name = getCName();
    const char *_type = getCSuffix();

    phints = &hints;
    class_enums = hints.class_enums;
    attr_cache  = hints.attr_cache;

    fprintf(fdc, "static " DEF_PREFIX "%sClass *%s_make(" DEF_PREFIX "%sClass *cls = 0, Schema *m = 0)\n{\n", _type, c_name, _type);
    ctxC.push();

    fprintf(fdc, "%sif (!cls)\n%s  {\n", ctxC.get(), ctxC.get());

    fprintf(fdc, "%s    cls = new " DEF_PREFIX "%sClass((m ? m->getClass(\"%s\") : %s%s), ",
	    ctxC.get(), _type, coll_class->getAliasName(), // changed 27/04/00
	    (coll_class->asCollectionClass() ? coll_class->getCName() :
	     className(coll_class)), classSuffix);

    if (dim > 1)
      fprintf(fdc, "%d);\n", dim);
    else
      fprintf(fdc, "%s);\n", (isref ? "True" : "False"));

    fprintf(fdc, "%s    ClassPeer::setMType(cls, Class::System);\n",
	    ctxC.get());
    fprintf(fdc, "%s  }\n", ctxC.get());

    fprintf(fdc, "%sreturn cls;\n", ctxC.get());
    fprintf(fdc, "}\n\n");

    fprintf(fdc, "static void %s_init_p()\n{\n", c_name);
    fprintf(fdc, "%s%s%s = %s_make();\n", ctxC.get(), c_name, classSuffix, c_name);
    fprintf(fdc, "}\n\n");

    return Success;
  }
}
  
