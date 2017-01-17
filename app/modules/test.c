/* 
 * http://blog.mclemon.io/esp8266-contributing-to-the-nodemcu-ecosystem
 *
 * test.c - simple lua for playing about with
 */


#include "module.h"
#include "lauxlib.h"
#include "platform.h"


// test.identity() - takes a single value, returns it
static int test_identity(lua_State *L) {  
  return 1;
}



// Module function map
static const LUA_REG_TYPE test_map[] = {
  { LSTRKEY( "identity" ),         LFUNCVAL( test_identity ) },
  { LSTRKEY( "__metatable" ),      LROVAL( test_map ) },
  { LNILKEY, LNILVAL }
};


// Define an empty test function - we won't use this functionality
int luaopen_test( lua_State *L ) {
  return 0;
}


// Register the module - NODEMCU_MODULE() will make sure a module called "test" 
// is available when we define the LUA_USE_MODULES_TEST macro in user_modules.h
NODEMCU_MODULE(TEST, "test", test_map, luaopen_test);

