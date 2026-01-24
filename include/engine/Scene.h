#pragma once
#include <vector>
#include <engine/GameObject.h>
#include <engine/Shader.h>
#include <engine/Camera.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <engine/Renderer.h>
#include <engine/Light.h>

class Scene{
    public:
        Scene(std::string name, Window* window, std::vector<GameObject*> gameObjects = {});
        ~Scene(){};
        void AddGameObject(GameObject* gameObject){gameObjects.push_back(gameObject);}
        // void RemoveGameObject(GameObject* gameObject){gameObjects.pop_back();};
        void Update(float deltaTime);
        virtual void Draw(){
            static float lastFrame = 0.0f;
            float currentFrame = glfwGetTime();
            float deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            if (!defaultShader || !mainCamera){
                std::cerr << "Scene Draw Error: Missing default shader or main camera." << std::endl;
                return;
            }
            glm::mat4 view = mainCamera->GetViewMatrix();
            float aspectRatio = 1920.0f / 1080.0f;
        
            glm::mat4 projection = glm::perspective(
                glm::radians(mainCamera->GetZoom()),
                aspectRatio,
                0.1f,
                100.0f
            );


            
            // Activate shader and set world space matrices
            defaultShader->Use();
            defaultShader->SetMat4("view", view);
            defaultShader->SetMat4("projection", projection);
            defaultShader->SetVec3("viewPos", mainCamera->GetPosition());
            lightManager->Draw(defaultShader);

            // defaultShader->SetVec3("light.position", glm::vec3(2.0f, 2.0f, 2.0f));
            // defaultShader->SetVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
            // defaultShader->SetVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
            // defaultShader->SetVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
            
            for(auto& gameObject : gameObjects){
                gameObject->Draw(defaultShader);
                gameObject->Update(deltaTime);
            };
        }
        std::vector<GameObject*> GetGameObjects(){return gameObjects;}
        Camera* mainCamera = new Camera();
        LightManager* lightManager = new LightManager();
    protected:
        std::vector<GameObject*> gameObjects;
        std::string name;
        Window* window;
        Shader* defaultShader = new Shader("shaders/container.vs", "shaders/lighting.fs");
        Renderer3D* renderer;
};

// class Scene3D : public Scene {
//     public:
//         Scene3D(std::string name, std::vector<GameObject*> gameObjects = {}) : Scene(name, gameObjects) {}
//         ~Scene3D() {}
// };