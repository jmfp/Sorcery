#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Shader{
    public:
        Shader(std::string vertexShader, std::string fragmentShader);
        ~Shader();
        void Use();
        void Draw();
        void SetBool(const std::string &name, bool value);
        void SetInt(const std::string &name, int value);
        void SetFloat(const std::string &name, float value);
        GLuint ID;
};