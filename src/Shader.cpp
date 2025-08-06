#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader() : m_id(0) {
    std::cout << "Shader created" << std::endl;
}

Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource) : m_id(0) {
    std::cout << "Shader created with sources" << std::endl;
    LoadFromStrings(vertexSource, fragmentSource);
}

Shader::~Shader() {
    std::cout << "Shader destroyed" << std::endl;
    Delete();
}

bool Shader::LoadFromFiles(const std::string& vertexPath, const std::string& fragmentPath) {
    std::cout << "Loading shader from files: " << vertexPath << ", " << fragmentPath << std::endl;
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    
    std::stringstream vShaderStream, fShaderStream;
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    
    vShaderFile.close();
    fShaderFile.close();
    
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
    
    std::cout << "✅ Shader loaded from files successfully" << std::endl;
    return LoadFromStrings(vertexCode, fragmentCode);
}

bool Shader::LoadFromStrings(const std::string& vertexSource, const std::string& fragmentSource) {
    std::cout << "Loading shader from strings" << std::endl;
    m_vertexSource = vertexSource;
    m_fragmentSource = fragmentSource;
    
    GLuint vertexShader, fragmentShader;
    
    if (!CompileShader(vertexSource, GL_VERTEX_SHADER, vertexShader)) {
        return false;
    }
    
    if (!CompileShader(fragmentSource, GL_FRAGMENT_SHADER, fragmentShader)) {
        return false;
    }
    
    m_id = glCreateProgram();
    glAttachShader(m_id, vertexShader);
    glAttachShader(m_id, fragmentShader);
    glLinkProgram(m_id);
    
    CheckCompileErrors(m_id, "PROGRAM");
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    std::cout << "✅ Shader compiled and linked successfully" << std::endl;
    return true;
}

void Shader::Use() {
    glUseProgram(m_id);
}

void Shader::Delete() {
    if (m_id != 0) {
        glDeleteProgram(m_id);
        m_id = 0;
    }
}

void Shader::SetBool(const std::string& name, bool value) {
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value) {
    glUniform2fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) {
    glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value) {
    glUniform4fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& value) {
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));     
}

bool Shader::CompileShader(const std::string& source, GLenum type, GLuint& shader) {
    shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    
    CheckCompileErrors(shader, type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
    
    return true;
}

bool Shader::LinkProgram() {
    glLinkProgram(m_id);
    CheckCompileErrors(m_id, "PROGRAM");
    return true;
}

void Shader::CheckCompileErrors(GLuint shader, const std::string& type) {
    GLint success;
    GLchar infoLog[1024];
    
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << std::endl;
        }
    }
} 