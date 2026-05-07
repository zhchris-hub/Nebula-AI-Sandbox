#include "Render/Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace nebula {

Camera::Camera(float left, float right, float bottom, float top) {
    SetProjection(left, right, bottom, top);
    Recalculate();
}

void Camera::SetProjection(float left, float right, float bottom, float top) {
    m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
}

void Camera::Recalculate() {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(m_Position, 0.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1))
        * glm::scale(glm::mat4(1.0f), glm::vec3(m_Zoom, m_Zoom, 1.0f));

    m_ViewMatrix = glm::inverse(transform);
}

} // namespace nebula
