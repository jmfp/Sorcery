#include <engine/Camera.h>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch){
    this->position = position;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
    this->front = glm::vec3(0.0f, 0.0f, -1.0f);
    this->movementSpeed = 2.5f;
    this->sensitivity = 0.1f;
    this->zoom = 45.0f;
    UpdateCameraVectors();
}
Camera::~Camera(){
}
glm::mat4 Camera::GetViewMatrix(){
    return glm::lookAt(position, position + front, up);
}
void Camera::ProcessKeyboard(int direction, float deltaTime){
    float velocity = movementSpeed * deltaTime;
    if(direction == 0) // Forward (W)
        position += front * velocity;
    if(direction == 1) // Backward (S)
        position -= front * velocity;
    if(direction == 2) // Left (A)
        position -= right * velocity;
    if(direction == 3) // Right (D)
        position += right * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch){
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if(constrainPitch){
        if(pitch > 89.0f)
            pitch = 89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;
    }

    // Update front, right and up vectors using the updated Euler angles
    UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset){
    zoom -= yoffset;
    if(zoom < 1.0f)
        zoom = 1.0f;
    if(zoom > 45.0f)
        zoom = 45.0f;
}

void Camera::UpdateCameraVectors(){
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}