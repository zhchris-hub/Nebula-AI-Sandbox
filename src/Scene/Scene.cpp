#include "Scene/Scene.h"
#include "Render/Renderer.h"
#include "Core/Log.h"
#include <fstream>

namespace nebula {

Scene::Scene(const std::string& name)
    : m_Name(name), m_PhysicsWorld(std::make_unique<PhysicsWorld>()) {
    NEBULA_INFO("Scene created: {0}", m_Name);
}

Scene::~Scene() {
    for (auto& entity : m_Entities) {
        if (HasRigidBodyComponent(entity)) {
            auto& rb = GetRigidBodyComponent(entity);
            if (rb.Body) {
                m_PhysicsWorld->DestroyBody(rb.Body);
                rb.Body = nullptr;
            }
        }
    }
    m_Entities.clear();
    m_Registry.clear();
    NEBULA_INFO("Scene destroyed: {0}", m_Name);
}

Entity Scene::CreateEntity(const std::string& name) {
    Entity entity(m_Registry.create(), this);
    m_Registry.emplace<TagComponent>(entity, name);
    m_Registry.emplace<TransformComponent>(entity);
    m_Entities.push_back(entity);
    return entity;
}

void Scene::DestroyEntity(Entity entity) {
    auto it = std::find(m_Entities.begin(), m_Entities.end(), entity);
    if (it != m_Entities.end()) {
        m_Entities.erase(it);
    }
    m_Registry.destroy(entity);
}

void Scene::AddTransformComponent(Entity entity, const glm::vec2& position, const glm::vec2& scale, float rotation) {
    m_Registry.emplace<TransformComponent>(entity, position, scale, rotation);
}

void Scene::AddSpriteComponent(Entity entity, const glm::vec4& color) {
    m_Registry.emplace<SpriteComponent>(entity, color);
}

void Scene::AddShapeComponent(Entity entity, ShapeType type) {
    m_Registry.emplace<ShapeComponent>(entity, type);
}

void Scene::AddRigidBodyComponent(Entity entity, bool isStatic, float density, float friction, float restitution) {
    if (!HasTransformComponent(entity)) {
        NEBULA_ERROR("Entity must have TransformComponent to add RigidBodyComponent");
        return;
    }

    auto& transform = GetTransformComponent(entity);

    b2BodyDef bodyDef;
    bodyDef.position.Set(transform.Position.x / 30.0f, transform.Position.y / 30.0f);
    bodyDef.angle = transform.Rotation;
    bodyDef.type = isStatic ? b2_staticBody : b2_dynamicBody;

    b2Body* body = m_PhysicsWorld->CreateBody(bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(transform.Scale.x / 30.0f / 2.0f, transform.Scale.y / 30.0f / 2.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    fixtureDef.restitution = restitution;

    body->CreateFixture(&fixtureDef);

    m_Registry.emplace<RigidBodyComponent>(entity, isStatic, density, friction, restitution);
    auto& rb = m_Registry.get<RigidBodyComponent>(entity);
    rb.Body = body;
}

TransformComponent& Scene::GetTransformComponent(Entity entity) {
    return m_Registry.get<TransformComponent>(entity);
}

SpriteComponent& Scene::GetSpriteComponent(Entity entity) {
    return m_Registry.get<SpriteComponent>(entity);
}

ShapeComponent& Scene::GetShapeComponent(Entity entity) {
    return m_Registry.get<ShapeComponent>(entity);
}

RigidBodyComponent& Scene::GetRigidBodyComponent(Entity entity) {
    return m_Registry.get<RigidBodyComponent>(entity);
}

bool Scene::HasTransformComponent(Entity entity) const {
    return m_Registry.all_of<TransformComponent>(entity);
}

bool Scene::HasSpriteComponent(Entity entity) const {
    return m_Registry.all_of<SpriteComponent>(entity);
}

bool Scene::HasShapeComponent(Entity entity) const {
    return m_Registry.all_of<ShapeComponent>(entity);
}

bool Scene::HasRigidBodyComponent(Entity entity) const {
    return m_Registry.all_of<RigidBodyComponent>(entity);
}

void Scene::SetGravity(const glm::vec2& gravity) {
    m_PhysicsWorld->SetGravity(gravity);
}

void Scene::OnUpdate(float deltaTime) {
    // Update physics
    m_PhysicsWorld->Step(deltaTime);

    // Sync physics transforms to ECS
    auto view = m_Registry.view<TransformComponent, RigidBodyComponent>();
    for (auto entity : view) {
        auto& transform = view.get<TransformComponent>(entity);
        auto& rb = view.get<RigidBodyComponent>(entity);
        if (rb.Body) {
            b2Vec2 pos = rb.Body->GetPosition();
            transform.Position = glm::vec2(pos.x * 30.0f, pos.y * 30.0f);
            transform.Rotation = rb.Body->GetAngle();
        }
    }
}

void Scene::OnRender(Renderer& renderer) {
    auto view = m_Registry.view<TransformComponent, SpriteComponent>();
    for (auto entity : view) {
        auto& transform = view.get<TransformComponent>(entity);
        auto& sprite = view.get<SpriteComponent>(entity);

        if (m_Registry.all_of<ShapeComponent>(entity)) {
            auto& shape = m_Registry.get<ShapeComponent>(entity);
            if (shape.Type == ShapeType::Triangle) {
                renderer.DrawTriangle(transform.Position, transform.Scale, sprite.Color, transform.Rotation);
            } else {
                renderer.DrawQuad(transform.Position, transform.Scale, sprite.Color, transform.Rotation);
            }
        } else {
            renderer.DrawQuad(transform.Position, transform.Scale, sprite.Color, transform.Rotation);
        }
    }
}

} // namespace nebula
