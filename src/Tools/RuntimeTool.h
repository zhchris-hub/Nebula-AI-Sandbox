#pragma once

#include "Tools/ITool.h"
#include "Scene/Scene.h"

namespace nebula {

class RuntimeTool : public ITool {
public:
    RuntimeTool(Scene* scene);

    const std::string& GetName() const override;
    const std::string& GetDescription() const override;
    nlohmann::json GetParameterSchema() const override;
    ToolResult Execute(const nlohmann::json& params) override;

private:
    Scene* m_Scene;

    ToolResult SetGravity(const nlohmann::json& params);
    ToolResult GetPhysicsInfo(const nlohmann::json& params);
    ToolResult AddPhysicsBody(const nlohmann::json& params);
};

} // namespace nebula
