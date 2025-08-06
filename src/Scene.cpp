#include "Scene.h"
#include <iostream>

Scene::Scene()
    : m_selectedPoint(-1)
    , m_selectedLine(-1)
    , m_hoveredPoint(-1)
    , m_hoveredLine(-1) {
}

Scene::~Scene() {
}

void Scene::Initialize() {
    // Initialize shaders
    m_pointShader = std::make_unique<Shader>();
    m_lineShader = std::make_unique<Shader>();
    m_gridShader = std::make_unique<Shader>();
    m_axesShader = std::make_unique<Shader>();
    
    // Shader sources for points with constant screen size
    const char* pointVertexSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        uniform float pointSize;
        
        void main() {
            vec4 worldPos = model * vec4(aPos, 1.0);
            vec4 viewPos = view * worldPos;
            gl_Position = projection * viewPos;
            
            // Use constant point size regardless of distance
            gl_PointSize = pointSize;
        }
    )";
    const char* pointFragmentSource = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(1.0, 0.5, 0.2, 1.0); // Orange color for points
        }
    )";
    
    const char* lineVertexSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        
        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
        }
    )";
    const char* lineFragmentSource = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(0.2, 0.5, 1.0, 1.0); // Blue color for lines
        }
    )";
    
    // Grid shader
    const char* gridVertexSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        
        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
        }
    )";
    const char* gridFragmentSource = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(1.0, 1.0, 1.0, 1.0); // White color for grid to make it very visible
        }
    )";
    
    // Axes shader
    const char* axesVertexSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;
        
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        
        out vec3 Color;
        
        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
            Color = aColor;
        }
    )";
    const char* axesFragmentSource = R"(
        #version 330 core
        in vec3 Color;
        out vec4 FragColor;
        void main() {
            FragColor = vec4(Color, 1.0);
        }
    )";
    
    if (!m_pointShader->LoadFromStrings(pointVertexSource, pointFragmentSource)) {
        std::cerr << "Failed to load point shader" << std::endl;
    }
    if (!m_lineShader->LoadFromStrings(lineVertexSource, lineFragmentSource)) {
        std::cerr << "Failed to load line shader" << std::endl;
    }
    if (!m_gridShader->LoadFromStrings(gridVertexSource, gridFragmentSource)) {
        std::cerr << "Failed to load grid shader" << std::endl;
    }
    if (!m_axesShader->LoadFromStrings(axesVertexSource, axesFragmentSource)) {
        std::cerr << "Failed to load axes shader" << std::endl;
    }
    
    m_camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
    
    // Initialize grid and axes
    InitializeGrid();
    InitializeAxes();
}

void Scene::Update() {
    // Update scene logic here
}

void Scene::Render() {
    // Render grid first (background)
    m_gridShader->Use();
    glm::mat4 view = m_camera->GetViewMatrix();
    glm::mat4 projection = m_camera->GetProjectionMatrix();
    
    m_gridShader->SetMat4("view", view);
    m_gridShader->SetMat4("projection", projection);
    
    // Enable blending for grid
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Render grid
    glm::mat4 gridModel = glm::mat4(1.0f);
    m_gridShader->SetMat4("model", gridModel);
    RenderGrid();
    
    // Disable blending for other objects
    glDisable(GL_BLEND);
    
    // Use point shader for points
    m_pointShader->Use();
    m_pointShader->SetMat4("view", view);
    m_pointShader->SetMat4("projection", projection);
    
    // Calculate constant screen size for points
    float pointSize = 20.0f; // Base point size in pixels
    m_pointShader->SetFloat("pointSize", pointSize);
    
    // Render points
    for (const auto& point : m_points) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, point->GetPosition());
        
        // No scaling needed - the shader handles constant screen size
        m_pointShader->SetMat4("model", model);
        point->Render();
    }
    
    // Use line shader for lines
    m_lineShader->Use();
    m_lineShader->SetMat4("view", view);
    m_lineShader->SetMat4("projection", projection);
    
    // Render lines
    for (const auto& line : m_lines) {
        glm::mat4 model = glm::mat4(1.0f);
        m_lineShader->SetMat4("model", model);
        line->Render();
    }
    
    // Render coordinate axes in top right corner
    RenderAxes();
}

