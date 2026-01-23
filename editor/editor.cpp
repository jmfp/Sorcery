#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <engine/Window.h>
#include <engine/Shader.h>
#include <engine/Renderer.h>
#include <engine/Scene.h>
// ... other engine includes

int main()
{
    Window* window = new Window();
    window->Initialize("Editor", 1920, 1080);
    
    // Re-enable cursor for ImGui (Window::Initialize disables it)
    glfwSetInputMode(window->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    
    // Remove mouse and scroll callbacks that expect a Camera pointer
    // These will cause segfaults when ImGui processes input
    glfwSetCursorPosCallback(window->GetWindow(), nullptr);
    glfwSetScrollCallback(window->GetWindow(), nullptr);
    
    // Enable depth testing for 3D rendering
    glEnable(GL_DEPTH_TEST);
    
    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    
    ImGui_ImplGlfw_InitForOpenGL(window->GetWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
    
    // Your engine setup (Scene, Renderer, etc.)
    Renderer3D renderer = Renderer3D(window);
    Scene* scene = new Scene("Editor Scene", window);
    
    // Main loop
    while (!glfwWindowShouldClose(window->GetWindow()))
    {
        glfwPollEvents();
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        // ImGui windows
        ImGui::DockSpaceOverViewport();
        
        // Your editor UI here
        ImGui::Begin("Scene Hierarchy");
        // ... scene tree
        ImGui::End();
        
        ImGui::Begin("Properties");
        // ... property editor
        ImGui::End();
        
        ImGui::Render();
        
        // Your engine rendering
        int display_w, display_h;
        glfwGetFramebufferSize(window->GetWindow(), &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Render your scene
        scene->Draw();
        
        // Render ImGui on top
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window->GetWindow());
    }
    
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    return 0;
}