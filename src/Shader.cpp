#include <engine/Shader.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

std::string ReadShaderFile(const std::string& filePath){
    std::ifstream file(filePath);

    if (!file.is_open()){
        std::cerr << "Failed to open shader file: " << filePath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::cout << "Succeeded in opening and reading shader " << filePath << std::endl;
    return buffer.str();
}

Shader::Shader(float vertices[], GLuint indices[], int vertexCount, std::string vertexShader, std::string fragmentShader, GLenum usage){
    this->vertexCount = vertexCount;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    // Vertex data
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(vertices), vertices, usage);

    // Element buffer object
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexCount * sizeof(indices), indices, GL_STATIC_DRAW);

    // Vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  

    // Shader
    std::string vertexShaderSource = ReadShaderFile(vertexShader);
    const char * sourceCStr = vertexShaderSource.c_str();

    GLuint vertexShaderProgram = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShaderProgram, 1, &sourceCStr, NULL);
    glCompileShader(vertexShaderProgram);


    std::string fragmentShaderSource = ReadShaderFile(fragmentShader);
    const char * sourceCStrFrag = fragmentShaderSource.c_str();

    GLuint fragmentShaderProgram = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderProgram, 1, &sourceCStrFrag, NULL);
    glCompileShader(fragmentShaderProgram);
    // Shader program
    shaderProgram = glCreateProgram();



    glAttachShader(shaderProgram, vertexShaderProgram);
    glAttachShader(shaderProgram, fragmentShaderProgram);
    glLinkProgram(shaderProgram);

    // glUseProgram(shaderProgram);
    // glBindVertexArray(VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
};

void Shader::Use(){
    glUseProgram(shaderProgram);
}

void Shader::Draw(){
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

Shader::~Shader(){
    // glDeleteShader(vertexShaderProgram);
    // glDeleteShader(fragmentShaderProgram);
}