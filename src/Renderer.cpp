#include "Renderer.h"
#include <iostream>

Renderer::Renderer()
    : m_viewportX(0), m_viewportY(0), m_viewportWidth(800), m_viewportHeight(600), m_initialized(false) {
    std::cout << "Renderer constructor called" << std::endl;
}

Renderer::~Renderer() {
    std::cout << "Renderer destructor called" << std::endl;
}

void Renderer::SetViewport(int x, int y, int width, int height) {
    m_viewportX = x;
    m_viewportY = y;
    m_viewportWidth = width;
    m_viewportHeight = height;
    glViewport(x, y, width, height);
}

void Renderer::Clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::EnableBlending() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::DisableBlending() {
    glDisable(GL_BLEND);
} 