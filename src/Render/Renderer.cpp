#include "Render/Renderer.h"
#include "Core/Log.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <sstream>

namespace nebula {

static std::string ReadFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        NEBULA_ERROR("Failed to open file: {0}", path);
        return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

Renderer::Renderer() {}

Renderer::~Renderer() {
    Shutdown();
}

void Renderer::Init() {
    // Load glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        NEBULA_CRITICAL("Failed to initialize glad!");
        return;
    }

    NEBULA_INFO("OpenGL Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    NEBULA_INFO("OpenGL Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    NEBULA_INFO("OpenGL Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Load shaders
    std::string vertexSrc = ReadFile("assets/shaders/Vertex.glsl");
    std::string fragmentSrc = ReadFile("assets/shaders/Fragment.glsl");
    m_Shader = new Shader(vertexSrc, fragmentSrc);

    // ── Triangle VAO ────────────────────────────────────────────────────
    float triangleVertices[] = {
        // position      // color
        -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
         0.0f,  0.5f,   0.0f, 0.0f, 1.0f,
    };

    glGenVertexArrays(1, &m_TriangleVAO);
    glGenBuffers(1, &m_TriangleVBO);
    glBindVertexArray(m_TriangleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_TriangleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);

    // ── Quad VAO ────────────────────────────────────────────────────────
    float quadVertices[] = {
        // position      // color
        -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,
         0.5f, -0.5f,   1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,   1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,
    };

    unsigned int quadIndices[] = {
        0, 1, 2,
        2, 3, 0,
    };

    glGenVertexArrays(1, &m_QuadVAO);
    glGenBuffers(1, &m_QuadVBO);
    glGenBuffers(1, &m_QuadEBO);
    glBindVertexArray(m_QuadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);
}

void Renderer::Shutdown() {
    delete m_Shader;
    m_Shader = nullptr;

    glDeleteVertexArrays(1, &m_TriangleVAO);
    glDeleteBuffers(1, &m_TriangleVBO);
    glDeleteVertexArrays(1, &m_QuadVAO);
    glDeleteBuffers(1, &m_QuadVBO);
    glDeleteBuffers(1, &m_QuadEBO);
}

void Renderer::BeginScene(const Camera& camera) {
    m_Shader->Bind();
    m_Shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
}

void Renderer::EndScene() {
    m_Shader->Unbind();
}

void Renderer::DrawTriangle(const glm::vec2& position, const glm::vec2& size,
                             const glm::vec4& color, float rotation) {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1))
        * glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

    m_Shader->SetMat4("u_Model", model);
    m_Shader->SetFloat4("u_Color", color);

    glBindVertexArray(m_TriangleVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size,
                         const glm::vec4& color, float rotation) {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1))
        * glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

    m_Shader->SetMat4("u_Model", model);
    m_Shader->SetFloat4("u_Color", color);

    glBindVertexArray(m_QuadVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Renderer::SetClearColor(const glm::vec4& color) {
    glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::Clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

} // namespace nebula
