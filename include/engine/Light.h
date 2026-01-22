#pragma once
#include <glm/glm.hpp>
#include <engine/Shader.h>

// using structs to have contiguous data in memory so that we can pass them to the shader as a single uniform block
struct PointLight {
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
};

struct DirLight {
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

class LightManager {
    public:
        LightManager(){};
        ~LightManager(){};
        void Draw(Shader* shader);
        void AddLight(PointLight* light){lights.push_back(light);};
        void SetDirLight(DirLight* light){dirLight = light;};
        void SetDirLightPosition(glm::vec3 position){dirLight->direction = position;};
        void SetDirLightAmbient(glm::vec3 ambient){dirLight->ambient = ambient;};
        void SetDirLightDiffuse(glm::vec3 diffuse){dirLight->diffuse = diffuse;};
        void SetDirLightSpecular(glm::vec3 specular){dirLight->specular = specular;};
        std::vector<PointLight*> GetLights(){return lights;};
        DirLight* GetDirLight(){return dirLight;};
    private:
        std::vector<PointLight*> lights = {};
        DirLight* dirLight = new DirLight{glm::vec3(-0.2f, -1.0f, -0.3f),
                             glm::vec3(0.05f, 0.05f, 0.05f),
                             glm::vec3(0.4f, 0.4f, 0.4f),
                             glm::vec3(0.5f, 0.5f, 0.5f)};
};
