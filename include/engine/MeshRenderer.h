#pragma once
#include <engine/Component.h>
#include <engine/Mesh.h>
#include <glad/glad.h>
#include <engine/Transform.h>
#include <engine/GameObject.h>
#include <engine/Material.h>
#include <engine/Shader.h>

class MeshRenderer : public Component, public Drawable {
    public:
        MeshRenderer(Mesh* mesh, Material* material = nullptr) 
            : mesh(mesh), material(material) {}
        
        void SetMaterial(Material* mat) { material = mat; }
        
        virtual void Draw(Shader* shader) override {
            if (!mesh || !shader) return;
            
            glm::mat4 model = glm::mat4(1.0f);
            Transform* transform = parent->GetTransform();
            if (transform) {
                if (transform->isDirty()) {
                    transform->computeModelMatrix();
                }
                model = transform->getModelMatrix();
            }
            
            shader->SetMat4("model", model);
            
            if (material) {
                material->Bind(shader);
            }
            
            mesh->Bind();
            glDrawElements(GL_TRIANGLES, mesh->GetIndexCount(), GL_UNSIGNED_INT, 0);
            mesh->Unbind();
        }
    private:
        Mesh* mesh;
        Material* material;
};