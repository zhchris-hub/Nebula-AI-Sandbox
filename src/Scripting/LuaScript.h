#pragma once

#include <sol/sol.hpp>
#include <string>
#include <vector>

namespace nebula {

class LuaScript {
public:
    LuaScript();
    ~LuaScript();

    // Load a script from file
    bool LoadFromFile(const std::string& filepath);

    // Load a script from string
    bool LoadFromString(const std::string& code);

    // Execute the loaded script
    bool Execute();

    // Call a specific function in the script
    sol::protected_function_result CallFunction(const std::string& funcName);

    // Get/set global variables
    template<typename T>
    T GetGlobal(const std::string& name) { return m_State[name].get<T>(); }

    template<typename T>
    void SetGlobal(const std::string& name, T value) { m_State[name] = value; }

    // Register a C++ function for Lua to call
    template<typename Func>
    void RegisterFunction(const std::string& name, Func func) { m_State[name] = func; }

    // Check if script is loaded
    bool IsLoaded() const { return m_Loaded; }

    // Get the Lua state
    sol::state& GetState() { return m_State; }
    const sol::state& GetState() const { return m_State; }

private:
    sol::state m_State;
    bool m_Loaded = false;
    std::string m_Source;
};

} // namespace nebula
