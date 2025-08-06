#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include "Point.h"
#include "Line.h"
#include "Camera.h"
#include "Shader.h"

class Scene {
public:
    Scene();
    ~Scene();
    
    void Initialize();
    void Update();
    void Render();
    
    // Point management
    void AddPoint(const glm::vec3& position);
    void RemovePoint(int index);
    void SelectPoint(int index);
    void DeselectAll();
    
    // Line management
    void AddLine(const glm::vec3& start, const glm::vec3& end);
    void RemoveLine(int index);
    void SelectLine(int index);
    
    // Getters
    const std::vector<std::unique_ptr<Point>>& GetPoints() const { return m_points; }
    const std::vector<std::unique_ptr<Line>>& GetLines() const { return m_lines; }
    Camera& GetCamera() { return *m_camera; }
    
    // Point selection by screen position
    int GetPointAtScreenPosition(double screenX, double screenY, int viewportWidth, int viewportHeight);
    
    // Viewport management
    void UpdateViewport(int width, int height);
    
private:
    std::vector<std::unique_ptr<Point>> m_points;
    std::vector<std::unique_ptr<Line>> m_lines;
    std::unique_ptr<Camera> m_camera;
    
    // Shaders
    std::unique_ptr<Shader> m_pointShader;
    std::unique_ptr<Shader> m_lineShader;
    std::unique_ptr<Shader> m_gridShader;
    std::unique_ptr<Shader> m_axesShader;
    
    // Grid rendering
    GLuint m_gridVAO, m_gridVBO;
    int m_gridVertexCount;
    
    // Axes rendering
    GLuint m_axesVAO, m_axesVBO, m_axesColorVBO;
    int m_axesVertexCount;
    
    // Selection state
    int m_selectedPoint;
    int m_selectedLine;
    int m_hoveredPoint;
    int m_hoveredLine;
    
    // Grid and axes methods
    void InitializeGrid();
    void InitializeAxes();
    void RenderGrid();
    void RenderAxes();
};

#endif 