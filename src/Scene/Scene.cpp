#include "Scene/Scene.h"
#include "Render/Renderer.h"
#include "Core/Log.h"
#include <fstream>

namespace nebula {

Scene::Scene(const std::string& name)
    : m_Name(name) {
    NEBULA_INFO("Scene created: {0}", m_Name);
}

Scene::~Scene() {
    // 清理所有实体
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

TransformComponent& Scene::GetTransformComponent(Entity entity) {
    return m_Registry.get<TransformComponent>(entity);
}

SpriteComponent& Scene::GetSpriteComponent(Entity entity) {
    return m_Registry.get<SpriteComponent>(entity);
}

ShapeComponent& Scene::GetShapeComponent(Entity entity) {
    return m_Registry.get<ShapeComponent>(entity);
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

void Scene::OnUpdate(float deltaTime) {
    (void)deltaTime;
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
