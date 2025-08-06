#include "UIComponent.h"
#include <iostream>

UIComponent::UIComponent(int windowWidth, int windowHeight)
    : m_currentTool(Tool::Point), m_isAddingLine(false), m_firstPointIndex(-1),
      m_panelWidth(200), m_windowWidth(windowWidth), m_windowHeight(windowHeight),
      m_initialized(false), m_uiShader(nullptr), m_uiVAO(0), m_uiVBO(0) {
}

UIComponent::~UIComponent() {
    if (m_uiVAO) glDeleteVertexArrays(1, &m_uiVAO);
    if (m_uiVBO) glDeleteBuffers(1, &m_uiVBO);
}

void UIComponent::Initialize() {
    CreateButtons();
    InitializeUIShader();
    InitializeUIBuffers();
    m_initialized = true;
}

void UIComponent::InitializeUIShader() {
    // Simple UI shader for 2D rendering with proper projection
    const char* vertexSource = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        layout (location = 1) in vec3 aColor;
        uniform mat4 projection;
        out vec3 ourColor;
        void main() {
            gl_Position = projection * vec4(aPos, 0.0, 1.0);
            ourColor = aColor;
        }
    )";
    
    const char* fragmentSource = R"(
        #version 330 core
        in vec3 ourColor;
        out vec4 FragColor;
        void main() {
            FragColor = vec4(ourColor, 1.0);
        }
    )";
    
    m_uiShader = std::make_unique<Shader>(vertexSource, fragmentSource);
}

void UIComponent::InitializeUIBuffers() {
    std::cout << "Initializing UI buffers" << std::endl;
    
    glGenVertexArrays(1, &m_uiVAO);
    glGenBuffers(1, &m_uiVBO);
    
    glBindVertexArray(m_uiVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_uiVBO);
    
    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
}

void UIComponent::CreateButtons() {
    std::cout << "Creating UI buttons" << std::endl;
    
    // Clear any existing buttons
    m_buttons.clear();
    
    // Point tool button - place it higher up and make it bigger
    m_buttons.emplace_back(glm::vec2(10, m_windowHeight - 100), glm::vec2(120, 40), "Point", Tool::Point);
    
    // Line tool button - place it below the Point button
    m_buttons.emplace_back(glm::vec2(10, m_windowHeight - 150), glm::vec2(120, 40), "Line", Tool::Line);
    
    std::cout << "Created " << m_buttons.size() << " buttons" << std::endl;
    std::cout << "Window height: " << m_windowHeight << std::endl;
    std::cout << "Point button at: (" << m_buttons[0].position.x << ", " << m_buttons[0].position.y << ")" << std::endl;
    std::cout << "Line button at: (" << m_buttons[1].position.x << ", " << m_buttons[1].position.y << ")" << std::endl;
}

void UIComponent::Update() {
    // Handle mouse input for UI
    // This would be implemented with GLFW mouse callbacks
}

void UIComponent::Render() {
    if (!m_initialized) {
        std::cout << "UI not initialized, skipping render" << std::endl;
        return;
    }
    
    std::cout << "Rendering UI components" << std::endl;
    
    // Save current OpenGL state
    GLint prevViewport[4];
    glGetIntegerv(GL_VIEWPORT, prevViewport);
    
    // Set up 2D orthographic projection for UI - use full window viewport
    glViewport(0, 0, m_windowWidth, m_windowHeight);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Use UI shader
    if (m_uiShader) {
        m_uiShader->Use();
        
        // Set up orthographic projection matrix
        // Left=0, Right=windowWidth, Bottom=0, Top=windowHeight
        float left = 0.0f;
        float right = static_cast<float>(m_windowWidth);
        float bottom = 0.0f;
        float top = static_cast<float>(m_windowHeight);
        
        // Create orthographic projection matrix manually
        float projection[16] = {
            2.0f / (right - left), 0.0f, 0.0f, 0.0f,
            0.0f, 2.0f / (top - bottom), 0.0f, 0.0f,
            0.0f, 0.0f, -1.0f, 0.0f,
            -(right + left) / (right - left), -(top + bottom) / (top - bottom), 0.0f, 1.0f
        };
        
        // Set the projection uniform
        GLint projLoc = glGetUniformLocation(m_uiShader->GetID(), "projection");
        if (projLoc != -1) {
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection);
        }
    }
    
    RenderPanel();
    RenderButtons();
    RenderDebugInfo();
    
    // Restore OpenGL state
    glViewport(prevViewport[0], prevViewport[1], prevViewport[2], prevViewport[3]);
}

