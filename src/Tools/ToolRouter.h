#pragma once

#include "Tools/ITool.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace nebula {

class ToolRouter {
public:
    ToolRouter();
    ~ToolRouter();

    // Register a tool
    void RegisterTool(std::shared_ptr<ITool> tool);

    // Execute a tool by name
    ToolResult ExecuteTool(const std::string& toolName, const nlohmann::json& params);

    // Get all registered tools (for AI context)
    nlohmann::json GetToolList() const;

    // Get tool count
    size_t GetToolCount() const { return m_Tools.size(); }

private:
    std::unordered_map<std::string, std::shared_ptr<ITool>> m_Tools;
};

} // namespace nebula
