
/*
 * plugin.cc
 *
 */

#include <iostream>
#include "schema.h"

namespace eyedb {

  class GenerationContext {

  public: 
    GenerationContext(const Schema *schema,
		      const std::string &odlfile,
		      const std::string &package,
		      const std::string &plugin_args) :
      schema(schema),
      odlfile(odlfile),
      package(package),
      plugin_args(plugin_args) { }

    const Schema *getSchema() const {return schema;}
    const std::string &getODLFile() const {return odlfile;}
    const std::string &getPackage() const {return package;}
    const std::string &getPluginArgs() const {return plugin_args;}

  private:
    const Schema *schema;
    std::string odlfile;
    std::string package;
    std::string plugin_args;
  };

  class GeneratorEngine {

    static std::map<std::string, GeneratorEngine *> map;
    std::string name;

  protected:
    GeneratorEngine(const std::string &name) : name(name) {
      map[name] = this;
    }

  public:
    const std::string& getName() const {return name;}

    virtual GenerationContext& create_context
    (const Schema *schema,
     const std::string &odlfile,
     const std::string &package,
     const std::string &plugin_args) const {
      return *new GenerationContext(schema, odlfile, package, plugin_args);
    }

    static GeneratorEngine *get(const std::string &name) {
      if (map.find(name) == map.end())
	return 0;
      return map[name];
    }

    virtual void generate(GenerationContext &ctx) const {
      generate_init(ctx);
      generate_schema(ctx);
    }

    virtual void generate_init(GenerationContext &ctx) const {
      generate_init_schema_begin(ctx, ctx.getSchema());

      LinkedListCursor c(ctx.getSchema()->getClassList());
      const Class *cls;
      while (c.getNext((void *&)cls)) {
	generate_init_class(ctx, cls);
      }

      generate_init_schema_end(ctx, ctx.getSchema());
    }

    virtual void generate_init_schema_begin(GenerationContext &ctx, const Schema *schema) const = 0;
    virtual void generate_init_schema_end(GenerationContext &ctx, const Schema *schema) const = 0;

    virtual void generate_init_class(GenerationContext &ctx, const Class *cls) const = 0;

    virtual void generate_schema(GenerationContext &ctx) const {
      generate_schema_begin(ctx, ctx.getSchema());

      LinkedListCursor c(ctx.getSchema()->getClassList());
      const Class *cls;
      while (c.getNext((void *&)cls)) {
	generate_class(ctx, cls);
      }

      generate_schema_end(ctx, ctx.getSchema());
    }

    virtual void generate_class(GenerationContext &ctx, const Class *cls) const {
      generate_class_begin(ctx, cls);

      unsigned int attr_cnt;
      const Attribute **attrs = cls->getAttributes(attr_cnt);

      for (unsigned int n = 0; n < attr_cnt; n++) {
	const Attribute *attr = attrs[n];
	const Class *attr_cls = attr->getClass();

	if (attr->isString())
	  generate_string_attribute(ctx, attr);
	else if (attr_cls->asBasicClass() && !attr->isIndirect())
	  generate_basic_attribute(ctx, attr);
	else if (attr_cls->asCollectionClass() && attr->isIndirect())
	  generate_object_collection_attribute(ctx, attr);
	else if (attr_cls->asCollectionClass() && !attr->isIndirect())
	  generate_literal_collection_attribute(ctx, attr);
	else if (attr->isIndirect())
	  generate_object_attribute(ctx, attr);
	else
	  generate_literal_attribute(ctx, attr);

	generate_general_attribute(ctx, attr);
      }

      generate_class_end(ctx, cls);
    }

    virtual void generate_schema_begin(GenerationContext &ctx, const Schema *sch) const = 0;
    virtual void generate_schema_end(GenerationContext &ctx, const Schema *sch) const = 0;

    virtual void generate_class_begin(GenerationContext &ctx, const Class *cls) const = 0;
    virtual void generate_class_end(GenerationContext &ctx, const Class *cls) const = 0;

    virtual void generate_general_attribute(GenerationContext &ctx, const Attribute *attr) const = 0;

    virtual void generate_string_attribute(GenerationContext &ctx, const Attribute *attr) const = 0;

    virtual void generate_basic_attribute(GenerationContext &ctx, const Attribute *attr) const = 0;

    virtual void generate_object_collection_attribute(GenerationContext &ctx, const Attribute *attr) const = 0;

    virtual void generate_literal_collection_attribute(GenerationContext &ctx, const Attribute *attr) const = 0;
    

    virtual void generate_object_attribute(GenerationContext &ctx, const Attribute *attr) const = 0;

    virtual void generate_literal_attribute(GenerationContext &ctx, const Attribute *attr) const { }

    virtual void generate_class_constructor(GenerationContext &ctx, const Attribute *attr) const = 0;

    virtual void generate_addto_coll(GenerationContext &ctx, const Attribute *attr) const = 0;

    virtual void generate_rmvfrom_coll(GenerationContext &ctx, const Attribute *attr) const = 0;

    virtual void generate_setcoll(GenerationContext &ctx, const Attribute *attr) const = 0;
    virtual void generate_getcoll(GenerationContext &ctx, const Attribute *attr) const = 0;

    virtual void generate_getat_coll(GenerationContext &ctx, const Attribute *attr) const = 0;
    virtual void generate_getoidat_coll(GenerationContext &ctx, const Attribute *attr) const = 0;

    virtual void generate_get(GenerationContext &ctx, const Attribute *attr) const = 0;
    virtual void generate_set(GenerationContext &ctx, const Attribute *attr) const = 0;

    virtual void generate_getoid(GenerationContext &ctx, const Attribute *attr) const = 0;
    virtual void generate_setoid(GenerationContext &ctx, const Attribute *attr) const = 0;

    virtual void generate_getsize(GenerationContext &ctx, const Attribute *attr) const = 0;
    virtual void generate_setsize(GenerationContext &ctx, const Attribute *attr) const = 0;
  };

  class GeneratorEngineWrapper : public GeneratorEngine {
  public:
    // redefine all pure virtual methods with null body
  };
}

// what we do when we need several pass on the classes
static int usage(const char *prog) {
  std::cerr << "usage: " << prog << " <dbname>" << std::endl;
  return 1;
}

int main(int argc, char *argv[])
{
  schema initializer(argc, argv);

  if (argc < 2) {
    return usage(argv[0]);
  }

  eyedb::Exception::setMode(eyedb::Exception::ExceptionMode);

  try {
    eyedb::Connection conn(true);

    schemaDatabase db(&conn, argv[1], eyedb::Database::DBRW);

    db.transactionBegin();

    db.transactionCommit();

    db.close();
  }
  catch(eyedb::Exception &e) {
    std::cerr << e << std::endl;
    return 1;
  }


  return 0;
}
