#include "Application.h"
#include <iostream>

Application::Application(int width, int height, const std::string& title)
    : m_width(width), m_height(height), m_title(title), m_window(nullptr), m_firstMouse(true),
      m_zoomLevel(1.0f), m_minZoom(0.1f), m_maxZoom(10.0f) {
    std::cout << "Starting MeshEngine..." << std::endl;
}

Application::~Application() {
    Shutdown();
}

bool Application::Initialize() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    if (!m_window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, this);

    // Set up scroll callback for forward/backward movement
    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) {
        Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if (app) {
            app->HandleForwardBackward(yoffset);
        }
    });
    
    // Set up window resize callback
    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
        Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if (app) {
            app->HandleWindowResize(width, height);
        }
    });

    // Initialize GLAD
    if (!gladLoadGL((void* (*)(const char*))glfwGetProcAddress)) {
        std::cerr << "Failed to initialize OpenGL" << std::endl;
        glfwTerminate();
        return false;
    }

    // Configure OpenGL
    glViewport(0, 0, m_width, m_height);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Initialize scene and UI
    m_scene = std::make_unique<Scene>();
    m_scene->Initialize();
    
    m_ui = std::make_unique<UIComponent>(m_width, m_height);
    m_ui->Initialize();

    return true;
}

void Application::Run() {
    while (!glfwWindowShouldClose(m_window)) {
        ProcessInput();
        Render();
        m_ui->Update();
        glfwPollEvents();
    }
}

void Application::Shutdown() {
    if (m_window) {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }
}

void Application::ProcessInput() {
    // Handle mouse input for camera
    static bool mousePressed = false;
    static bool rightMousePressed = false;
    
    double mouseX, mouseY;
    glfwGetCursorPos(m_window, &mouseX, &mouseY);
    
    // Handle right mouse button for camera movement
    if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        if (!rightMousePressed) {
            m_lastMouseX = mouseX;
            m_lastMouseY = mouseY;
            rightMousePressed = true;
        }
        
        float xoffset = mouseX - m_lastMouseX;
        float yoffset = m_lastMouseY - mouseY;
        
        m_lastMouseX = mouseX;
        m_lastMouseY = mouseY;
        
        m_scene->GetCamera().ProcessMouseMovement(xoffset, yoffset);
    } else {
        rightMousePressed = false;
    }
    
    // Handle keyboard forward/backward as fallback
    if (glfwGetKey(m_window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
        HandleForwardBackward(1.0); // Move forward
    } else if (glfwGetKey(m_window, GLFW_KEY_MINUS) == GLFW_PRESS) {
        HandleForwardBackward(-1.0); // Move backward
    }
    
    // Handle arrow key camera movement
    float cameraSpeed = 0.1f; // Adjust this value to control movement speed
    
    if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS) {
        // Move camera up along Y axis
        m_scene->GetCamera().ProcessKeyboard(4, cameraSpeed); // UP
    }
    if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        // Move camera down along Y axis
        m_scene->GetCamera().ProcessKeyboard(5, cameraSpeed); // DOWN
    }
    if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        // Move camera left along X axis
        m_scene->GetCamera().ProcessKeyboard(2, cameraSpeed); // LEFT
    }
    if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        // Move camera right along X axis
        m_scene->GetCamera().ProcessKeyboard(3, cameraSpeed); // RIGHT
    }
    
    // Handle left mouse button for tool interaction
    if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (!mousePressed) {
            mousePressed = true;
            
            // Check if click is in UI panel area
            if (m_ui->HandleMouseClick(mouseX, mouseY)) {
                // UI handled the click
                return;
            }
            
            // Check if click is in graphics area (right side of window)
            int panelWidth = 200;
            if (mouseX >= panelWidth) {
                // Adjust mouse coordinates for graphics area
                double adjustedMouseX = mouseX - panelWidth;
                double adjustedMouseY = mouseY;
                
                // Convert screen coordinates to world coordinates
                float normalizedX = (2.0f * adjustedMouseX) / (m_width - panelWidth) - 1.0f;
                float normalizedY = 1.0f - (2.0f * adjustedMouseY) / m_height;
                
                Tool currentTool = m_ui->GetCurrentTool();
                
                if (currentTool == Tool::Point) {
                    // Add point at mouse position
                    glm::vec3 worldPos(normalizedX * 5.0f, normalizedY * 5.0f, 0.0f);
                    m_scene->AddPoint(worldPos);
                } else if (currentTool == Tool::Line) {
                    // Handle line creation by selecting points
                    int pointIndex = m_scene->GetPointAtScreenPosition(adjustedMouseX, adjustedMouseY, m_width - panelWidth, m_height);
                    if (pointIndex >= 0) {
                        if (!m_ui->IsAddingLine()) {
                            // Start line creation
                            m_ui->SetAddingLine(true);
                            m_ui->SetFirstPointIndex(pointIndex);
                        } else {
                            // Complete line creation
                            if (m_ui->GetFirstPointIndex() != pointIndex) {
                                // Create line between the two points
                                const auto& points = m_scene->GetPoints();
                                if (m_ui->GetFirstPointIndex() < points.size() && pointIndex < points.size()) {
                                    const auto& p1 = points[m_ui->GetFirstPointIndex()]->GetPosition();
                                    const auto& p2 = points[pointIndex]->GetPosition();
                                    m_scene->AddLine(p1, p2);
                                }
                            }
                            // Reset line creation state
                            m_ui->SetAddingLine(false);
                            m_ui->SetFirstPointIndex(-1);
                        }
                    }
                }
            }
        }
    } else {
        mousePressed = false;
    }
    
    // Handle keyboard input for tool selection
    if (glfwGetKey(m_window, GLFW_KEY_1) == GLFW_PRESS) {
        m_ui->SetTool(Tool::Point);
    } else if (glfwGetKey(m_window, GLFW_KEY_2) == GLFW_PRESS) {
        m_ui->SetTool(Tool::Line);
    }
}

