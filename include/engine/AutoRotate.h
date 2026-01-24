#pragma once
#include <engine/Component.h>
#include <engine/Transform.h>

// This is an example of a component. Since this isn't a drawable component, so it doesn't use the Drawable interface.

enum class Axis {
    X,
    Y,
    Z,
    XY,
    YZ, 
    XZ,
    ALL
};

class AutoRotate : public Component {
    public:
        AutoRotate(Transform* transform, float speed = 5.0f, Axis axis = Axis::Y);
        ~AutoRotate();
        void Update(float deltaTime);
    private:
        Transform* transform;
        float speed;
        Axis axis;
};