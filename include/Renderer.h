#ifndef RENDERER_H
#define RENDERER_H

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <memory>

class Renderer {
public:
    Renderer();
    virtual ~Renderer();
    
    virtual void Initialize() = 0;
    virtual void Render() = 0;
    virtual void Update() = 0;
    
    // Common rendering utilities
    void SetViewport(int x, int y, int width, int height);
    void Clear(float r, float g, float b, float a);
    void EnableBlending();
    void DisableBlending();
    
protected:
    int m_viewportX, m_viewportY, m_viewportWidth, m_viewportHeight;
    bool m_initialized;
};

#endif 