void Application::HandleForwardBackward(double yoffset) {
    const float movementSpeed = 0.1f;
    float deltaTime = movementSpeed;
    
    if (yoffset > 0) {
        m_scene->GetCamera().ProcessKeyboard(0, deltaTime); // FORWARD
    } else if (yoffset < 0) {
        m_scene->GetCamera().ProcessKeyboard(1, deltaTime); // BACKWARD
    }
}

void Application::HandleWindowResize(int width, int height) {
    m_width = width;
    m_height = height;
    
    // Update UI component with new window size
    m_ui->UpdateWindowSize(width, height);
    
    // Update scene viewport for graphics area
    int panelWidth = 200;
    int graphicsWidth = width - panelWidth;
    int graphicsHeight = height;
    m_scene->UpdateViewport(graphicsWidth, graphicsHeight);
    
    // Update viewport
    glViewport(0, 0, width, height);
    
    std::cout << "Window resized to: " << width << "x" << height << " (Graphics: " << graphicsWidth << "x" << graphicsHeight << ")" << std::endl;
}

void Application::Render() {
    // Clear the screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Render graphics in the right portion of the window
    RenderGraphics();
    
    // Render UI panel on the left - render this last to ensure it's on top
    m_ui->Render();
    
    // Swap buffers
    glfwSwapBuffers(m_window);
}

void Application::RenderGraphics() {
    // Set viewport for graphics area (right side of window)
    int panelWidth = 200; // Should match UIComponent's panel width
    int graphicsWidth = m_width - panelWidth;
    int graphicsHeight = m_height;
    
    glViewport(panelWidth, 0, graphicsWidth, graphicsHeight);
    
    // Update scene camera with new viewport dimensions
    m_scene->UpdateViewport(graphicsWidth, graphicsHeight);
    
    // Render scene in the graphics area
    m_scene->Render();
    
    // Render version number in bottom right of graphics area
    RenderVersionNumber();
    
    // Reset viewport to full window for UI rendering
    glViewport(0, 0, m_width, m_height);
}

