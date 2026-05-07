#pragma once

#include <glm/glm.hpp>

namespace nebula {

class Camera {
public:
    Camera(float left, float right, float bottom, float top);

    void SetPosition(const glm::vec2& position) { m_Position = position; Recalculate(); }
    void SetRotation(float rotation) { m_Rotation = rotation; Recalculate(); }
    void SetZoom(float zoom) { m_Zoom = zoom; Recalculate(); }

    const glm::vec2& GetPosition() const { return m_Position; }
    float GetRotation() const { return m_Rotation; }
    float GetZoom() const { return m_Zoom; }

    const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
    const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
    glm::mat4 GetViewProjectionMatrix() const { return m_ProjectionMatrix * m_ViewMatrix; }

    void SetProjection(float left, float right, float bottom, float top);

private:
    void Recalculate();

    glm::vec2 m_Position = { 0.0f, 0.0f };
    float m_Rotation = 0.0f;
    float m_Zoom = 1.0f;

    glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
    glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
};

} // namespace nebula
