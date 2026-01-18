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
    window->Initialize("Game Name", 1920, 1080);
    Shader testShader = Shader("shaders/point_light.vs", "shaders/lighting.fs");
    Renderer3D renderer = Renderer3D(window);

    renderer.Render(&testShader);
    return 0;
}


