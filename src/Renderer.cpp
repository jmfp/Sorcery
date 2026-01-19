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
    Model testFloor = Model((char *)"../src/models/floor/pixel_floor.obj");
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    Texture diffuseMapTexture = Texture((char *)"../src/textures/container2.png", 256, 256);
    Texture specularMapTexture = Texture((char *)"../src/textures/specular.png", 256, 256);
    Texture emissionMap = Texture((char *)"../src/textures/emission.jpg", 256, 256);
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
    testFloor.SetShader(backpackShader);
    testFloor.SetCamera(&camera);
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f), // field of view
        800.0f / 800.0f, // aspect ratio
        0.1f, //near plane
        100.0f // far plane
    );
    
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    unsigned int indices[] = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };
    float points[] = {
	    -0.5f,  0.5f, // top-left
	     0.5f,  0.5f, // top-right
	     0.5f, -0.5f, // bottom-right
	    -0.5f, -0.5f  // bottom-left
    };
    Shader* pointShader = new Shader("shaders/city_layout.vs", "shaders/city_layout.fs", "shaders/city_layout.gs");
    unsigned int VBO, containerVAO, pointsVAO, pointsVBO;
    glGenVertexArrays(1, &pointsVAO);
    glGenBuffers(1, &pointsVBO);
    
    // Set up points VAO and VBO
    glBindVertexArray(pointsVAO);
    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Set up container VAO and VBO
    glGenVertexArrays(1, &containerVAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(containerVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Light source VAO (uses same VBO)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };
    // positions all containers
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
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
        // testModel.Draw(backpackShader);
        testFloor.SetMatrices(model, view, projection);
        testFloor.Draw(backpackShader);
        
        /////////////////////////////////////
        //////This is test stuff/////////////
        /////////////////////////////////////

        // be sure to activate shader when setting uniforms/drawing objects
        shader->Use();
        shader->SetVec3("viewPos", camera.GetPosition());
        shader->SetFloat("material.shininess", 32.0f);

        lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
        lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;
        shader->SetVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        shader->SetVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        shader->SetVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        shader->SetVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        shader->SetVec3("pointLights[0].position", pointLightPositions[0]);
        shader->SetVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        shader->SetVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        shader->SetVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        shader->SetFloat("pointLights[0].constant", 1.0f);
        shader->SetFloat("pointLights[0].linear", 0.09f);
        shader->SetFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        shader->SetVec3("pointLights[1].position", pointLightPositions[1]);
        shader->SetVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        shader->SetVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        shader->SetVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        shader->SetFloat("pointLights[1].constant", 1.0f);
        shader->SetFloat("pointLights[1].linear", 0.09f);
        shader->SetFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        shader->SetVec3("pointLights[2].position", pointLightPositions[2]);
        shader->SetVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        shader->SetVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        shader->SetVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        shader->SetFloat("pointLights[2].constant", 1.0f);
        shader->SetFloat("pointLights[2].linear", 0.09f);
        shader->SetFloat("pointLights[2].quadratic", 0.032f);
        // point light 4
        shader->SetVec3("pointLights[3].position", pointLightPositions[3]);
        shader->SetVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        shader->SetVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        shader->SetVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        shader->SetFloat("pointLights[3].constant", 1.0f);
        shader->SetFloat("pointLights[3].linear", 0.09f);
        shader->SetFloat("pointLights[3].quadratic", 0.032f);
        // spotLight
        shader->SetVec3("spotLight.position", camera.GetPosition());
        shader->SetVec3("spotLight.direction", camera.GetFront());
        shader->SetVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        shader->SetVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        shader->SetVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        shader->SetFloat("spotLight.constant", 1.0f);
        shader->SetFloat("spotLight.linear", 0.09f);
        shader->SetFloat("spotLight.quadratic", 0.032f);
        shader->SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        shader->SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f))); 
        shader->SetInt("material.diffuse", 0);
        shader->SetInt("material.specular", 1);
        shader->SetInt("material.emission", 2);

        // view/projection transformations
        shader->SetMat4("view", view);
        shader->SetMat4("projection", projection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMapTexture.GetTexture());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMapTexture.GetTexture());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, emissionMap.GetTexture());
        
        // render containers
        glBindVertexArray(containerVAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader->SetMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        // Render points (with geometry shader creating lines)
        // Disable depth test temporarily so lines are always visible
        glDisable(GL_DEPTH_TEST);
        pointShader->Use();
        pointShader->SetMat4("projection", projection);
        pointShader->SetMat4("view", view);
        pointShader->SetMat4("model", glm::mat4(1.0f));
        glBindVertexArray(pointsVAO);
        glDrawArrays(GL_POINTS, 0, 4);
        glEnable(GL_DEPTH_TEST);
        
        // also draw the lamp object(s)
        light.Use();
        light.SetMat4("projection", projection);
        light.SetMat4("view", view);

        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            light.SetMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


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