void Scene::AddPoint(const glm::vec3& position) {
    m_points.push_back(std::make_unique<Point>(position));
}

void Scene::RemovePoint(int index) {
    if (index >= 0 && index < static_cast<int>(m_points.size())) {
        m_points.erase(m_points.begin() + index);
    }
}

void Scene::SelectPoint(int index) {
    if (index >= 0 && index < static_cast<int>(m_points.size())) {
        m_selectedPoint = index;
        // Deselect all other points
        for (auto& point : m_points) {
            point->SetSelected(false);
        }
        m_points[index]->SetSelected(true);
    }
}

void Scene::DeselectAll() {
    m_selectedPoint = -1;
    m_selectedLine = -1;
    for (auto& point : m_points) {
        point->SetSelected(false);
    }
    for (auto& line : m_lines) {
        line->SetSelected(false);
    }
}

void Scene::AddLine(const glm::vec3& start, const glm::vec3& end) {
    m_lines.push_back(std::make_unique<Line>(start, end));
}

void Scene::RemoveLine(int index) {
    if (index >= 0 && index < static_cast<int>(m_lines.size())) {
        m_lines.erase(m_lines.begin() + index);
    }
}

void Scene::SelectLine(int index) {
    if (index >= 0 && index < static_cast<int>(m_lines.size())) {
        m_selectedLine = index;
        // Deselect all other lines
        for (auto& line : m_lines) {
            line->SetSelected(false);
        }
        m_lines[index]->SetSelected(true);
    }
}

int Scene::GetPointAtScreenPosition(double screenX, double screenY, int viewportWidth, int viewportHeight) {
    // Convert screen coordinates to normalized device coordinates
    // screenX and screenY are already adjusted for the graphics area (panel width subtracted)
    float normalizedX = (2.0f * screenX) / viewportWidth - 1.0f;
    float normalizedY = 1.0f - (2.0f * screenY) / viewportHeight;
    
    // Simple distance-based selection
    const float selectionRadius = 0.05f; // Adjust this value for selection sensitivity
    int closestPoint = -1;
    float closestDistance = selectionRadius;
    
    for (int i = 0; i < static_cast<int>(m_points.size()); ++i) {
        const auto& point = m_points[i];
        glm::vec3 pointPos = point->GetPosition();
        
        // Convert world position to screen space (simplified)
        glm::vec3 screenPos = glm::vec3(pointPos.x, pointPos.y, 0.0f);
        
        // Calculate distance in screen space
        float distance = glm::length(glm::vec2(screenPos.x - normalizedX, screenPos.y - normalizedY));
        
        if (distance < closestDistance) {
            closestDistance = distance;
            closestPoint = i;
        }
    }
    
    return closestPoint;
}

void Scene::UpdateViewport(int width, int height) {
    // Update camera aspect ratio based on new viewport dimensions
    if (m_camera) {
        m_camera->SetAspectRatio(static_cast<float>(width) / static_cast<float>(height));
    }
} 

void Scene::InitializeGrid() {
    // Create grid vertices (1 unit spacing)
    std::vector<float> gridVertices;
    const float gridSize = 5.0f; // Smaller grid for better visibility
    
    // Horizontal lines
    for (float z = -gridSize; z <= gridSize; z += 1.0f) {
        gridVertices.push_back(-gridSize); // x1
        gridVertices.push_back(0.0f);      // y1
        gridVertices.push_back(z);         // z1
        gridVertices.push_back(gridSize);  // x2
        gridVertices.push_back(0.0f);      // y2
        gridVertices.push_back(z);         // z2
    }
    
    // Vertical lines
    for (float x = -gridSize; x <= gridSize; x += 1.0f) {
        gridVertices.push_back(x);         // x1
        gridVertices.push_back(0.0f);      // y1
        gridVertices.push_back(-gridSize); // z1
        gridVertices.push_back(x);         // x2
        gridVertices.push_back(0.0f);      // y2
        gridVertices.push_back(gridSize);  // z2
    }
    
    glGenVertexArrays(1, &m_gridVAO);
    glGenBuffers(1, &m_gridVBO);
    
    glBindVertexArray(m_gridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_gridVBO);
    glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(float), gridVertices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
    
    m_gridVertexCount = gridVertices.size() / 3; // Each vertex has 3 components (x,y,z)
}

