#pragma once
#include <vector>
#include <engine/GameObject.h>
#include <engine/Shader.h>
#include <engine/Camera.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <engine/Renderer.h>
#include <engine/Light.h>
#include <engine/Procedural.h>
#include <glad/glad.h>

class Scene{
    public:
        Scene(std::string name, Window* window, std::vector<GameObject*> gameObjects = {});
        ~Scene(){};
        void AddGameObject(GameObject* gameObject){gameObjects.push_back(gameObject);}
        // void RemoveGameObject(GameObject* gameObject){gameObjects.pop_back();};
        void Update(float deltaTime);
        virtual void Draw(){
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
                2000.0f // Increased far plane for larger city
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
            };
            
            // Render city main roads if available
            if(city && cityVAO != 0 && cityShader && cityVertexCount > 0) {
                glDisable(GL_DEPTH_TEST); // Disable depth test for city overlay
                cityShader->Use();
                cityShader->SetMat4("view", view);
                cityShader->SetMat4("projection", projection);
                cityShader->SetMat4("model", glm::mat4(1.0f));
                cityShader->SetFloat("roadWidth", 3.0f);
                cityShader->SetVec3("roadColor", glm::vec3(1.0f, 1.0f, 1.0f)); // White for main roads
                
                glBindVertexArray(cityVAO);
                glDrawArrays(GL_LINES, 0, cityVertexCount);
                glBindVertexArray(0);
                
                // Render alleyways if available (blue, narrower)
                if(alleyVAO != 0 && alleyVertexCount > 0) {
                    cityShader->SetFloat("roadWidth", 1.5f); // Narrower for alleyways
                    cityShader->SetVec3("roadColor", glm::vec3(0.2f, 0.4f, 1.0f)); // Blue for alleyways
                    
                    glBindVertexArray(alleyVAO);
                    glDrawArrays(GL_LINES, 0, alleyVertexCount);
                    glBindVertexArray(0);
                }
                
                // Render outskirts roads if available (green)
                if(outskirtsVAO != 0 && outskirtsVertexCount > 0) {
                    cityShader->SetFloat("roadWidth", 3.0f); // Same width as main roads
                    cityShader->SetVec3("roadColor", glm::vec3(0.2f, 1.0f, 0.2f)); // Green for outskirts
                    
                    glBindVertexArray(outskirtsVAO);
                    glDrawArrays(GL_LINES, 0, outskirtsVertexCount);
                    glBindVertexArray(0);
                }
                
                glEnable(GL_DEPTH_TEST);
            }
            
            // Render buildings if available
            // Buildings are rendered as simple cubes using the default lighting shader
            if(buildingVAO != 0 && buildingVertexCount > 0 && defaultShader) {
                defaultShader->Use();
                defaultShader->SetMat4("view", view);
                defaultShader->SetMat4("projection", projection);
                defaultShader->SetMat4("model", glm::mat4(1.0f));
                defaultShader->SetVec3("viewPos", mainCamera->GetPosition());
                lightManager->Draw(defaultShader); // Apply lighting to buildings
                
                glBindVertexArray(buildingVAO);
                glDrawArrays(GL_TRIANGLES, 0, buildingVertexCount); // Draw all building cubes
                glBindVertexArray(0);
            }
        }
        Camera* mainCamera = new Camera();
        LightManager* lightManager = new LightManager();
        
        void SetCity(CityGenerator* city, unsigned int cityVAO, unsigned int cityVBO, unsigned int cityVertexCount, Shader* cityShader);
        void SetAlleyways(unsigned int alleyVAO, unsigned int alleyVBO, unsigned int alleyVertexCount);
        void SetOutskirts(unsigned int outskirtsVAO, unsigned int outskirtsVBO, unsigned int outskirtsVertexCount);
        void SetBuildings(unsigned int buildingVAO, unsigned int buildingVBO, unsigned int buildingVertexCount);
        
    protected:
        std::vector<GameObject*> gameObjects;
        std::string name;
        Window* window;
        Shader* defaultShader = new Shader("shaders/container.vs", "shaders/lighting.fs");
        Renderer3D* renderer;
        
        // City rendering data
        CityGenerator* city = nullptr;
        unsigned int cityVAO = 0;
        unsigned int cityVBO = 0;
        unsigned int cityVertexCount = 0;
        Shader* cityShader = nullptr;
        
        // Alleyway rendering data
        unsigned int alleyVAO = 0;
        unsigned int alleyVBO = 0;
        unsigned int alleyVertexCount = 0;
        
        // Outskirts rendering data
        unsigned int outskirtsVAO = 0;
        unsigned int outskirtsVBO = 0;
        unsigned int outskirtsVertexCount = 0;
        
        // Building rendering data
        unsigned int buildingVAO = 0;
        unsigned int buildingVBO = 0;
        unsigned int buildingVertexCount = 0;
};

// class Scene3D : public Scene {
//     public:
//         Scene3D(std::string name, std::vector<GameObject*> gameObjects = {}) : Scene(name, gameObjects) {}
//         ~Scene3D() {}
// };
