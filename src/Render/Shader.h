#pragma once

#include <string>
#include <glm/glm.hpp>

namespace nebula {

class Shader {
public:
    Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    void SetInt(const std::string& name, int value);
    void SetFloat(const std::string& name, float value);
    void SetFloat3(const std::string& name, const glm::vec3& value);
    void SetFloat4(const std::string& name, const glm::vec4& value);
    void SetMat4(const std::string& name, const glm::mat4& value);

    unsigned int GetRendererID() const { return m_RendererID; }

private:
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int m_RendererID;
};

} // namespace nebula