void UIComponent::RenderPanel() {
    std::cout << "Rendering panel: " << m_panelWidth << "x" << m_windowHeight << std::endl;
    
    // White panel background
    float panelVertices[] = {
        // Position (x, y)    // Color (r, g, b)
        0.0f, 0.0f,          0.95f, 0.95f, 0.95f,
        static_cast<float>(m_panelWidth), 0.0f,  0.95f, 0.95f, 0.95f,
        static_cast<float>(m_panelWidth), static_cast<float>(m_windowHeight), 0.95f, 0.95f, 0.95f,
        0.0f, static_cast<float>(m_windowHeight), 0.95f, 0.95f, 0.95f
    };
    
    glBindVertexArray(m_uiVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_uiVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(panelVertices), panelVertices, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    
    // Panel border - make it thicker and more visible
    float borderVertices[] = {
        // Position (x, y)    // Color (r, g, b)
        0.0f, 0.0f,          0.7f, 0.7f, 0.7f,
        static_cast<float>(m_panelWidth), 0.0f,  0.7f, 0.7f, 0.7f,
        static_cast<float>(m_panelWidth), static_cast<float>(m_windowHeight), 0.7f, 0.7f, 0.7f,
        0.0f, static_cast<float>(m_windowHeight), 0.7f, 0.7f, 0.7f
    };
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(borderVertices), borderVertices, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    
    // Title bar
    float titleVertices[] = {
        // Position (x, y)    // Color (r, g, b)
        0.0f, static_cast<float>(m_windowHeight - 30), 0.8f, 0.8f, 0.8f,
        static_cast<float>(m_panelWidth), static_cast<float>(m_windowHeight - 30), 0.8f, 0.8f, 0.8f,
        static_cast<float>(m_panelWidth), static_cast<float>(m_windowHeight), 0.8f, 0.8f, 0.8f,
        0.0f, static_cast<float>(m_windowHeight), 0.8f, 0.8f, 0.8f
    };
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(titleVertices), titleVertices, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    
    // Title border
    float titleBorderVertices[] = {
        // Position (x, y)    // Color (r, g, b)
        0.0f, static_cast<float>(m_windowHeight - 30), 0.5f, 0.5f, 0.5f,
        static_cast<float>(m_panelWidth), static_cast<float>(m_windowHeight - 30), 0.5f, 0.5f, 0.5f
    };
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(titleBorderVertices), titleBorderVertices, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_LINES, 0, 2);
    
    glBindVertexArray(0);
}

