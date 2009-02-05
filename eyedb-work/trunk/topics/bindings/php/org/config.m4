/*
 *  config.m4
 *  
 *
 *  Created by Jean-Marc Fiaschi on 02/01/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
PHP_ARG_ENABLE(eyedb, whether to enable eyedb support,[ --enable-eyedb Enable eyedb support]) 

if test "$PHP_EYEDB" = "yes"; then
	PHP_REQUIRE_CXX #compilation c++   phpize --help
	#init
	EYEDB_DIR="/Users/jmf/eyedb" #Chemin vers la lib eyedb
	
	PHP_CHECK_LIBRARY(eyedb,_ZnwmPv, 
	[
		PHP_ADD_INCLUDE($EYEDB_DIR/include)
		PHP_ADD_LIBRARY_WITH_PATH(eyedb, $EYEDB_DIR/$PHP_LIBDIR, EYEDB_SHARED_LIBADD)
		AC_DEFINE(HAVE_EYEDB,1,[ ])
	], [
		AC_MSG_ERROR(Lib eyedb non trouve!!!)
	], [
		-L$EYEDB_DIR/$PHP_LIBDIR
	])
	PHP_NEW_EXTENSION(eyedb, php_eyedb.cc, $ext_shared)
	PHP_SUBST(EYEDB_SHARED_LIBADD)
fi
