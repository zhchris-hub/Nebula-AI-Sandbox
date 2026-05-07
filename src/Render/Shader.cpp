#include "Render/Shader.h"
#include "Core/Log.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace nebula {

Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc) {
    unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSrc);
    unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);

    m_RendererID = glCreateProgram();
    glAttachShader(m_RendererID, vertexShader);
    glAttachShader(m_RendererID, fragmentShader);
    glLinkProgram(m_RendererID);

    int success;
    glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(m_RendererID, 512, nullptr, infoLog);
        NEBULA_ERROR("Shader program link failed: {0}", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const {
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

void Shader::SetInt(const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(m_RendererID, name.c_str()), value);
}

void Shader::SetFloat3(const std::string& name, const glm::vec3& value) {
    glUniform3f(glGetUniformLocation(m_RendererID, name.c_str()), value.x, value.y, value.z);
}

void Shader::SetFloat4(const std::string& name, const glm::vec4& value) {
    glUniform4f(glGetUniformLocation(m_RendererID, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& value) {
    glUniformMatrix4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    unsigned int shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        NEBULA_ERROR("Shader compilation failed: {0}", infoLog);
    }

    return shader;
}

} // namespace nebula
