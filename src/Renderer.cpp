#include <engine/Renderer.h>
#include <engine/Texture.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <engine/Camera.h>
#include <engine/Model.h>
#include <engine/Mesh.h>
Renderer::Renderer(Window* window){
    this->window = window;
}

void Renderer3D::Render(Shader* shader){
    stbi_set_flip_vertically_on_load(true);
    Model testModel = Model((char *)"../src/models/backpack/backpack.obj");
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    glfwSetWindowUserPointer(window->GetWindow(), &camera);
    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(window->GetWindow(), &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);
    ///////////////////////////////////////
    ////// This is test stuff /////////////
    ///////////////////////////////////////
    glEnable(GL_DEPTH_TEST);
    Shader light("shaders/light.vs", "shaders/light.fs");
    Shader backpackShader("shaders/mesh.vs", "shaders/mesh.fs");
    testModel.SetShader(backpackShader);
    testModel.SetCamera(&camera);
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f), // field of view
        800.0f / 800.0f, // aspect ratio
        0.1f, //near plane
        100.0f // far plane
    );
    
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    unsigned int indices[] = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };
    unsigned int VBO, containerVAO;
    glGenVertexArrays(1, &containerVAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(containerVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Light source VAO (uses same VBO)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    ///////////////////////////////////////
    ////// This is not test stuff /////////
    ///////////////////////////////////////
    // Render loop
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
        
        glm::mat4 projection = glm::perspective(
            glm::radians(camera.GetZoom()),
            (float)framebufferWidth / (float)framebufferHeight,
            0.1f,
            100.0f
        );

        glm::mat4 view = camera.GetViewMatrix();

        window->ProcessInput(camera, deltaTime);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // shader->Draw();
        // glBindTexture(GL_TEXTURE_2D, testTexture.GetTexture());
        glm::mat4 model = glm::mat4(1.0f);
        // testModel.SetMatrices(model, view, projection);
        testModel.Draw(backpackShader);
        
        /////////////////////////////////////
        //////This is test stuff/////////////
        /////////////////////////////////////

        lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
        lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;
        shader->Use();
        shader->SetVec3("viewPos", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        shader->SetVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        shader->SetVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
        shader->SetVec3("material.specular", 0.5f, 0.5f, 0.5f);
        shader->SetFloat("material.shininess", 32.0f);
        shader->SetVec3("light.position", lightPos.x, lightPos.y, lightPos.z);
        shader->SetVec3("light.ambient",  0.2f, 0.2f, 0.2f);
        shader->SetVec3("light.diffuse",  0.5f, 0.5f, 0.5f); // darken diffuse light a bit
        shader->SetVec3("light.specular", 1.0f, 1.0f, 1.0f);
        unsigned int modelLoc = glGetUniformLocation(shader->ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        
        unsigned int viewLoc = glGetUniformLocation(shader->ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        
        unsigned int projLoc = glGetUniformLocation(shader->ID, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glBindVertexArray(containerVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        // Render light source cube
        light.Use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        
        unsigned int lightModelLoc = glGetUniformLocation(light.ID, "model");
        glUniformMatrix4fv(lightModelLoc, 1, GL_FALSE, glm::value_ptr(model));
        
        unsigned int lightViewLoc = glGetUniformLocation(light.ID, "view");
        glUniformMatrix4fv(lightViewLoc, 1, GL_FALSE, glm::value_ptr(view));
        
        unsigned int lightProjLoc = glGetUniformLocation(light.ID, "projection");
        glUniformMatrix4fv(lightProjLoc, 1, GL_FALSE, glm::value_ptr(projection));
        
        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Swap buffers and poll events
        glfwSwapBuffers(window->GetWindow());
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &containerVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
}

void Renderer::Render(Shader* shader){
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