void Scene::InitializeAxes() {
    // Create axes vertices (X=red, Y=green, Z=blue)
    std::vector<float> axesVertices;
    std::vector<float> axesColors;
    
    const float axisLength = 0.5f; // Length of axes in top right corner
    
    // X-axis (red)
    axesVertices.push_back(0.0f); axesVertices.push_back(0.0f); axesVertices.push_back(0.0f);
    axesVertices.push_back(axisLength); axesVertices.push_back(0.0f); axesVertices.push_back(0.0f);
    axesColors.push_back(1.0f); axesColors.push_back(0.0f); axesColors.push_back(0.0f);
    axesColors.push_back(1.0f); axesColors.push_back(0.0f); axesColors.push_back(0.0f);
    
    // Y-axis (green)
    axesVertices.push_back(0.0f); axesVertices.push_back(0.0f); axesVertices.push_back(0.0f);
    axesVertices.push_back(0.0f); axesVertices.push_back(axisLength); axesVertices.push_back(0.0f);
    axesColors.push_back(0.0f); axesColors.push_back(1.0f); axesColors.push_back(0.0f);
    axesColors.push_back(0.0f); axesColors.push_back(1.0f); axesColors.push_back(0.0f);
    
    // Z-axis (blue)
    axesVertices.push_back(0.0f); axesVertices.push_back(0.0f); axesVertices.push_back(0.0f);
    axesVertices.push_back(0.0f); axesVertices.push_back(0.0f); axesVertices.push_back(axisLength);
    axesColors.push_back(0.0f); axesColors.push_back(0.0f); axesColors.push_back(1.0f);
    axesColors.push_back(0.0f); axesColors.push_back(0.0f); axesColors.push_back(1.0f);
    
    glGenVertexArrays(1, &m_axesVAO);
    glGenBuffers(1, &m_axesVBO);
    glGenBuffers(1, &m_axesColorVBO);
    
    glBindVertexArray(m_axesVAO);
    
    // Position buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_axesVBO);
    glBufferData(GL_ARRAY_BUFFER, axesVertices.size() * sizeof(float), axesVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Color buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_axesColorVBO);
    glBufferData(GL_ARRAY_BUFFER, axesColors.size() * sizeof(float), axesColors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
    
    m_axesVertexCount = axesVertices.size() / 3;
}

void Scene::RenderGrid() {
    glBindVertexArray(m_gridVAO);
    glDrawArrays(GL_LINES, 0, m_gridVertexCount);
    glBindVertexArray(0);
}

void Scene::RenderAxes() {
    // Save current viewport
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    // Set viewport for top right corner
    int axesSize = 100;
    glViewport(viewport[2] - axesSize - 10, viewport[3] - axesSize - 10, axesSize, axesSize);
    
    // Use axes shader
    m_axesShader->Use();
    
    // Set up orthographic projection for axes
    glm::mat4 axesProjection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    glm::mat4 axesView = glm::mat4(1.0f);
    glm::mat4 axesModel = glm::mat4(1.0f);
    
    m_axesShader->SetMat4("projection", axesProjection);
    m_axesShader->SetMat4("view", axesView);
    m_axesShader->SetMat4("model", axesModel);
    
    // Render axes
    glBindVertexArray(m_axesVAO);
    glDrawArrays(GL_LINES, 0, m_axesVertexCount);
    glBindVertexArray(0);
    
    // Restore viewport
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
} 