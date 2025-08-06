#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera();
    Camera(const glm::vec3& position);

    void Update();
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void ProcessMouseScroll(float yoffset);
    void ProcessKeyboard(int direction, float deltaTime);

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;
    
    // Getters
    glm::vec3 GetPosition() const { return m_position; }
    glm::vec3 GetFront() const { return m_front; }
    glm::vec3 GetUp() const { return m_up; }
    glm::vec3 GetRight() const { return m_right; }
    float GetYaw() const { return m_yaw; }
    float GetPitch() const { return m_pitch; }
    
    // Setters
    void SetAspectRatio(float aspectRatio);

private:
    void UpdateCameraVectors();

    // Camera attributes
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;

    // Euler angles
    float m_yaw;
    float m_pitch;

    // Camera options
    float m_movementSpeed;
    float m_mouseSensitivity;
    float m_zoom;
    float m_aspectRatio;
}; 