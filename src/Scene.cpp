#include <engine/Scene.h>
#include <engine/Light.h>

Scene::Scene(std::string name, Window* window, std::vector<GameObject*> gameObjects) {
    this->name = name;
    this->window = window;
    this->gameObjects = gameObjects;
    this->renderer = new Renderer3D(window);
    
    // Initialize default point light
    PointLight* pointLight = new PointLight{glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.75f, 0.25f, 0.65f), glm::vec3(2.0f, 2.0f, 2.0f)};
    lightManager->AddLight(pointLight);
}

void Scene::SetCity(CityGenerator* city, unsigned int cityVAO, unsigned int cityVBO, unsigned int cityVertexCount, Shader* cityShader) {
    this->city = city;
    this->cityVAO = cityVAO;
    this->cityVBO = cityVBO;
    this->cityVertexCount = cityVertexCount;
    this->cityShader = cityShader;
}

void Scene::SetAlleyways(unsigned int alleyVAO, unsigned int alleyVBO, unsigned int alleyVertexCount) {
    this->alleyVAO = alleyVAO;
    this->alleyVBO = alleyVBO;
    this->alleyVertexCount = alleyVertexCount;
}

void Scene::SetOutskirts(unsigned int outskirtsVAO, unsigned int outskirtsVBO, unsigned int outskirtsVertexCount) {
    this->outskirtsVAO = outskirtsVAO;
    this->outskirtsVBO = outskirtsVBO;
    this->outskirtsVertexCount = outskirtsVertexCount;
}

void Scene::SetBuildings(unsigned int buildingVAO, unsigned int buildingVBO, unsigned int buildingVertexCount) {
    this->buildingVAO = buildingVAO;
    this->buildingVBO = buildingVBO;
    this->buildingVertexCount = buildingVertexCount;
}
