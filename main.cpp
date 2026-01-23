#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <engine/Window.h>
#include <engine/Shader.h>
#include <engine/Renderer.h>
#include <engine/Scene.h>
#include <engine/GameObject.h>
#include <engine/MeshRenderer.h>
#include <engine/Texture.h>
#include <engine/Material.h>
#include <engine/Procedural.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main()
{
    std::vector<Vertex> boxVerts = {
        // Front face (z = -0.5)
        Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
        Vertex{glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
        Vertex{glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
        Vertex{glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
        Vertex{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)},
        Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
        
        // Back face (z = 0.5)
        Vertex{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
        Vertex{glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
        Vertex{glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
        Vertex{glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
        Vertex{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
        Vertex{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
        
        // Left face (x = -0.5)
        Vertex{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        Vertex{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
        Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        Vertex{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
        Vertex{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        
        // Right face (x = 0.5)
        Vertex{glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        Vertex{glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
        Vertex{glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        Vertex{glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        Vertex{glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
        Vertex{glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        
        // Bottom face (y = -0.5)
        Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        Vertex{glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
        Vertex{glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        Vertex{glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        Vertex{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
        Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        
        // Top face (y = 0.5)
        Vertex{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
        Vertex{glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
        Vertex{glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        Vertex{glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        Vertex{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
        Vertex{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)}
    };
    // Initialize GLFW
    Window* window = new Window();
    window->Initialize("Witch In The Woods", 1920, 1080);
    Shader testShader = Shader("shaders/point_light.vs", "shaders/lighting.fs");
    Renderer3D renderer = Renderer3D(window);
    Scene* scene = new Scene("Test Scene", window);
    GameObject* testObject = new GameObject();
    Texture diffuseTexture = Texture("../src/textures/container2.png", 512, 512, TextureType::DIFFUSE);
    Texture specularTexture = Texture("../src/textures/specular.png", 512, 512, TextureType::SPECULAR);
    Texture emissionTexture = Texture("../src/textures/emission.jpg", 512, 512, TextureType::EMISSION);

    std::vector<Texture*> textures = {&diffuseTexture, &specularTexture, &emissionTexture};
    Material* material = new Material(textures, 32.0f);
    
    unsigned int indices[] = {
        // Front face (6 vertices)
        0, 1, 2,   // first triangle
        3, 4, 5,   // second triangle
        // Back face (6 vertices)
        6, 7, 8,   // first triangle
        9, 10, 11, // second triangle
        // Left face (6 vertices)
        12, 13, 14, // first triangle
        15, 16, 17, // second triangle
        // Right face (6 vertices)
        18, 19, 20, // first triangle
        21, 22, 23, // second triangle
        // Bottom face (6 vertices)
        24, 25, 26, // first triangle
        27, 28, 29, // second triangle
        // Top face (6 vertices)
        30, 31, 32, // first triangle
        33, 34, 35  // second triangle
    };
    std::vector<unsigned int> indicesVec(indices, indices + sizeof(indices) / sizeof(indices[0]));
    Mesh* cubeMesh = new Mesh(boxVerts, indicesVec, textures);
    MeshRenderer* meshRenderer = new MeshRenderer(cubeMesh, material);
    testObject->GetTransform()->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    testObject->AddComponent(meshRenderer);
    scene->AddGameObject(testObject);

    // Generate city - wider, less dense
    CityGenerator* city = new CityGenerator(0.5f);
    city->Generate(60, 3); // 60 nodes spread over larger area, depth 3 for MANY more roads
    
    // Get edge vertices for rendering (main roads)
    std::vector<float> edgeVertices = city->GetEdgeVertices();
    
    // Set up VBO and VAO for city main roads
    unsigned int cityVAO, cityVBO;
    glGenVertexArrays(1, &cityVAO);
    glGenBuffers(1, &cityVBO);
    
    glBindVertexArray(cityVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cityVBO);
    glBufferData(GL_ARRAY_BUFFER, edgeVertices.size() * sizeof(float), edgeVertices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
    
    // Get alleyway vertices for rendering
    std::vector<float> alleyVertices = city->GetAlleywayVertices();
    
    // Set up VBO and VAO for alleyways
    unsigned int alleyVAO, alleyVBO;
    glGenVertexArrays(1, &alleyVAO);
    glGenBuffers(1, &alleyVBO);
    
    glBindVertexArray(alleyVAO);
    glBindBuffer(GL_ARRAY_BUFFER, alleyVBO);
    glBufferData(GL_ARRAY_BUFFER, alleyVertices.size() * sizeof(float), alleyVertices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
    
    // Get outskirts vertices for rendering (dead end roads)
    std::vector<float> outskirtsVertices = city->GetOutskirtsVertices();
    
    // Set up VBO and VAO for outskirts
    unsigned int outskirtsVAO, outskirtsVBO;
    glGenVertexArrays(1, &outskirtsVAO);
    glGenBuffers(1, &outskirtsVBO);
    
    glBindVertexArray(outskirtsVAO);
    glBindBuffer(GL_ARRAY_BUFFER, outskirtsVBO);
    glBufferData(GL_ARRAY_BUFFER, outskirtsVertices.size() * sizeof(float), outskirtsVertices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
    
    // Create city shader with geometry shader
    Shader* cityShader = new Shader("shaders/city_layout.vs", "shaders/city_layout.fs", "shaders/city_layout.gs");
    
    // Set city in scene
    scene->SetCity(city, cityVAO, cityVBO, static_cast<unsigned int>(edgeVertices.size() / 3), cityShader);
    scene->SetAlleyways(alleyVAO, alleyVBO, static_cast<unsigned int>(alleyVertices.size() / 3));
    scene->SetOutskirts(outskirtsVAO, outskirtsVBO, static_cast<unsigned int>(outskirtsVertices.size() / 3));
    
    // Get building vertices for rendering
    std::vector<float> buildingVertices = city->GetBuildingVertices();
    
    // Set up VBO and VAO for buildings
    unsigned int buildingVAO, buildingVBO;
    glGenVertexArrays(1, &buildingVAO);
    glGenBuffers(1, &buildingVBO);
    
    glBindVertexArray(buildingVAO);
    glBindBuffer(GL_ARRAY_BUFFER, buildingVBO);
    glBufferData(GL_ARRAY_BUFFER, buildingVertices.size() * sizeof(float), buildingVertices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
    
    scene->SetBuildings(buildingVAO, buildingVBO, static_cast<unsigned int>(buildingVertices.size() / 3));
    
    // Position camera to see the city (city is in -100 to 100 range)
    // Position camera to see the city
    scene->mainCamera->SetPosition(glm::vec3(0.0f, 80.0f, 200.0f));
    scene->mainCamera->SetYaw(-90.0f);
    scene->mainCamera->SetPitch(-25.0f);
    scene->mainCamera->ProcessMouseMovement(0.0f, 0.0f, false); // Update camera vectors

    renderer.Render(&testShader, scene);
    
    // Clean up
    glDeleteVertexArrays(1, &cityVAO);
    glDeleteBuffers(1, &cityVBO);
    glDeleteVertexArrays(1, &alleyVAO);
    glDeleteBuffers(1, &alleyVBO);
    glDeleteVertexArrays(1, &outskirtsVAO);
    glDeleteBuffers(1, &outskirtsVBO);
    glDeleteVertexArrays(1, &buildingVAO);
    glDeleteBuffers(1, &buildingVBO);
    delete city;
    delete cityShader;
    return 0;
}


