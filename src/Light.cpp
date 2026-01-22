#include <engine/Light.h>
#include <string>

void LightManager::Draw(Shader* shader) {
    if (!shader) return;
    
    // Set directional light
    if (dirLight) {
        shader->SetVec3("dirLight.direction", dirLight->direction);
        shader->SetVec3("dirLight.ambient", dirLight->ambient);
        shader->SetVec3("dirLight.diffuse", dirLight->diffuse);
        shader->SetVec3("dirLight.specular", dirLight->specular);
    }

    // Set the number of active point lights
    int numLights = static_cast<int>(lights.size());
    shader->SetInt("numPointLights", numLights);

    // Set point lights (only set the ones that exist)
    for (size_t i = 0; i < lights.size(); ++i) {
        if (!lights[i]) continue;
        
        std::string prefix = "pointLights[" + std::to_string(i) + "]";
        shader->SetVec3(prefix + ".position", lights[i]->position);
        shader->SetVec3(prefix + ".ambient", lights[i]->ambient);
        shader->SetVec3(prefix + ".diffuse", lights[i]->diffuse);
        shader->SetVec3(prefix + ".specular", lights[i]->specular);
        shader->SetFloat(prefix + ".constant", lights[i]->constant);
        shader->SetFloat(prefix + ".linear", lights[i]->linear);
        shader->SetFloat(prefix + ".quadratic", lights[i]->quadratic);
    }
};