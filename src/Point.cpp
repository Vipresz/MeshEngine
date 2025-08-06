#include "Point.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cmath>
#include <vector>

Point::Point(const glm::vec3& position)
    : m_position(position), m_selected(false), m_hovered(false) {
    InitializeBuffers();
}

Point::~Point() {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
}

void Point::Render() {
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
    glBindVertexArray(0);
}

void Point::SetPosition(const glm::vec3& position) {
    m_position = position;
}

void Point::InitializeBuffers() {
    
    // Create a proper sphere
    const int latitudeSegments = 16;
    const int longitudeSegments = 16;
    const float radius = 0.1f;
    
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    
    // Generate sphere vertices
    for (int lat = 0; lat <= latitudeSegments; ++lat) {
        float theta = M_PI * (float)lat / latitudeSegments;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);
        
        for (int lon = 0; lon <= longitudeSegments; ++lon) {
            float phi = 2.0f * M_PI * (float)lon / longitudeSegments;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);
            
            float x = radius * cosPhi * sinTheta;
            float y = radius * cosTheta;
            float z = radius * sinPhi * sinTheta;
            
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }
    
    // Generate indices for triangles
    for (int lat = 0; lat < latitudeSegments; ++lat) {
        for (int lon = 0; lon < longitudeSegments; ++lon) {
            int current = lat * (longitudeSegments + 1) + lon;
            int next = current + longitudeSegments + 1;
            
            // First triangle
            indices.push_back(current);
            indices.push_back(next);
            indices.push_back(current + 1);
            
            // Second triangle
            indices.push_back(next);
            indices.push_back(next + 1);
            indices.push_back(current + 1);
        }
    }
    
    // Convert indices to triangle vertices for glDrawArrays
    std::vector<float> triangleVertices;
    for (size_t i = 0; i < indices.size(); i += 3) {
        // First vertex of triangle
        int idx1 = indices[i] * 3;
        triangleVertices.push_back(vertices[idx1]);
        triangleVertices.push_back(vertices[idx1 + 1]);
        triangleVertices.push_back(vertices[idx1 + 2]);
        
        // Second vertex of triangle
        int idx2 = indices[i + 1] * 3;
        triangleVertices.push_back(vertices[idx2]);
        triangleVertices.push_back(vertices[idx2 + 1]);
        triangleVertices.push_back(vertices[idx2 + 2]);
        
        // Third vertex of triangle
        int idx3 = indices[i + 2] * 3;
        triangleVertices.push_back(vertices[idx3]);
        triangleVertices.push_back(vertices[idx3 + 1]);
        triangleVertices.push_back(vertices[idx3 + 2]);
    }
    
    m_vertexCount = triangleVertices.size() / 3;
    
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, triangleVertices.size() * sizeof(float), triangleVertices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
    
} 