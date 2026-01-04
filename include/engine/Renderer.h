#pragma once
#include <engine/Shader.h>
#include <engine/Window.h>

class Renderer {
    public:
        Renderer(Window* window);
        ~Renderer();
        void Render(Shader* shader);
        void SetWindow(Window* newWindow) {window = newWindow;}

    private:
        Window* window;
};