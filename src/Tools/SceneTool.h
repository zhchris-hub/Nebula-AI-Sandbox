#pragma once

#include "Tools/ITool.h"
#include "Scene/Scene.h"

namespace nebula {

class SceneTool : public ITool {
public:
    SceneTool(Scene* scene);

    const std::string& GetName() const override;
    const std::string& GetDescription() const override;
    nlohmann::json GetParameterSchema() const override;
    ToolResult Execute(const nlohmann::json& params) override;

private:
    Scene* m_Scene;

    ToolResult CreateEntity(const nlohmann::json& params);
    ToolResult DestroyEntity(const nlohmann::json& params);
    ToolResult GetSceneInfo(const nlohmann::json& params);
    ToolResult GetEntityInfo(const nlohmann::json& params);
    ToolResult SetEntityTransform(const nlohmann::json& params);
    ToolResult SetEntityColor(const nlohmann::json& params);
};

} // namespace nebula
