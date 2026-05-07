#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <box2d/box2d.h>
#include "Physics/PhysicsWorld.h"
#include <string>
#include <vector>
#include <fstream>

namespace nebula {

// Components
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

struct RigidBodyComponent {
    b2Body* Body = nullptr;
    bool IsStatic = true;
    float Density = 1.0f;
    float Friction = 0.3f;
    float Restitution = 0.2f;
    RigidBodyComponent() = default;
    RigidBodyComponent(bool isStatic, float density = 1.0f, float friction = 0.3f, float restitution = 0.2f)
        : IsStatic(isStatic), Density(density), Friction(friction), Restitution(restitution) {}
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

    // Entity management
    Entity CreateEntity(const std::string& name = "Entity");
    void DestroyEntity(Entity entity);

    // Component operations
    void AddTransformComponent(Entity entity, const glm::vec2& position, const glm::vec2& scale, float rotation);
    void AddSpriteComponent(Entity entity, const glm::vec4& color);
    void AddShapeComponent(Entity entity, ShapeType type);
    void AddRigidBodyComponent(Entity entity, bool isStatic, float density = 1.0f, float friction = 0.3f, float restitution = 0.2f);

    TransformComponent& GetTransformComponent(Entity entity);
    SpriteComponent& GetSpriteComponent(Entity entity);
    ShapeComponent& GetShapeComponent(Entity entity);
    RigidBodyComponent& GetRigidBodyComponent(Entity entity);

    bool HasTransformComponent(Entity entity) const;
    bool HasSpriteComponent(Entity entity) const;
    bool HasShapeComponent(Entity entity) const;
    bool HasRigidBodyComponent(Entity entity) const;

    // Get all entities
    const std::vector<Entity>& GetEntities() const { return m_Entities; }

    // Physics world
    PhysicsWorld& GetPhysicsWorld() { return *m_PhysicsWorld; }
    void SetGravity(const glm::vec2& gravity);

    // Scene update
    void OnUpdate(float deltaTime);
    void OnRender(class Renderer& renderer);

    // Scene info
    const std::string& GetName() const { return m_Name; }
    void SetName(const std::string& name) { m_Name = name; }
    size_t GetEntityCount() const { return m_Entities.size(); }

private:
    std::string m_Name;
    entt::registry m_Registry;
    std::vector<Entity> m_Entities;
    std::unique_ptr<PhysicsWorld> m_PhysicsWorld;
};

} // namespace nebula
