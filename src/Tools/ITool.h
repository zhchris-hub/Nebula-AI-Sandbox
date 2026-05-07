#pragma once

#include "Tools/ToolResult.h"
#include <nlohmann/json.hpp>
#include <string>

namespace nebula {

class ITool {
public:
    virtual ~ITool() = default;

    // Tool name (unique identifier)
    virtual const std::string& GetName() const = 0;

    // Tool description for AI
    virtual const std::string& GetDescription() const = 0;

    // JSON schema for parameters
    virtual nlohmann::json GetParameterSchema() const = 0;

    // Execute the tool
    virtual ToolResult Execute(const nlohmann::json& params) = 0;
};

} // namespace nebula
