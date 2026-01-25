#include <engine/Renderer.h>
#include <engine/Texture.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <engine/Camera.h>
#include <engine/Model.h>
#include <engine/Mesh.h>
#include <engine/Math.h>
#include <engine/Scene.h>
#include <engine/PhysicsSystem.h>
#include <engine/CharacterController.h>
#include <GLFW/glfw3.h>
Renderer::Renderer(Window* window){
    this->window = window;
}

void Renderer3D::Render(Shader* shader, Scene* scene){
    glfwSetWindowUserPointer(window->GetWindow(), scene->mainCamera);
    glEnable(GL_DEPTH_TEST);
    
    while(!glfwWindowShouldClose(window->GetWindow()))
    {
        static float lastFrame = 0.0f;
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        int framebufferWidth, framebufferHeight;
        glfwGetFramebufferSize(window->GetWindow(), &framebufferWidth, &framebufferHeight);
        glViewport(0, 0, framebufferWidth, framebufferHeight);

        // Find character controller and process input for it instead of camera
        CharacterController* characterController = nullptr;
        for (auto* gameObject : scene->GetGameObjects()) {
            characterController = gameObject->GetComponent<CharacterController>();
            if (characterController) {
                break;
            }
        }
        
        if (characterController) {
            // Process character input (WASD controls character)
            characterController->ProcessInput(window->GetWindow(), deltaTime);
            
            // Attach camera to character's head position (first-person view)
            glm::vec3 characterPosition = characterController->GetPosition();
            // Offset upward for head position (character has halfHeight=1.0, so head is roughly at +1.0)
            float headHeight = 1.0f; // Adjust this to match your character's head height
            scene->mainCamera->SetPosition(characterPosition + glm::vec3(0.0f, headHeight, 0.0f));
        } else {
            // Fall back to camera controls if no character controller
            window->ProcessInput(*scene->mainCamera, deltaTime);
        }

        PhysicsSystem::GetInstance().Step(deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene->Draw();

        glfwSwapBuffers(window->GetWindow());
        glfwPollEvents();
    }
    glfwTerminate();
}

void Renderer::Render(Shader* shader, Scene* scene){
    //Camera
    // TODO: have renderer use a camera as a parameter so that we can pass either 2d or 3d
    Camera camera;
    glfwSetWindowUserPointer(window->GetWindow(), &camera);
    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(window->GetWindow(), &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);
    glEnable(GL_DEPTH_TEST);
    glm::mat4 projection = glm::ortho(
        -400.0f, 400.0f, // left and right
        -400.0f, 400.0f, // bottom and top
        0.1f, //near plane
        100.0f // far plane
    );
    while(!glfwWindowShouldClose(window->GetWindow()))
    {
        // Calculate delta time
        static float lastFrame = 0.0f;
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        int framebufferWidth, framebufferHeight;
        glfwGetFramebufferSize(window->GetWindow(), &framebufferWidth, &framebufferHeight);
        glViewport(0, 0, framebufferWidth, framebufferHeight);

        glm::mat4 view = camera.GetViewMatrix();

        window->ProcessInput(camera, deltaTime);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap buffers and poll events
        glfwSwapBuffers(window->GetWindow());
        glfwPollEvents();
    }
    // Cleanup
    glfwTerminate();
}

Renderer::~Renderer(){

}