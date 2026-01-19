#pragma once
#include <glm/glm.hpp>

class Light {
    public:
        Light(glm::vec3 position, glm::vec3 color, float intensity);
        ~Light();
        glm::vec3 position;
        glm::vec3 color;
        float intensity;
    private:
        glm::vec3 position;
        glm::vec3 color;
        float intensity;
};