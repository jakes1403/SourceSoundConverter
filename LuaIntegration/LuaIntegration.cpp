/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

 // Copyright 2019-2020 jakers1403

#include "LuaIntegration.h"

lua_State* state = luaL_newstate();

void LI_PrintError()
{
	// The error message is on top of the stack.
	// Fetch it, print it and then pop it off the stack.
	const char* message = lua_tostring(state, -1);
	printf(message);
	lua_pop(state, 1);
}

std::string LI_GetStringVarValue(const char* varName)
{
	lua_getglobal(state, varName);
	std::string varValue = lua_tostring(state, 1);
	lua_pop(state, 1);
	return varValue;
}

void LI_SetStringVarValue(const char* varName, std::string varValue)
{
	lua_pushstring(state, varValue.c_str());
	lua_setglobal(state, varName);
}

void LI_SetBoolVarValue(const char* varName, bool varValue)
{
	lua_pushboolean(state, varValue);
	lua_setglobal(state, varName);
}

void LI_SetFloatVarValue(const char* varName, float varValue)
{
	lua_pushnumber(state, varValue);
	lua_setglobal(state, varName);
}
void LI_SetIntVarValue(const char* varName, int varValue)
{
	lua_pushinteger(state, varValue);
	lua_setglobal(state, varName);
}

bool LI_GetBoolVarValue(const char* varName)
{
	lua_getglobal(state, varName);
	bool varValue = lua_toboolean(state, 1);
	lua_pop(state, 1);
	return varValue;
}

int LI_GetIntVarValue(const char* varName)
{
	lua_getglobal(state, varName);
	int varValue = lua_tointeger(state, 1);
	lua_pop(state, 1);
	return varValue;
}

float LI_GetFloatVarValue(const char* varName)
{
	lua_getglobal(state, varName);
	float varValue = lua_tonumber(state, 1);
	lua_pop(state, 1);
	return varValue;
}

void LI_RunFunction(const char* funcName)
{
	/* the function name */
	lua_getglobal(state, funcName);

	/* call the function with no arguments, return no results */
	int result = lua_pcall(state, 0, LUA_MULTRET, 0);

	if (result != LUA_OK) {
		LI_PrintError;
		return;
	}
}

void LI_Init()
{
	// Make standard libraries available in the Lua object
	luaL_openlibs(state);
}

void LI_ExecuteLua(const char* filename)
{
	int result;

	// Load the program; this supports both source code and bytecode files.
	result = luaL_loadfile(state, filename);

	if (result != LUA_OK) {
		LI_PrintError();
		return;
	}

	// Finally, execute the program by calling into it.
	// Change the arguments if you're not running vanilla Lua code.

	result = lua_pcall(state, 0, LUA_MULTRET, 0);

	if (result != LUA_OK) {
		LI_PrintError;
		return;
	}
}