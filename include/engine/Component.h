#pragma once

class GameObject;
class Shader;

// Drawable interface
class Drawable {
    public:
        virtual void Draw(Shader* shader) = 0;
        virtual ~Drawable() = default;
};

class Component{
    public:
        virtual void Start(){return;}
        virtual void Update(float deltaTime){return;}
        void SetEnabled(bool isEnabled){enabled = isEnabled;}
        GameObject* parent;
    protected:
        bool enabled = true;
};