void UIComponent::RenderButtons() {
    std::cout << "Rendering " << m_buttons.size() << " buttons" << std::endl;
    
    for (const auto& button : m_buttons) {
        std::cout << "Button: " << button.text << " at (" << button.position.x << ", " << button.position.y 
                  << ") size (" << button.size.x << ", " << button.size.y << ")" << std::endl;
        
        // Button background - make it much more visible
        float r, g, b;
        if (button.isSelected) {
            r = 0.1f; g = 0.7f; b = 1.0f; // Bright blue when selected
        } else {
            r = 0.3f; g = 0.3f; b = 0.8f; // Purple/blue for better visibility
        }
        
        float buttonVertices[] = {
            // Position (x, y)    // Color (r, g, b)
            button.position.x, button.position.y, r, g, b,
            button.position.x + button.size.x, button.position.y, r, g, b,
            button.position.x + button.size.x, button.position.y + button.size.y, r, g, b,
            button.position.x, button.position.y + button.size.y, r, g, b
        };
        
        glBindVertexArray(m_uiVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_uiVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(buttonVertices), buttonVertices, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        
        // Button border - make it dark and visible
        float borderVertices[] = {
            // Position (x, y)    // Color (r, g, b)
            button.position.x, button.position.y, 0.0f, 0.0f, 0.0f,  // Black border
            button.position.x + button.size.x, button.position.y, 0.0f, 0.0f, 0.0f,
            button.position.x + button.size.x, button.position.y + button.size.y, 0.0f, 0.0f, 0.0f,
            button.position.x, button.position.y + button.size.y, 0.0f, 0.0f, 0.0f
        };
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(borderVertices), borderVertices, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        
        // Button text (simplified - just draw colored rectangles for letters)
        if (button.text == "Point") {
            // Draw "P" as a simple rectangle
            float pVertices[] = {
                button.position.x + 10, button.position.y + 10, 0.0f, 0.0f, 0.0f,
                button.position.x + 20, button.position.y + 10, 0.0f, 0.0f, 0.0f,
                button.position.x + 20, button.position.y + 20, 0.0f, 0.0f, 0.0f,
                button.position.x + 10, button.position.y + 20, 0.0f, 0.0f, 0.0f
            };
            glBufferData(GL_ARRAY_BUFFER, sizeof(pVertices), pVertices, GL_DYNAMIC_DRAW);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        } else if (button.text == "Line") {
            // Draw "L" as a simple rectangle
            float lVertices[] = {
                button.position.x + 10, button.position.y + 10, 0.0f, 0.0f, 0.0f,
                button.position.x + 20, button.position.y + 10, 0.0f, 0.0f, 0.0f,
                button.position.x + 20, button.position.y + 20, 0.0f, 0.0f, 0.0f,
                button.position.x + 10, button.position.y + 20, 0.0f, 0.0f, 0.0f
            };
            glBufferData(GL_ARRAY_BUFFER, sizeof(lVertices), lVertices, GL_DYNAMIC_DRAW);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        }
        
        glBindVertexArray(0);
    }
}

void UIComponent::RenderText(const std::string& text, float x, float y, float scale) {
    std::cout << "Rendering text: " << text << " at (" << x << ", " << y << ")" << std::endl;
    
    // Simple text rendering using colored rectangles
    std::vector<float> charVertices;
    
    for (size_t i = 0; i < text.length(); ++i) {
        char c = text[i];
        float charX = x + i * 10 * scale;
        float charY = y;
        
        // Create a simple rectangle for each character
        float charWidth = 8 * scale;
        float charHeight = 12 * scale;
        
        // Rectangle vertices (position + color)
        charVertices.insert(charVertices.end(), {
            charX, charY, 0.0f, 0.0f, 0.0f,
            charX + charWidth, charY, 0.0f, 0.0f, 0.0f,
            charX + charWidth, charY + charHeight, 0.0f, 0.0f, 0.0f,
            charX, charY + charHeight, 0.0f, 0.0f, 0.0f
        });
    }
    
    if (!charVertices.empty()) {
        glBindVertexArray(m_uiVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_uiVBO);
        glBufferData(GL_ARRAY_BUFFER, charVertices.size() * sizeof(float), charVertices.data(), GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLE_FAN, 0, charVertices.size() / 5);
        glBindVertexArray(0);
    }
}

void UIComponent::SetCurrentTool(Tool tool) {
    m_currentTool = tool;
    
    // Update button selection states
    for (auto& button : m_buttons) {
        button.isSelected = (button.tool == tool);
    }
}

bool UIComponent::IsPointInsidePanel(float x, float y) const {
    return x >= 0 && x <= m_panelWidth && y >= 0 && y <= m_windowHeight;
}

Button* UIComponent::GetButtonAt(float x, float y) {
    for (auto& button : m_buttons) {
        if (button.IsPointInside(glm::vec2(x, y))) {
            return &button;
        }
    }
    return nullptr;
}

bool UIComponent::HandleMouseClick(float x, float y) {
    Button* button = GetButtonAt(x, y);
    if (button) {
        SetCurrentTool(button->tool);
        return true;
    }
    return false;
}

void UIComponent::UpdateWindowSize(int width, int height) {
    m_windowWidth = width;
    m_windowHeight = height;
}

void UIComponent::RenderDebugInfo() {
    // Render some simple text using colored rectangles
    
    // Title text "MESH ENGINE" at the top
    float titleY = m_windowHeight - 20;
    
    // Letter "M" 
    float letterM[] = {
        // Position (x, y)    // Color (r, g, b) - black text
        20.0f, titleY - 15, 0.0f, 0.0f, 0.0f,
        25.0f, titleY - 15, 0.0f, 0.0f, 0.0f,
        25.0f, titleY, 0.0f, 0.0f, 0.0f,
        20.0f, titleY, 0.0f, 0.0f, 0.0f
    };
    
    glBindVertexArray(m_uiVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_uiVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(letterM), letterM, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    
    // Letter "E"
    float letterE[] = {
        30.0f, titleY - 15, 0.0f, 0.0f, 0.0f,
        35.0f, titleY - 15, 0.0f, 0.0f, 0.0f,
        35.0f, titleY, 0.0f, 0.0f, 0.0f,
        30.0f, titleY, 0.0f, 0.0f, 0.0f
    };
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(letterE), letterE, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    
    // Add some colored rectangles as "buttons" to make sure something is visible
    
    // Red rectangle button
    float redButton[] = {
        10.0f, m_windowHeight - 80, 1.0f, 0.0f, 0.0f,  // Red
        130.0f, m_windowHeight - 80, 1.0f, 0.0f, 0.0f,
        130.0f, m_windowHeight - 50, 1.0f, 0.0f, 0.0f,
        10.0f, m_windowHeight - 50, 1.0f, 0.0f, 0.0f
    };
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(redButton), redButton, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    
    // Green rectangle button  
    float greenButton[] = {
        10.0f, m_windowHeight - 130, 0.0f, 1.0f, 0.0f,  // Green
        130.0f, m_windowHeight - 130, 0.0f, 1.0f, 0.0f,
        130.0f, m_windowHeight - 100, 0.0f, 1.0f, 0.0f,
        10.0f, m_windowHeight - 100, 0.0f, 1.0f, 0.0f
    };
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(greenButton), greenButton, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    
    // Blue rectangle button
    float blueButton[] = {
        10.0f, m_windowHeight - 180, 0.0f, 0.0f, 1.0f,  // Blue
        130.0f, m_windowHeight - 180, 0.0f, 0.0f, 1.0f,
        130.0f, m_windowHeight - 150, 0.0f, 0.0f, 1.0f,
        10.0f, m_windowHeight - 150, 0.0f, 0.0f, 1.0f
    };
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(blueButton), blueButton, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    
    // Add some simple "text" using rectangles
    // "POINT" label on red button
    for (int i = 0; i < 5; i++) {
        float textPixel[] = {
            20.0f + i * 8, m_windowHeight - 75, 1.0f, 1.0f, 1.0f,  // White text
            22.0f + i * 8, m_windowHeight - 75, 1.0f, 1.0f, 1.0f,
            22.0f + i * 8, m_windowHeight - 70, 1.0f, 1.0f, 1.0f,
            20.0f + i * 8, m_windowHeight - 70, 1.0f, 1.0f, 1.0f
        };
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(textPixel), textPixel, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
    
    glBindVertexArray(0);
} 