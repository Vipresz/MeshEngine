#include "Line.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Line::Line(const glm::vec3& start, const glm::vec3& end)
    : m_start(start), m_end(end), m_selected(false), m_hovered(false) {
    std::cout << "Line created from (" << start.x << ", " << start.y << ", " << start.z 
              << ") to (" << end.x << ", " << end.y << ", " << end.z << ")" << std::endl;
    InitializeBuffers();
}

Line::~Line() {
    std::cout << "Line destroyed from (" << m_start.x << ", " << m_start.y << ", " << m_start.z 
              << ") to (" << m_end.x << ", " << m_end.y << ", " << m_end.z << ")" << std::endl;
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
}

void Line::Render() {
    glBindVertexArray(m_vao);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
}

void Line::SetStart(const glm::vec3& start) {
    std::cout << "Line start updated from (" << m_start.x << ", " << m_start.y << ", " << m_start.z 
              << ") to (" << start.x << ", " << start.y << ", " << start.z << ")" << std::endl;
    m_start = start;
    UpdateBuffers();
}

void Line::SetEnd(const glm::vec3& end) {
    std::cout << "Line end updated from (" << m_end.x << ", " << m_end.y << ", " << m_end.z 
              << ") to (" << end.x << ", " << end.y << ", " << end.z << ")" << std::endl;
    m_end = end;
    UpdateBuffers();
}

void Line::InitializeBuffers() {
    std::cout << "Initializing line buffers" << std::endl;
    
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
    
    UpdateBuffers();
}

void Line::UpdateBuffers() {
    float vertices[] = {
        m_start.x, m_start.y, m_start.z,
        m_end.x, m_end.y, m_end.z
    };
    
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
} 