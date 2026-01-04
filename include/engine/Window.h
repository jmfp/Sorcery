#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


class Window{
    public:
        void Initialize(const char* name = "Sorcery Window", int width = 800, int height = 800, int majorVersion = 3, int minorVersion = 3);
        virtual GLFWwindow* GetWindow(){return window;}
    private:
        GLFWwindow* window;
};