#include "php_eyedb.h"
/* declaration of functions to be exported */
/* compiled function list so Zend knows what's in this module */
zend_function_entry eyedb_functions[] = {
   ZEND_FE(eyedb_test, NULL)
   ZEND_FE(eyedb_connect, NULL)
	{NULL, NULL, NULL}
};    

/* compiled module information */
zend_module_entry eyedb_module_entry = {
    STANDARD_MODULE_HEADER,
    "eyedb",
    eyedb_functions,
    NULL, NULL, NULL, NULL, NULL,
    NO_VERSION_YET, STANDARD_MODULE_PROPERTIES
};    

/* implement standard "stub" routine to introduce ourselves to Zend */
ZEND_GET_MODULE(eyedb)
/* DoubleUp function */
/* This method takes 1 parameter, a long value, returns
   the value multiplied by 2 */
ZEND_FUNCTION(eyedb_test){
  RETURN_STRING("Module eyedb correctement installé!!!",1);
}

ZEND_FUNCTION(eyedb_connect){
	zend_printf("eyedb_connect debut<br>");
	eyedb::init();
	RETURN_STRING("Exec avec succes",1);
}
