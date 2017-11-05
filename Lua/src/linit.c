/*
** $Id: linit.c,v 1.39 2016/12/04 20:17:24 roberto Exp $
** Initialization of libraries for lua.c and other clients
** See Copyright Notice in lua.h
*/


#define linit_c
#define LUA_LIB

/*
** If you embed Lua in your program and need to open the standard
** libraries, call luaL_openlibs in your program. If you need a
** different set of libraries, copy this file to your project and edit
** it to suit your needs.
**
** You can also *preload* libraries, so that a later 'require' can
** open the library, which is already linked to the application.
** For that, do the following code:
**
**  luaL_getsubtable(L, LUA_REGISTRYINDEX, LUA_PRELOAD_TABLE);
**  lua_pushcfunction(L, luaopen_modname);
**  lua_setfield(L, -2, modname);
**  lua_pop(L, 1);  // remove PRELOAD table
*/

#include "lprefix.h"


#include <stddef.h>

#include "lua.h"

#include "lualib.h"
#include "lauxlib.h"

#include "drivers/i2c-platform.h"


#define DEF_SDA 4
#define DEF_SCL 5


#if !LUA_USE_ROTABLE
/*
** these libs are loaded by lua.c and are readily available to any Lua
** program
*/
static const luaL_Reg loadedlibs[] = {
  {"_G", luaopen_base},
  {LUA_LOADLIBNAME, luaopen_package},
  {LUA_COLIBNAME, luaopen_coroutine},
  {LUA_TABLIBNAME, luaopen_table},
  {LUA_IOLIBNAME, luaopen_io},
  {LUA_OSLIBNAME, luaopen_os},
  {LUA_STRLIBNAME, luaopen_string},
  {LUA_MATHLIBNAME, luaopen_math},
  {LUA_UTF8LIBNAME, luaopen_utf8},
  {LUA_DBLIBNAME, luaopen_debug},
#if defined(LUA_COMPAT_BITLIB)
  {LUA_BITLIBNAME, luaopen_bit32},
#endif
  {NULL, NULL}
};


LUALIB_API void luaL_openlibs (lua_State *L) {
  const luaL_Reg *lib;
  /* "require" functions from 'loadedlibs' and set results to global table */
  for (lib = loadedlibs; lib->func; lib++) {
    luaL_requiref(L, lib->name, lib->func, 1);
    lua_pop(L, 1);  /* remove lib */
  }
}
#else
#include "lrotable.h"
#include "modules.h"
#include "lgc.h"
#include <sys/debug.h>

extern const luaL_Reg lua_libs1[];

MODULE_REGISTER_UNMAPPED(_G, _G, luaopen_base);
//MODULE_REGISTER_UNMAPPED(IO, io, luaopen_io);
//MODULE_REGISTER_UNMAPPED(PACKAGE, package, luaopen_package);


LUALIB_API void luaL_openlibs (lua_State *L) {
  const luaL_Reg *lib = lua_libs1;


  for (; lib->name; lib++) {
    if (lib->func) {
  		debug_free_mem_begin(luaL_openlibs);

		
		uint8_t sda = DEF_SDA;
		uint8_t scl = DEF_SCL;
		
		platform_i2c_setup(0, sda, scl, 100);		

		#include "user_modules.inc"
		
		#if LUA_USE_ROTABLE
		//if (luaR_findglobal(lib->name)) {
	    //      lua_pushcfunction(L, lib->func);
	    //      lua_pushstring(L, lib->name);
	    //      lua_call(L, 1, 0);
		//} else {
			luaL_requiref(L, lib->name, lib->func, 1);
			lua_pop(L, 1);  /* remove lib */
		//}
		#else
			luaL_requiref(L, lib->name, lib->func, 1);
			lua_pop(L, 1);  /* remove lib */
		#endif
		
		#if DEBUG_FREE_MEM
		luaC_fullgc(L, 1);
		#endif
	  	debug_free_mem_end(luaL_openlibs, lib->name);
    }
  }

  luaC_fullgc(L, 1);
}
#endif
