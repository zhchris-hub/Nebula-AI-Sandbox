#pragma once

#include <box2d/box2d.h>
#include <glm/glm.hpp>
#include <vector>

namespace nebula {

class PhysicsWorld {
public:
    PhysicsWorld(const glm::vec2& gravity = {0.0f, -9.8f});
    ~PhysicsWorld();

    void Step(float deltaTime);
    void SetGravity(const glm::vec2& gravity);

    // 创建刚体
    b2Body* CreateBody(const b2BodyDef& bodyDef);
    void DestroyBody(b2Body* body);

    // 创建形状
    b2Fixture* CreateFixture(b2Body* body, const b2FixtureDef& fixtureDef);

    // 获取世界
    b2World& GetWorld() { return *m_World; }
    const b2World& GetWorld() const { return *m_World; }

private:
    b2World* m_World = nullptr;
    std::vector<b2Body*> m_Bodies;
};

} // namespace nebula
