#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
    : m_position(0.0f, 0.0f, 3.0f)
    , m_front(0.0f, 0.0f, -1.0f)
    , m_up(0.0f, 1.0f, 0.0f)
    , m_worldUp(0.0f, 1.0f, 0.0f)
    , m_yaw(-90.0f)
    , m_pitch(0.0f)
    , m_movementSpeed(2.5f)
    , m_mouseSensitivity(0.1f)
    , m_zoom(45.0f)
    , m_aspectRatio(1200.0f / 800.0f) {
    UpdateCameraVectors();
}

Camera::Camera(const glm::vec3& position)
    : m_position(position)
    , m_front(0.0f, 0.0f, -1.0f)
    , m_up(0.0f, 1.0f, 0.0f)
    , m_worldUp(0.0f, 1.0f, 0.0f)
    , m_yaw(-90.0f)
    , m_pitch(0.0f)
    , m_movementSpeed(2.5f)
    , m_mouseSensitivity(0.1f)
    , m_zoom(45.0f)
    , m_aspectRatio(1200.0f / 800.0f) {
    UpdateCameraVectors();
}

void Camera::Update() {
    UpdateCameraVectors();
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= m_mouseSensitivity;
    yoffset *= m_mouseSensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    if (constrainPitch) {
        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;
    }

    UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
    m_zoom -= yoffset;
    if (m_zoom < -45.0f)
        m_zoom = -45.0f;
    if (m_zoom > 45.0f)
        m_zoom = 45.0f;
}

void Camera::ProcessKeyboard(int direction, float deltaTime) {
    float velocity = m_movementSpeed * deltaTime;
    
    switch (direction) {
        case 0: // FORWARD
            m_position += m_front * velocity;
            break;
        case 1: // BACKWARD
            m_position -= m_front * velocity;
            break;
        case 2: // LEFT
            m_position -= m_right * velocity;
            break;
        case 3: // RIGHT
            m_position += m_right * velocity;
            break;
        case 4: // UP
            m_position += m_up * velocity;
            break;
        case 5: // DOWN
            m_position -= m_up * velocity;
            break;
    }
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::mat4 Camera::GetProjectionMatrix() const {
    return glm::perspective(glm::radians(m_zoom), m_aspectRatio, 0.1f, 100.0f);
}

void Camera::UpdateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);
    
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

void Camera::SetAspectRatio(float aspectRatio) {
    m_aspectRatio = aspectRatio;
} 