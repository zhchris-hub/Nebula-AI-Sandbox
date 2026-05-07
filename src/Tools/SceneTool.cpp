#include "Tools/SceneTool.h"
#include "Core/Log.h"

namespace nebula {

static const std::string TOOL_NAME = "scene";
static const std::string TOOL_DESC = "Scene management tools: create/destroy entities, get/set properties";

SceneTool::SceneTool(Scene* scene) : m_Scene(scene) {}

const std::string& SceneTool::GetName() const { return TOOL_NAME; }
const std::string& SceneTool::GetDescription() const { return TOOL_DESC; }

nlohmann::json SceneTool::GetParameterSchema() const {
    return {
        {"type", "object"},
        {"properties", {
            {"action", {"type", "string", "enum", {"create_entity", "destroy_entity", "get_scene_info", "get_entity_info", "set_transform", "set_color"}}},
            {"entity_name", {"type", "string"}},
            {"entity_id", {"type", "integer"}},
            {"position", {"type", "object", "properties", {{"x", {"type", "number"}}, {"y", {"type", "number"}}}}},
            {"scale", {"type", "object", "properties", {{"x", {"type", "number"}}, {"y", {"type", "number"}}}}},
            {"rotation", {"type", "number"}},
            {"color", {"type", "object", "properties", {{"r", {"type", "number"}}, {"g", {"type", "number"}}, {"b", {"type", "number"}}, {"a", {"type", "number"}}}}}
        }},
        {"required", {"action"}}
    };
}

ToolResult SceneTool::Execute(const nlohmann::json& params) {
    if (!params.contains("action")) {
        return ToolResult::Fail("Missing 'action' parameter");
    }

    std::string action = params["action"].get<std::string>();

    if (action == "create_entity") return CreateEntity(params);
    if (action == "destroy_entity") return DestroyEntity(params);
    if (action == "get_scene_info") return GetSceneInfo(params);
    if (action == "get_entity_info") return GetEntityInfo(params);
    if (action == "set_transform") return SetEntityTransform(params);
    if (action == "set_color") return SetEntityColor(params);

    return ToolResult::Fail("Unknown action: " + action);
}

ToolResult SceneTool::CreateEntity(const nlohmann::json& params) {
    std::string name = params.value("entity_name", "Entity");
    Entity entity = m_Scene->CreateEntity(name);

    // Add transform if provided
    if (params.contains("position") || params.contains("scale")) {
        glm::vec2 pos = {0.0f, 0.0f};
        glm::vec2 scale = {100.0f, 100.0f};
        float rotation = 0.0f;

        if (params.contains("position")) {
            pos.x = params["position"].value("x", 0.0f);
            pos.y = params["position"].value("y", 0.0f);
        }
        if (params.contains("scale")) {
            scale.x = params["scale"].value("x", 100.0f);
            scale.y = params["scale"].value("y", 100.0f);
        }
        if (params.contains("rotation")) {
            rotation = params["rotation"].get<float>();
        }

        m_Scene->AddTransformComponent(entity, pos, scale, rotation);
    } else {
        m_Scene->AddTransformComponent(entity, {0.0f, 0.0f}, {100.0f, 100.0f}, 0.0f);
    }

    // Add color if provided
    glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
    if (params.contains("color")) {
        color.r = params["color"].value("r", 1.0f);
        color.g = params["color"].value("g", 1.0f);
        color.b = params["color"].value("b", 1.0f);
        color.a = params["color"].value("a", 1.0f);
    }
    m_Scene->AddSpriteComponent(entity, color);
    m_Scene->AddShapeComponent(entity, ShapeType::Quad);

    return ToolResult::Ok("Entity created: " + name, {{"entity_id", entity.GetID()}});
}

ToolResult SceneTool::DestroyEntity(const nlohmann::json& params) {
    if (!params.contains("entity_id")) {
        return ToolResult::Fail("Missing 'entity_id' parameter");
    }

    uint32_t id = params["entity_id"].get<uint32_t>();

    // Find entity by ID
    for (auto& entity : m_Scene->GetEntities()) {
        if (entity.GetID() == id) {
            m_Scene->DestroyEntity(entity);
            return ToolResult::Ok("Entity destroyed", {{"entity_id", id}});
        }
    }

    return ToolResult::Fail("Entity not found: " + std::to_string(id));
}

ToolResult SceneTool::GetSceneInfo(const nlohmann::json& params) {
    (void)params;

    nlohmann::json entities = nlohmann::json::array();
    for (auto& entity : m_Scene->GetEntities()) {
        nlohmann::json entityInfo = {{"id", entity.GetID()}};

        if (m_Scene->HasTransformComponent(entity)) {
            auto& transform = m_Scene->GetTransformComponent(entity);
            entityInfo["position"] = {{"x", transform.Position.x}, {"y", transform.Position.y}};
            entityInfo["scale"] = {{"x", transform.Scale.x}, {"y", transform.Scale.y}};
            entityInfo["rotation"] = transform.Rotation;
        }

        if (m_Scene->HasSpriteComponent(entity)) {
            auto& sprite = m_Scene->GetSpriteComponent(entity);
            entityInfo["color"] = {{"r", sprite.Color.r}, {"g", sprite.Color.g}, {"b", sprite.Color.b}, {"a", sprite.Color.a}};
        }

        entities.push_back(entityInfo);
    }

    return ToolResult::Ok("Scene info retrieved", {
        {"name", m_Scene->GetName()},
        {"entity_count", m_Scene->GetEntityCount()},
        {"entities", entities}
    });
}

ToolResult SceneTool::GetEntityInfo(const nlohmann::json& params) {
    if (!params.contains("entity_id")) {
        return ToolResult::Fail("Missing 'entity_id' parameter");
    }

    uint32_t id = params["entity_id"].get<uint32_t>();

    for (auto& entity : m_Scene->GetEntities()) {
        if (entity.GetID() == id) {
            nlohmann::json info = {{"id", id}};

            if (m_Scene->HasTransformComponent(entity)) {
                auto& t = m_Scene->GetTransformComponent(entity);
                info["position"] = {{"x", t.Position.x}, {"y", t.Position.y}};
                info["scale"] = {{"x", t.Scale.x}, {"y", t.Scale.y}};
                info["rotation"] = t.Rotation;
            }

            if (m_Scene->HasSpriteComponent(entity)) {
                auto& s = m_Scene->GetSpriteComponent(entity);
                info["color"] = {{"r", s.Color.r}, {"g", s.Color.g}, {"b", s.Color.b}, {"a", s.Color.a}};
            }

            if (m_Scene->HasRigidBodyComponent(entity)) {
                auto& rb = m_Scene->GetRigidBodyComponent(entity);
                info["physics"] = {{"is_static", rb.IsStatic}, {"density", rb.Density}, {"friction", rb.Friction}, {"restitution", rb.Restitution}};
            }

            return ToolResult::Ok("Entity info retrieved", info);
        }
    }

    return ToolResult::Fail("Entity not found: " + std::to_string(id));
}

ToolResult SceneTool::SetEntityTransform(const nlohmann::json& params) {
    if (!params.contains("entity_id")) {
        return ToolResult::Fail("Missing 'entity_id' parameter");
    }

    uint32_t id = params["entity_id"].get<uint32_t>();

    for (auto& entity : m_Scene->GetEntities()) {
        if (entity.GetID() == id && m_Scene->HasTransformComponent(entity)) {
            auto& transform = m_Scene->GetTransformComponent(entity);

            if (params.contains("position")) {
                transform.Position.x = params["position"].value("x", transform.Position.x);
                transform.Position.y = params["position"].value("y", transform.Position.y);
            }
            if (params.contains("scale")) {
                transform.Scale.x = params["scale"].value("x", transform.Scale.x);
                transform.Scale.y = params["scale"].value("y", transform.Scale.y);
            }
            if (params.contains("rotation")) {
                transform.Rotation = params["rotation"].get<float>();
            }

            return ToolResult::Ok("Transform updated");
        }
    }

    return ToolResult::Fail("Entity not found: " + std::to_string(id));
}

ToolResult SceneTool::SetEntityColor(const nlohmann::json& params) {
    if (!params.contains("entity_id")) {
        return ToolResult::Fail("Missing 'entity_id' parameter");
    }

    uint32_t id = params["entity_id"].get<uint32_t>();

    for (auto& entity : m_Scene->GetEntities()) {
        if (entity.GetID() == id && m_Scene->HasSpriteComponent(entity)) {
            auto& sprite = m_Scene->GetSpriteComponent(entity);

            if (params.contains("color")) {
                sprite.Color.r = params["color"].value("r", sprite.Color.r);
                sprite.Color.g = params["color"].value("g", sprite.Color.g);
                sprite.Color.b = params["color"].value("b", sprite.Color.b);
                sprite.Color.a = params["color"].value("a", sprite.Color.a);
            }

            return ToolResult::Ok("Color updated");
        }
    }

    return ToolResult::Fail("Entity not found: " + std::to_string(id));
}

} // namespace nebula
