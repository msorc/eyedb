require 'mkmf'
CC="g++"
CPP="g++ -shared"


EYEDB_CXXFLAGS="-fPIC -pthread"
EYEDB_CPPFLAGS="-D_REENTRANT -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64"

Config::CONFIG['CPP']= 'g++ -shared'
Config::CONFIG['CC'] = "g++ -shared"
CONFIG['CC'] = "g++ -shared"
CONFIG['LDSHARED'] = "g++ -shared"
#CONFIG['DLDFLAGS'] += "#{EYEDB_LDLIBS} #{EYEDB_LDFLAGS}"
CONFIG['CXXFLAGS'] = "#{EYEDB_CXXFLAGS} #{EYEDB_CPPFLAGS}"
have_library("eyedb",nil,"eyedb/eyedb.h");
have_library("eyedbrpcfe");
have_library("eyedbsm");
have_library("eyedbutils");
have_library("pthread");
have_library("crypt");
have_library("dl");
have_library("nsl");
have_header("eyedb/eyedb.h")


create_makefile("eyedb")
