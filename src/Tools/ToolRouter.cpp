#include "Tools/ToolRouter.h"
#include "Core/Log.h"

namespace nebula {

ToolRouter::ToolRouter() {
    NEBULA_INFO("ToolRouter created");
}

ToolRouter::~ToolRouter() {
    NEBULA_INFO("ToolRouter destroyed");
}

void ToolRouter::RegisterTool(std::shared_ptr<ITool> tool) {
    if (m_Tools.find(tool->GetName()) != m_Tools.end()) {
        NEBULA_WARN("Tool already registered, overwriting: {0}", tool->GetName());
    }
    m_Tools[tool->GetName()] = tool;
    NEBULA_INFO("Registered tool: {0}", tool->GetName());
}

ToolResult ToolRouter::ExecuteTool(const std::string& toolName, const nlohmann::json& params) {
    auto it = m_Tools.find(toolName);
    if (it == m_Tools.end()) {
        NEBULA_ERROR("Tool not found: {0}", toolName);
        return ToolResult::Fail("Tool not found: " + toolName);
    }

    NEBULA_INFO("Executing tool: {0}", toolName);
    return it->second->Execute(params);
}

nlohmann::json ToolRouter::GetToolList() const {
    nlohmann::json tools = nlohmann::json::array();
    for (const auto& [name, tool] : m_Tools) {
        tools.push_back({
            {"name", tool->GetName()},
            {"description", tool->GetDescription()},
            {"parameters", tool->GetParameterSchema()}
        });
    }
    return tools;
}

} // namespace nebula
