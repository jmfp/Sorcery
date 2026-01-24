#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <engine/Window.h>
#include <engine/Shader.h>
#include <engine/Renderer.h>
#include <engine/Scene.h>
#include <engine/Model.h>
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
    
    // Create framebuffer AFTER OpenGL context is initialized
    unsigned int framebuffer, textureColorBuffer, rbo;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1920, 1080);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    
    // Unbind framebuffer initially
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // Your engine setup (Scene, Renderer, etc.)
    Renderer3D renderer = Renderer3D(window);
    Scene* scene = new Scene("Editor Scene", window);

    char modelPath[] = "../src/models/backpack/backpack.obj";
    Model* model = new Model(modelPath);
    model->SetCamera(scene->mainCamera);
    Shader modelShader = Shader("shaders/mesh.vs", "shaders/mesh.fs");
    model->SetShader(modelShader);
    
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
        
        // Viewport window - must be before ImGui::Render()
        ImGui::Begin("Viewport");
        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        
        // Only render if viewport has valid size
        if (viewportSize.x > 0 && viewportSize.y > 0)
        {
            // Bind framebuffer and render scene
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
            glViewport(0, 0, (int)viewportSize.x, (int)viewportSize.y);
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            // Render scene GameObjects
            scene->Draw();
            
            // Render Model directly (Model doesn't use component system)
            model->Draw(modelShader);

            // Unbind framebuffer
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // Display the texture in ImGui
            ImGui::Image((void*)(intptr_t)textureColorBuffer, viewportSize);
        }
        ImGui::End();
        
        // Render all ImGui windows
        ImGui::Render();
        
        // Clear the default framebuffer and render ImGui
        int display_w, display_h;
        glfwGetFramebufferSize(window->GetWindow(), &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window->GetWindow());
    }
    
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    return 0;
}