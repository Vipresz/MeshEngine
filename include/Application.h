#ifndef APPLICATION_H
#define APPLICATION_H

#include <GLFW/glfw3.h>
#include <memory>
#include <vector>
#include <string>
#include <glad/gl.h>
#include "Scene.h"
#include "UIComponent.h"

// Version information
#define MESHENGINE_VERSION "v1.0.0"

class Application {
public:
    Application(int width, int height, const std::string& title);
    ~Application();
    
    bool Initialize();
    void Run();
    void Shutdown();
    
private:
    void ProcessInput();
    void HandleForwardBackward(double yoffset);
    void HandleWindowResize(int width, int height);
    void Render();
    void RenderGraphics();
    void RenderVersionNumber();
    void RenderUI();
    
    // Window properties
    int m_width, m_height;
    std::string m_title;
    GLFWwindow* m_window;
    
    // Scene and UI
    std::unique_ptr<Scene> m_scene;
    std::unique_ptr<UIComponent> m_ui;
    
    // Input state
    double m_lastMouseX, m_lastMouseY;
    bool m_firstMouse;
    
    // Zoom state
    float m_zoomLevel;
    float m_minZoom, m_maxZoom;
};

#endif 