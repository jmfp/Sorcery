#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <engine/Window.h>
#include <engine/Shader.h>
#include <engine/Renderer.h>

int main()
{
    // Initialize GLFW
    Window* window = new Window();
    window->Initialize("Game Name", 800, 800);
    float vertices[] = {
        0.5f,  0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f   
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };
    
    Shader testShader = Shader(vertices, indices, 12, "shaders/vertex.vs", "shaders/fragment.fs");
    Renderer renderer = Renderer(window);

    renderer.Render(&testShader);
    return 0;
}


