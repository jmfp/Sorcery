#include <engine/AutoRotate.h>

AutoRotate::AutoRotate(Transform* transform, float speed, Axis axis) : transform(transform), speed(speed), axis(axis) {
}

AutoRotate::~AutoRotate() {
}

void AutoRotate::Update(float deltaTime) {
    transform->SetRotation(glm::vec3(transform->rotation.x + deltaTime * speed, transform->rotation.y + deltaTime * speed, transform->rotation.z + deltaTime * speed));
}