#pragma once

#include "Tools/ITool.h"
#include "Scripting/LuaScript.h"

namespace nebula {

class ScriptingTool : public ITool {
public:
    ScriptingTool();

    const std::string& GetName() const override;
    const std::string& GetDescription() const override;
    nlohmann::json GetParameterSchema() const override;
    ToolResult Execute(const nlohmann::json& params) override;

private:
    LuaScript m_Script;

    ToolResult LoadScript(const nlohmann::json& params);
    ToolResult ExecuteScript(const nlohmann::json& params);
    ToolResult RunCode(const nlohmann::json& params);
};

} // namespace nebula
