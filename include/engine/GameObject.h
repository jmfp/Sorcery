#pragma once
#include <vector>
#include <engine/Component.h>
class GameObject{
    public:
        GameObject();
        ~GameObject();
        template<typename T>
        T* GetComponent(){
            for (auto* component : components){
                if (T* casted = dynamic_cast<T*>(component)){
                    return casted;
                }
            }
            return nullptr;
        };
        template<typename T>
        void AddComponent(Component* componentToAdd){components.push_back(componentToAdd);}
    private:
        std::vector<Component*> components;
};