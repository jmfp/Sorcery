#pragma once
#include <engine/Shader.h>
#include <engine/Window.h>

// Forward declaration - we only need a pointer, not the full definition
class Scene;

class Renderer {
    public:
        Renderer(Window* window);
        ~Renderer();
        virtual void Render(Shader* shader, Scene* scene);
        void SetWindow(Window* newWindow) {window = newWindow;}

    protected:
        Window* window;
};

class Renderer3D : public Renderer {
    public:
        Renderer3D(Window* window) : Renderer(window) {}
        virtual void Render(Shader* shader, Scene* scene) override;
        // Additional 3D-specific methods can be added here
};