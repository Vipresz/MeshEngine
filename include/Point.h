#ifndef POINT_H
#define POINT_H

#include <glm/glm.hpp>
#include <glad/gl.h>

class Point {
public:
    Point(const glm::vec3& position);
    ~Point();
    
    void Render();
    void SetPosition(const glm::vec3& position);
    const glm::vec3& GetPosition() const { return m_position; }
    
    void SetSelected(bool selected) { m_selected = selected; }
    bool IsSelected() const { return m_selected; }
    
    void SetHovered(bool hovered) { m_hovered = hovered; }
    bool IsHovered() const { return m_hovered; }
    
private:
    glm::vec3 m_position;
    bool m_selected;
    bool m_hovered;
    
    // OpenGL objects
    GLuint m_vao;
    GLuint m_vbo;
    int m_vertexCount;
    
    void InitializeBuffers();
};

#endif 