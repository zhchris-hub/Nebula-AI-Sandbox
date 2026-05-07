#include "Tools/RuntimeTool.h"
#include "Core/Log.h"

namespace nebula {

static const std::string TOOL_NAME = "runtime";
static const std::string TOOL_DESC = "Runtime control: physics simulation, gravity, physics bodies";

RuntimeTool::RuntimeTool(Scene* scene) : m_Scene(scene) {}

const std::string& RuntimeTool::GetName() const { return TOOL_NAME; }
const std::string& RuntimeTool::GetDescription() const { return TOOL_DESC; }

nlohmann::json RuntimeTool::GetParameterSchema() const {
    return {
        {"type", "object"},
        {"properties", {
            {"action", {"type", "string", "enum", {"set_gravity", "get_physics_info", "add_physics_body"}}},
            {"gravity", {"type", "object", "properties", {{"x", {"type", "number"}}, {"y", {"type", "number"}}}}},
            {"entity_id", {"type", "integer"}},
            {"is_static", {"type", "boolean"}},
            {"density", {"type", "number"}},
            {"friction", {"type", "number"}},
            {"restitution", {"type", "number"}}
        }},
        {"required", {"action"}}
    };
}

ToolResult RuntimeTool::Execute(const nlohmann::json& params) {
    if (!params.contains("action")) {
        return ToolResult::Fail("Missing 'action' parameter");
    }

    std::string action = params["action"].get<std::string>();

    if (action == "set_gravity") return SetGravity(params);
    if (action == "get_physics_info") return GetPhysicsInfo(params);
    if (action == "add_physics_body") return AddPhysicsBody(params);

    return ToolResult::Fail("Unknown action: " + action);
}

ToolResult RuntimeTool::SetGravity(const nlohmann::json& params) {
    if (!params.contains("gravity")) {
        return ToolResult::Fail("Missing 'gravity' parameter");
    }

    float x = params["gravity"].value("x", 0.0f);
    float y = params["gravity"].value("y", -9.8f);

    m_Scene->SetGravity({x, y});
    return ToolResult::Ok("Gravity set", {{"x", x}, {"y", y}});
}

ToolResult RuntimeTool::GetPhysicsInfo(const nlohmann::json& params) {
    (void)params;

    auto& world = m_Scene->GetPhysicsWorld();
    b2Vec2 gravity = world.GetWorld().GetGravity();

    int bodyCount = 0;
    for (auto& entity : m_Scene->GetEntities()) {
        if (m_Scene->HasRigidBodyComponent(entity)) {
            bodyCount++;
        }
    }

    return ToolResult::Ok("Physics info retrieved", {
        {"gravity", {{"x", gravity.x}, {"y", gravity.y}}},
        {"body_count", bodyCount}
    });
}

ToolResult RuntimeTool::AddPhysicsBody(const nlohmann::json& params) {
    if (!params.contains("entity_id")) {
        return ToolResult::Fail("Missing 'entity_id' parameter");
    }

    uint32_t id = params["entity_id"].get<uint32_t>();

    for (auto& entity : m_Scene->GetEntities()) {
        if (entity.GetID() == id) {
            if (m_Scene->HasRigidBodyComponent(entity)) {
                return ToolResult::Fail("Entity already has physics body");
            }

            bool isStatic = params.value("is_static", false);
            float density = params.value("density", 1.0f);
            float friction = params.value("friction", 0.3f);
            float restitution = params.value("restitution", 0.2f);

            m_Scene->AddRigidBodyComponent(entity, isStatic, density, friction, restitution);
            return ToolResult::Ok("Physics body added", {{"entity_id", id}});
        }
    }

    return ToolResult::Fail("Entity not found: " + std::to_string(id));
}

} // namespace nebula
