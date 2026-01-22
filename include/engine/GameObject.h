#pragma once
#include <vector>
#include <engine/Component.h>
#include <engine/Transform.h>

class Shader;

class GameObject{
    public:
        GameObject(){};
        ~GameObject(){};
        template<typename T>
        T* GetComponent(){
            for (auto* component : components){
                if (T* casted = dynamic_cast<T*>(component)){
                    return casted;
                }
            }
            return nullptr;
        };
        void AddComponent(Component* componentToAdd){
            components.push_back(componentToAdd);
            componentToAdd->parent = this;
        }
        void AddChild(GameObject* child){children.push_back(child);}
        std::vector<GameObject*> GetChildren(){return children;}
        void Draw(Shader* shader){
            for (auto* component : components) {
                if (Drawable* drawable = dynamic_cast<Drawable*>(component)) {
                    drawable->Draw(shader);
                }
            }
        };
        unsigned int id;
        Transform* GetTransform(){
            return transform;
        }
    protected:
        std::vector<Component*> components, drawableComponents;
        std::vector<GameObject*> children;
        Transform* transform = new Transform();
};