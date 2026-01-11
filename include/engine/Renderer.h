#pragma once
#include <engine/Shader.h>
#include <engine/Window.h>

class Renderer {
    public:
        Renderer(Window* window);
        ~Renderer();
        virtual void Render(Shader* shader);
        void SetWindow(Window* newWindow) {window = newWindow;}

    protected:
        Window* window;
};

class Renderer3D : public Renderer {
    public:
        Renderer3D(Window* window) : Renderer(window) {}
        virtual void Render(Shader* shader) override;
        // Additional 3D-specific methods can be added here
};