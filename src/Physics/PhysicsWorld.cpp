#include "Physics/PhysicsWorld.h"
#include "Core/Log.h"

namespace nebula {

PhysicsWorld::PhysicsWorld(const glm::vec2& gravity) {
    m_World = new b2World(b2Vec2(gravity.x, gravity.y));
    NEBULA_INFO("PhysicsWorld created with gravity ({0}, {1})", gravity.x, gravity.y);
}

PhysicsWorld::~PhysicsWorld() {
    for (b2Body* body : m_Bodies) {
        m_World->DestroyBody(body);
    }
    m_Bodies.clear();

    delete m_World;
    m_World = nullptr;
    NEBULA_INFO("PhysicsWorld destroyed");
}

void PhysicsWorld::Step(float deltaTime) {
    m_World->Step(deltaTime, 8, 3);
}

void PhysicsWorld::SetGravity(const glm::vec2& gravity) {
    m_World->SetGravity(b2Vec2(gravity.x, gravity.y));
}

b2Body* PhysicsWorld::CreateBody(const b2BodyDef& bodyDef) {
    b2Body* body = m_World->CreateBody(&bodyDef);
    m_Bodies.push_back(body);
    return body;
}

void PhysicsWorld::DestroyBody(b2Body* body) {
    auto it = std::find(m_Bodies.begin(), m_Bodies.end(), body);
    if (it != m_Bodies.end()) {
        m_Bodies.erase(it);
    }
    m_World->DestroyBody(body);
}

b2Fixture* PhysicsWorld::CreateFixture(b2Body* body, const b2FixtureDef& fixtureDef) {
    return body->CreateFixture(&fixtureDef);
}

} // namespace nebula
