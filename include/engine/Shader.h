#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Shader{
    public:
        Shader(float vertices[], GLuint indices[], int vertexCount, std::string vertexShader, std::string fragmentShader, GLenum usage = GL_STATIC_DRAW);
        ~Shader();
        void Use();
        void Draw();
    private:
        GLuint VBO;
        GLuint VAO;
        GLuint EBO;
        GLuint shaderProgram;
        int vertexCount;
};