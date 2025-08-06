#ifndef LINE_H
#define LINE_H

#include <glm/glm.hpp>
#include <glad/gl.h>

class Line {
public:
    Line(const glm::vec3& start, const glm::vec3& end);
    ~Line();
    
    void Render();
    void SetStart(const glm::vec3& start);
    void SetEnd(const glm::vec3& end);
    const glm::vec3& GetStart() const { return m_start; }
    const glm::vec3& GetEnd() const { return m_end; }
    
    void SetSelected(bool selected) { m_selected = selected; }
    bool IsSelected() const { return m_selected; }
    
    void SetHovered(bool hovered) { m_hovered = hovered; }
    bool IsHovered() const { return m_hovered; }
    
private:
    glm::vec3 m_start;
    glm::vec3 m_end;
    bool m_selected;
    bool m_hovered;
    
    // OpenGL objects
    GLuint m_vao;
    GLuint m_vbo;
    
    void InitializeBuffers();
    void UpdateBuffers();
};

#endif 