void Application::RenderVersionNumber() {
    std::cout << "Rendering version number..." << std::endl;
    
    // Save current OpenGL state
    GLint prevViewport[4];
    glGetIntegerv(GL_VIEWPORT, prevViewport);
    
    // Set up 2D orthographic projection for version text
    int panelWidth = 200;
    int graphicsWidth = m_width - panelWidth;
    int graphicsHeight = m_height;
    
    std::cout << "Graphics area: " << graphicsWidth << "x" << graphicsHeight << std::endl;
    
    glViewport(panelWidth, 0, graphicsWidth, graphicsHeight);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Use immediate mode for simple rendering
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, graphicsWidth, graphicsHeight, 0, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    // TEST: Draw a large bright green rectangle in the center to verify our changes are working
    float centerX = graphicsWidth / 2.0f - 100;
    float centerY = graphicsHeight / 2.0f - 50;
    float width = 200;
    float height = 100;
    
    std::cout << "Drawing test rectangle at (" << centerX << ", " << centerY << ")" << std::endl;
    
    // Draw bright green test rectangle
    glColor3f(0.0f, 1.0f, 0.0f); // Bright green
    glBegin(GL_QUADS);
    glVertex2f(centerX, centerY);
    glVertex2f(centerX + width, centerY);
    glVertex2f(centerX + width, centerY + height);
    glVertex2f(centerX, centerY + height);
    glEnd();
    
    // Draw white border around test rectangle
    glColor3f(1.0f, 1.0f, 1.0f); // White border
    glLineWidth(5.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(centerX, centerY);
    glVertex2f(centerX + width, centerY);
    glVertex2f(centerX + width, centerY + height);
    glVertex2f(centerX, centerY + height);
    glEnd();
    
    // Render a very prominent version indicator - make it much larger and more visible
    float yPos = graphicsHeight - 80;
    float xPos = graphicsWidth - 200;
    float versionWidth = 180;
    float versionHeight = 50;
    
    std::cout << "Version position: (" << xPos << ", " << yPos << ")" << std::endl;
    std::cout << "Version text: " << MESHENGINE_VERSION << std::endl;
    
    // Draw bright red background rectangle for maximum visibility
    glColor3f(1.0f, 0.0f, 0.0f); // Bright red background
    glBegin(GL_QUADS);
    glVertex2f(xPos, yPos);
    glVertex2f(xPos + versionWidth, yPos);
    glVertex2f(xPos + versionWidth, yPos + versionHeight);
    glVertex2f(xPos, yPos + versionHeight);
    glEnd();
    
    // Draw thick white border
    glColor3f(1.0f, 1.0f, 1.0f); // White border
    glLineWidth(4.0f); // Much thicker lines
    glBegin(GL_LINE_LOOP);
    glVertex2f(xPos, yPos);
    glVertex2f(xPos + versionWidth, yPos);
    glVertex2f(xPos + versionWidth, yPos + versionHeight);
    glVertex2f(xPos, yPos + versionHeight);
    glEnd();
    
    // Draw simple version text as lines - make it much larger
    glColor3f(1.0f, 1.0f, 1.0f); // White text
    glLineWidth(3.0f); // Thicker text lines
    
    // Draw "v1.0.0" as simple lines - make it larger
    float textX = xPos + 20;
    float textY = yPos + 15;
    
    // Draw "v" - make it larger
    glBegin(GL_LINES);
    glVertex2f(textX, textY + 10); glVertex2f(textX + 16, textY + 30);
    glVertex2f(textX + 16, textY + 30); glVertex2f(textX + 32, textY + 10);
    glEnd();
    
    // Draw "1" - make it larger
    textX += 40;
    glBegin(GL_LINES);
    glVertex2f(textX + 16, textY); glVertex2f(textX + 16, textY + 30);
    glEnd();
    
    // Draw "." - make it larger
    textX += 24;
    glBegin(GL_LINES);
    glVertex2f(textX + 8, textY + 10); glVertex2f(textX + 8, textY + 10);
    glEnd();
    
    // Draw "0" - make it larger
    textX += 16;
    glBegin(GL_LINE_LOOP);
    glVertex2f(textX, textY + 10); glVertex2f(textX + 16, textY + 10);
    glVertex2f(textX + 16, textY + 30); glVertex2f(textX, textY + 30);
    glEnd();
    
    // Draw "." - make it larger
    textX += 24;
    glBegin(GL_LINES);
    glVertex2f(textX + 8, textY + 10); glVertex2f(textX + 8, textY + 10);
    glEnd();
    
    // Draw "0" - make it larger
    textX += 16;
    glBegin(GL_LINE_LOOP);
    glVertex2f(textX, textY + 10); glVertex2f(textX + 16, textY + 10);
    glVertex2f(textX + 16, textY + 30); glVertex2f(textX, textY + 30);
    glEnd();
    
    glLineWidth(1.0f);
    
    // Restore OpenGL state
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
    glViewport(prevViewport[0], prevViewport[1], prevViewport[2], prevViewport[3]);
    glDisable(GL_BLEND);
    
    std::cout << "Version rendering completed" << std::endl;
} 