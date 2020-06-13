/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

 // Copyright 2019-2020 jakers1403

// Header for LuaIntegration library

#pragma once

#include <lua.hpp>
#include <string>

extern lua_State* state;

extern std::string LI_GetStringVarValue(const char* varName);

extern void LI_SetStringVarValue(const char* varName, std::string varValue);

extern void LI_SetBoolVarValue(const char* varName, bool varValue);

extern void LI_SetFloatVarValue(const char* varName, float varValue);

extern void LI_SetIntVarValue(const char* varName, int varValue);

extern bool LI_GetBoolVarValue(const char* varName);

extern int LI_GetIntVarValue(const char* varName);

extern float LI_GetFloatVarValue(const char* varName);

extern void LI_RunFunction(const char* funcName);

extern void LI_Init();

extern void LI_ExecuteLua(const char* filename);

