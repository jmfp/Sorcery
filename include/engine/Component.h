#pragma once

class Component{
    public:
        virtual void Start(){return;}
        virtual void Update(float deltaTime){return;}
        void SetEnabled(bool isEnabled){enabled = isEnabled;}
    private:
        // GameObject* parent;
        bool enabled = true;
};