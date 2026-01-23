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
        // Front face - reuse vertices 2 and 0
        0, 1, 2,   // first triangle
        2, 4, 0,   // second triangle (reuses 2 and 0)
        // Back face - reuse vertices 8 and 6
        6, 7, 8,   // first triangle
        8, 10, 6,  // second triangle (reuses 8 and 6)
        // Left face - reuse vertices 14 and 12
        12, 13, 14, // first triangle
        14, 16, 12, // second triangle (reuses 14 and 12)
        // Right face - reuse vertices 20 and 18
        18, 19, 20, // first triangle
        20, 22, 18, // second triangle (reuses 20 and 18)
        // Bottom face - reuse vertices 26 and 24
        24, 25, 26, // first triangle
        26, 28, 24, // second triangle (reuses 26 and 24)
        // Top face - reuse vertices 32 and 30
        30, 31, 32, // first triangle
        32, 34, 30  // second triangle (reuses 32 and 30)
    };
    std::vector<unsigned int> indicesVec(indices, indices + sizeof(indices) / sizeof(indices[0]));
    Mesh* cubeMesh = new Mesh(boxVerts, indicesVec, textures);
    MeshRenderer* meshRenderer = new MeshRenderer(cubeMesh, material);
    testObject->GetTransform()->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    testObject->AddComponent(meshRenderer);
    scene->AddGameObject(testObject);

    renderer.Render(&testShader, scene);
    return 0;
}


