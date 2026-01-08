#include <engine/Renderer.h>
#include <engine/Texture.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <engine/Camera.h>

Renderer::Renderer(Window* window){
    this->window = window;
}

void Renderer::Render(Shader* shader){
    //Camera
    // TODO: have renderer use a camera as a parameter so that we can pass either 2d or 3d
    Camera camera;
    glfwSetWindowUserPointer(window->GetWindow(), &camera);
    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(window->GetWindow(), &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);
    ///////////////////////////////////////
    ////// This is test stuff /////////////
    ///////////////////////////////////////
    glEnable(GL_DEPTH_TEST);
    Texture testTexture = Texture("../src/textures/wall.jpg", 16, 16);
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };
    unsigned int indices[] = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2); 
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
        glBindTexture(GL_TEXTURE_2D, testTexture.GetTexture());
        
        /////////////////////////////////////
        //////This is test stuff/////////////
        /////////////////////////////////////
        shader->Use();
        
        // glm::mat4 transform = glm::mat4(1.0f);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // position in world
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f),
        glm::vec3(0.5f, 1.0f, 0.0f)); // rotation around an axis
        // view matrix is already set above from camera.GetViewMatrix()
        
        unsigned int modelLoc = glGetUniformLocation(shader->ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        
        unsigned int viewLoc = glGetUniformLocation(shader->ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        
        unsigned int projLoc = glGetUniformLocation(shader->ID, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        // unsigned int modelLoc = glGetUniformLocation(shader->ID, "model");
        // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        /////////////////////////////////////
        /////////////////////////////////////
        /////////////////////////////////////
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap buffers and poll events
        glfwSwapBuffers(window->GetWindow());
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // Cleanup
    glfwTerminate();
}

Renderer::~Renderer(){

}