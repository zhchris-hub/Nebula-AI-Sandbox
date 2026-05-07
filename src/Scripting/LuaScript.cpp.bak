#include "Scripting/LuaScript.h"
#include "Core/Log.h"
#include <fstream>
#include <sstream>

namespace nebula {

LuaScript::LuaScript() {
    m_State.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string, sol::lib::table);
    NEBULA_INFO("LuaScript created");
}

LuaScript::~LuaScript() {
    NEBULA_INFO("LuaScript destroyed");
}

bool LuaScript::LoadFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        NEBULA_ERROR("Failed to open Lua script file: {0}", filepath);
        return false;
    }

    std::stringstream ss;
    ss << file.rdbuf();
    m_Source = ss.str();
    m_Loaded = true;
    NEBULA_INFO("Loaded Lua script from: {0}", filepath);
    return true;
}

bool LuaScript::LoadFromString(const std::string& code) {
    m_Source = code;
    m_Loaded = true;
    NEBULA_INFO("Loaded Lua script from string");
    return true;
}

bool LuaScript::Execute() {
    if (!m_Loaded) {
        NEBULA_ERROR("No Lua script loaded");
        return false;
    }

    sol::protected_function_result result = m_State.safe_script(m_Source, sol::script_pass_on_error);
    if (!result.valid()) {
        sol::error err = result;
        NEBULA_ERROR("Lua script execution error: {0}", err.what());
        return false;
    }

    NEBULA_INFO("Lua script executed successfully");
    return true;
}

sol::protected_function_result LuaScript::CallFunction(const std::string& funcName) {
    sol::protected_function func = m_State[funcName];
    if (!func.valid()) {
        NEBULA_ERROR("Lua function not found: {0}", funcName);
        return sol::protected_function_result();
    }

    sol::protected_function_result result = func();
    if (!result.valid()) {
        sol::error err = result;
        NEBULA_ERROR("Lua function call error: {0}", err.what());
    }

    return result;
}

} // namespace nebula
