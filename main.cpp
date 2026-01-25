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
#include <engine/AutoRotate.h>

int main()
{
    std::vector<Vertex> planeVerts = {
        Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)),
        Vertex(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)),
        Vertex(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)),
        Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)),
        Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)),
    };
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
    window->Initialize("Sorcery Engine", 1920, 1080);
    Shader testShader = Shader("shaders/point_light.vs", "shaders/lighting.fs");
    Renderer3D renderer = Renderer3D(window);
    Scene* scene = new Scene("Test Scene", window);
    GameObject* testObject = new GameObject();
    testObject->GetTransform()->SetRotation(glm::vec3(10.0f, 10.0f, 10.0f));
    // Texture diffuseTexture = Texture("../src/textures/container2.png", 512, 512, TextureType::DIFFUSE);
    // Texture specularTexture = Texture("../src/textures/specu456lar.png", 512, 512, TextureType::SPECULAR);
    // Texture emissionTexture = Texture("../src/textures/emission.jpg", 512, 512, TextureType::EMISSION);

    std::vector<Texture*> textures = {};
    // std::vector<Texture*> textures = {&diffuseTexture, &specularTexture, &emissionTexture};
    Material* material = new Material(textures, 32.0f);
    
    unsigned int indices[] = {
        // Front face (z = -0.5) - vertices 0-5
        0, 1, 2,   // first triangle: bottom-left -> bottom-right -> top-right
        2, 4, 0,   // second triangle: top-right -> top-left -> bottom-left (skip duplicate 3, use 2; skip duplicate 5, use 0)
        // Back face (z = 0.5) - vertices 6-11
        6, 7, 8,   // first triangle: bottom-left -> bottom-right -> top-right
        8, 10, 6,  // second triangle: top-right -> top-left -> bottom-left (skip duplicate 9, use 8; skip duplicate 11, use 6)
        // Left face (x = -0.5) - vertices 12-17
        12, 13, 14, // first triangle: top-back -> top-front -> bottom-front
        14, 16, 12, // second triangle: bottom-front -> bottom-back -> top-back (skip duplicate 15, use 14; skip duplicate 17, use 12)
        // Right face (x = 0.5) - vertices 18-23
        18, 19, 20, // first triangle: top-back -> top-front -> bottom-front
        20, 22, 18, // second triangle: bottom-front -> bottom-back -> top-back (skip duplicate 21, use 20; skip duplicate 23, use 18)
        // Bottom face (y = -0.5) - vertices 24-29
        24, 25, 26, // first triangle: front-left -> front-right -> back-right
        26, 28, 24, // second triangle: back-right -> back-left -> front-left (skip duplicate 27, use 26; skip duplicate 29, use 24)
        // Top face (y = 0.5) - vertices 30-35
        30, 31, 32, // first triangle: front-left -> front-right -> back-right
        32, 34, 30  // second triangle: back-right -> back-left -> front-left (skip duplicate 33, use 32; skip duplicate 35, use 30)
    };
    std::vector<unsigned int> planeIndices = {
        0, 1, 2,
        2, 4, 0,
    };
    std::vector<unsigned int> indicesVec(indices, indices + sizeof(indices) / sizeof(indices[0]));
    Mesh* planeMesh = new Mesh(planeVerts, planeIndices, textures);
    Mesh* cubeMesh = new Mesh(boxVerts, indicesVec, textures);
    MeshRenderer* meshRenderer = new MeshRenderer(cubeMesh, material);
    MeshRenderer* planeMeshRenderer = new MeshRenderer(planeMesh, material);
    testObject->GetTransform()->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    // testObject->AddComponent(meshRenderer);
    GameObject* testFloor = new GameObject();
    testFloor->AddComponent(planeMeshRenderer);
    testFloor->GetTransform()->SetPosition(glm::vec3(0.0f, -0.5f, 0.0f));
    testFloor->GetTransform()->SetScale(glm::vec3(100.0f, 100.0f, 100.0f));
    testFloor->GetTransform()->SetRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
    scene->AddGameObject(testFloor);
    AutoRotate* autoRotate = new AutoRotate(testObject->GetTransform(), 5.0f, Axis::Y);
    testObject->AddComponent(autoRotate);
    scene->AddGameObject(testObject);

    renderer.Render(&testShader, scene);
    return 0;
}


