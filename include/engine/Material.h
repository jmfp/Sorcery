#pragma once
#include <engine/Component.h>
#include <engine/Texture.h>
#include <engine/Shader.h>
#include <vector>
#include <glad/glad.h>

class Material {
    public:
        Material(std::vector<Texture*> textures, float shininess = 32.0f) 
            : textures(textures), shininess(shininess) {}
        
        Material() : shininess(32.0f) {}
        ~Material() {}
        
        void Bind(Shader* shader) {
            if (!shader) return;
            
            // Ensure shader is active before setting uniforms
            shader->Use();
            
            unsigned int textureUnit = 0;
            for(auto* tex : textures) {
                if (!tex) continue;
                
                TextureType type = tex->GetType();
                
                // Bind texture to texture unit
                glActiveTexture(GL_TEXTURE0 + textureUnit);
                glBindTexture(GL_TEXTURE_2D, tex->GetTexture());
                
                // Set uniform to point to this texture unit
                if(type == TextureType::DIFFUSE) {
                    shader->SetInt("material.diffuse", textureUnit);
                } else if(type == TextureType::SPECULAR) {
                    shader->SetInt("material.specular", textureUnit);
                } else if(type == TextureType::EMISSION) {
                    shader->SetInt("material.emission", textureUnit);
                } else if(type == TextureType::NORMAL) {
                    shader->SetInt("material.normal", textureUnit);
                } else if(type == TextureType::HEIGHT) {
                    shader->SetInt("material.height", textureUnit);
                }
                textureUnit++;
            }
            shader->SetFloat("material.shininess", shininess);
        }
        
        float shininess;
    private:
        std::vector<Texture*> textures;
};