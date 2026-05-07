#include "Tools/ScriptingTool.h"
#include "Core/Log.h"

namespace nebula {

static const std::string TOOL_NAME = "scripting";
static const std::string TOOL_DESC = "Lua scripting: load and execute Lua scripts";

ScriptingTool::ScriptingTool() {
    NEBULA_INFO("ScriptingTool created");
}

const std::string& ScriptingTool::GetName() const { return TOOL_NAME; }
const std::string& ScriptingTool::GetDescription() const { return TOOL_DESC; }

nlohmann::json ScriptingTool::GetParameterSchema() const {
    return {
        {"type", "object"},
        {"properties", {
            {"action", {"type", "string", "enum", {"load", "execute", "run_code"}}},
            {"filepath", {"type", "string"}},
            {"code", {"type", "string"}}
        }},
        {"required", {"action"}}
    };
}

ToolResult ScriptingTool::Execute(const nlohmann::json& params) {
    if (!params.contains("action")) {
        return ToolResult::Fail("Missing 'action' parameter");
    }

    std::string action = params["action"].get<std::string>();

    if (action == "load") return LoadScript(params);
    if (action == "execute") return ExecuteScript(params);
    if (action == "run_code") return RunCode(params);

    return ToolResult::Fail("Unknown action: " + action);
}

ToolResult ScriptingTool::LoadScript(const nlohmann::json& params) {
    if (!params.contains("filepath")) {
        return ToolResult::Fail("Missing 'filepath' parameter");
    }

    std::string filepath = params["filepath"].get<std::string>();
    if (m_Script.LoadFromFile(filepath)) {
        return ToolResult::Ok("Script loaded: " + filepath);
    }
    return ToolResult::Fail("Failed to load script: " + filepath);
}

ToolResult ScriptingTool::ExecuteScript(const nlohmann::json& params) {
    (void)params;

    if (!m_Script.IsLoaded()) {
        return ToolResult::Fail("No script loaded");
    }

    if (m_Script.Execute()) {
        return ToolResult::Ok("Script executed successfully");
    }
    return ToolResult::Fail("Script execution failed");
}

ToolResult ScriptingTool::RunCode(const nlohmann::json& params) {
    if (!params.contains("code")) {
        return ToolResult::Fail("Missing 'code' parameter");
    }

    std::string code = params["code"].get<std::string>();
    if (m_Script.LoadFromString(code) && m_Script.Execute()) {
        return ToolResult::Ok("Code executed successfully");
    }
    return ToolResult::Fail("Code execution failed");
}

} // namespace nebula
