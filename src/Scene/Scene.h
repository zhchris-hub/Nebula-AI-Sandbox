#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <fstream>

namespace nebula {

// 组件定义
struct TagComponent {
    std::string Tag;
    TagComponent() = default;
    TagComponent(const std::string& tag) : Tag(tag) {}
};

struct TransformComponent {
    glm::vec2 Position = {0.0f, 0.0f};
    glm::vec2 Scale = {1.0f, 1.0f};
    float Rotation = 0.0f;
    TransformComponent() = default;
    TransformComponent(const glm::vec2& pos, const glm::vec2& scale, float rot)
        : Position(pos), Scale(scale), Rotation(rot) {}
};

struct SpriteComponent {
    glm::vec4 Color = {1.0f, 1.0f, 1.0f, 1.0f};
    SpriteComponent() = default;
    SpriteComponent(const glm::vec4& color) : Color(color) {}
};

enum class ShapeType { Quad, Triangle };

struct ShapeComponent {
    ShapeType Type = ShapeType::Quad;
    ShapeComponent() = default;
    ShapeComponent(ShapeType type) : Type(type) {}
};

class Entity {
public:
    Entity() = default;
    Entity(entt::entity handle, class Scene* scene) : m_Handle(handle), m_Scene(scene) {}
    Entity(const Entity&) = default;

    uint32_t GetID() const { return static_cast<uint32_t>(m_Handle); }

    operator bool() const { return m_Handle != entt::null; }
    operator entt::entity() const { return m_Handle; }
    bool operator==(const Entity& other) const {
        return m_Handle == other.m_Handle && m_Scene == other.m_Scene;
    }
    bool operator!=(const Entity& other) const { return !(*this == other); }

private:
    entt::entity m_Handle = entt::null;
    class Scene* m_Scene = nullptr;
    friend class Scene;
};

class Scene {
public:
    Scene(const std::string& name = "Untitled Scene");
    ~Scene();

    // 实体管理
    Entity CreateEntity(const std::string& name = "Entity");
    void DestroyEntity(Entity entity);

    // 组件操作
    void AddTransformComponent(Entity entity, const glm::vec2& position, const glm::vec2& scale, float rotation);
    void AddSpriteComponent(Entity entity, const glm::vec4& color);
    void AddShapeComponent(Entity entity, ShapeType type);

    TransformComponent& GetTransformComponent(Entity entity);
    SpriteComponent& GetSpriteComponent(Entity entity);
    ShapeComponent& GetShapeComponent(Entity entity);

    bool HasTransformComponent(Entity entity) const;
    bool HasSpriteComponent(Entity entity) const;
    bool HasShapeComponent(Entity entity) const;

    // 获取所有实体
    const std::vector<Entity>& GetEntities() const { return m_Entities; }

    // 场景更新
    void OnUpdate(float deltaTime);
    void OnRender(class Renderer& renderer);

    // 场景信息
    const std::string& GetName() const { return m_Name; }
    void SetName(const std::string& name) { m_Name = name; }
    size_t GetEntityCount() const { return m_Entities.size(); }

    // 序列化（暂时禁用）
    // nlohmann::json Serialize() const;
    // bool Deserialize(const nlohmann::json& json);
    // bool SaveToFile(const std::string& filepath) const;
    // bool LoadFromFile(const std::string& filepath);

private:
    std::string m_Name;
    entt::registry m_Registry;
    std::vector<Entity> m_Entities;
};

} // namespace nebula
