#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader{
    public:
        Shader(std::string vertexShader, std::string fragmentShader, const char* geometryShader = nullptr);
        ~Shader();
        void Use();
        void Draw();
        void SetBool(const std::string &name, bool value);
        void SetInt(const std::string &name, int value);
        void SetFloat(const std::string &name, float value);
        void SetVec3(const std::string &name, float x, float y, float z);
        void SetVec3(const std::string &name, const glm::vec3 &value);
        void SetMat4(const std::string &name, const glm::mat4 &mat);
        GLuint ID;
};