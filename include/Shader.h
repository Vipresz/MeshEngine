#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glm/glm.hpp>
#include <glad/gl.h>

class Shader {
public:
    Shader();
    Shader(const std::string& vertexSource, const std::string& fragmentSource);
    ~Shader();
    
    bool LoadFromFiles(const std::string& vertexPath, const std::string& fragmentPath);
    bool LoadFromStrings(const std::string& vertexSource, const std::string& fragmentSource);
    
    void Use();
    void Delete();
    
    // Get shader program ID
    GLuint GetID() const { return m_id; }
    
    // Uniform setters
    void SetBool(const std::string& name, bool value);
    void SetInt(const std::string& name, int value);
    void SetFloat(const std::string& name, float value);
    void SetVec2(const std::string& name, const glm::vec2& value);
    void SetVec3(const std::string& name, const glm::vec3& value);
    void SetVec4(const std::string& name, const glm::vec4& value);
    void SetMat4(const std::string& name, const glm::mat4& value);
    
private:
    GLuint m_id;
    std::string m_vertexSource;
    std::string m_fragmentSource;
    
    bool CompileShader(const std::string& source, GLenum type, GLuint& shader);
    bool LinkProgram();
    void CheckCompileErrors(GLuint shader, const std::string& type);
};

#endif 