#ifndef UICOMPONENT_H
#define UICOMPONENT_H

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>
#include "Shader.h"

// Version information
#define MESHENGINE_VERSION "v1.0.0"

enum class Tool {
    Point,
    Line
};

struct Button {
    glm::vec2 position;
    glm::vec2 size;
    std::string text;
    Tool tool;
    bool isSelected;
    
    Button(const glm::vec2& pos, const glm::vec2& siz, const std::string& txt, Tool t)
        : position(pos), size(siz), text(txt), tool(t), isSelected(false) {}
    
    bool IsPointInside(const glm::vec2& point) const {
        return point.x >= position.x && point.x <= position.x + size.x &&
               point.y >= position.y && point.y <= position.y + size.y;
    }
};

class UIComponent {
public:
    UIComponent(int windowWidth, int windowHeight);
    ~UIComponent();
    
    void Initialize();
    void Update();
    void Render();
    
    Tool GetCurrentTool() const { return m_currentTool; }
    void SetCurrentTool(Tool tool);
    void SetTool(Tool tool) { SetCurrentTool(tool); }  // Alias for compatibility
    bool IsPointInsidePanel(float x, float y) const;
    Button* GetButtonAt(float x, float y);
    bool HandleMouseClick(float x, float y);
    void UpdateWindowSize(int width, int height);
    
    // Line creation state management
    bool IsAddingLine() const { return m_isAddingLine; }
    void SetAddingLine(bool adding) { m_isAddingLine = adding; }
    int GetFirstPointIndex() const { return m_firstPointIndex; }
    void SetFirstPointIndex(int index) { m_firstPointIndex = index; }
    
private:
    void CreateButtons();
    void InitializeUIShader();
    void InitializeUIBuffers();
    void RenderPanel();
    void RenderButtons();
    void RenderText(const std::string& text, float x, float y, float scale);
    void RenderDebugInfo();
    
    Tool m_currentTool;
    bool m_isAddingLine;
    int m_firstPointIndex;
    
    int m_panelWidth;
    int m_windowWidth;
    int m_windowHeight;
    
    bool m_initialized;
    std::unique_ptr<Shader> m_uiShader;
    GLuint m_uiVAO;
    GLuint m_uiVBO;
    
    std::vector<Button> m_buttons;
};

#endif 