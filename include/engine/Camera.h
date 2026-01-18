#pragma once

#include <glm/glm.hpp>

class Camera{
    public:
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw=-90.f, float pitch=0.0f);
        ~Camera();
        glm::mat4 GetViewMatrix();
        glm::vec3 GetPosition(){return position;}
        float GetYaw(){return yaw;}
        float GetZoom(){return zoom;}
        glm::vec3 GetFront(){return front;}
        void SetPosition(glm::vec3 newPosition){position = newPosition;}
        void SetFront(glm::vec3 newFront){front = newFront;}
        void SetUpDirection(glm::vec3 newUp){up = newUp;}
        void SetRight(glm::vec3 newRight){right = newRight;}
        void SetWorldUpDirection(glm::vec3 newWorldUp){worldUp = newWorldUp;}
        void SetYaw(float newYaw){yaw = newYaw;}
        void SetPitch(float newPitch){pitch = newPitch;}
        void SetMovementSpeed(float newMovementSpeed){movementSpeed = newMovementSpeed;}
        void SetZoom(float newZoom){zoom = newZoom;}
        void SetSensitivity(float newSensitivity){sensitivity = newSensitivity;}
        void ProcessKeyboard(int direction, float deltaTime);
        void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
        void ProcessMouseScroll(float yoffset);
        
    private:
        glm::vec3 position, front, up, right, worldUp;
        float yaw, pitch, movementSpeed = 0, zoom, sensitivity;
        void UpdateCameraVectors();
};