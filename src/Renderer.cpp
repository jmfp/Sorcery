#include <engine/Renderer.h>

Renderer::Renderer(Window* window){
    this->window = window;
}

void Renderer::Render(Shader* shader){
    // Set viewport
    glViewport(0, 0, 800, 400);
    // Render loop
    while(!glfwWindowShouldClose(window->GetWindow()))
    {
        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader->Use();
        shader->Draw();

        // Swap buffers and poll events
        glfwSwapBuffers(window->GetWindow());
        glfwPollEvents();
    }

    // Cleanup
    glfwTerminate();
}

Renderer::~Renderer(){

}