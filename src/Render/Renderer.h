#pragma once

#include <glm/glm.hpp>
#include "Render/Shader.h"
#include "Render/Camera.h"

namespace nebula {

class Renderer {
public:
    Renderer();
    ~Renderer();

    void Init();
    void Shutdown();

    void BeginScene(const Camera& camera);
    void EndScene();

    void DrawTriangle(const glm::vec2& position, const glm::vec2& size,
                      const glm::vec4& color, float rotation = 0.0f);
    void DrawQuad(const glm::vec2& position, const glm::vec2& size,
                  const glm::vec4& color, float rotation = 0.0f);

    void SetClearColor(const glm::vec4& color);
    void Clear();

private:
    Shader* m_Shader = nullptr;
    unsigned int m_TriangleVAO = 0, m_TriangleVBO = 0;
    unsigned int m_QuadVAO = 0, m_QuadVBO = 0, m_QuadEBO = 0;
};

} // namespace nebula
