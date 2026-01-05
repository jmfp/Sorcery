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
    Shader testShader = Shader("shaders/vertex.vs", "shaders/fragment.fs");
    Renderer renderer = Renderer(window);

    renderer.Render(&testShader);
    return 0;
}


