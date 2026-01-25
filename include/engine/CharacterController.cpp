#include <engine/CharacterController.h>
#include <engine/PhysicsSystem.h>
#include <engine/GameObject.h>
#include <engine/Transform.h>
#include <GLFW/glfw3.h>
#include <Jolt/Physics/Character/CharacterVirtual.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/Shape.h>
#include <Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>
#include <Jolt/Physics/Body/BodyFilter.h>
#include <Jolt/Physics/Collision/ShapeFilter.h>
#include <Jolt/Geometry/Plane.h>
#include <engine/PhysicsLayers.h>
#include <glm/gtc/quaternion.hpp>

CharacterController::CharacterController() {
}

CharacterController::~CharacterController() {
    if (character) {
        delete character;
        character = nullptr;
    }
}

void CharacterController::Start() {
    if (!parent) {
        return;
    }
    transform = parent->GetTransform();
    if (!transform) {
        return;
    }

    ::PhysicsSystem& physics = ::PhysicsSystem::GetInstance();
    JPH::PhysicsSystem* physicsSystem = physics.GetJoltPhysicsSystem();

    // Create capsule shape for character
    JPH::Ref<JPH::Shape> capsule = new JPH::CapsuleShape(halfHeight, radius);
    
    // Offset capsule so bottom is at origin
    JPH::RotatedTranslatedShapeSettings shapeSettings(
        JPH::Vec3(0, halfHeight + radius, 0),
        JPH::Quat::sIdentity(),
        capsule
    );
    JPH::Ref<JPH::Shape> shape = shapeSettings.Create().Get();

    // Create character settings
    JPH::CharacterVirtualSettings settings;
    settings.mMaxSlopeAngle = glm::radians(maxSlopeAngle);
    settings.mMaxStrength = maxStrength;
    settings.mShape = shape;
    settings.mSupportingVolume = JPH::Plane(JPH::Vec3::sAxisY(), -halfHeight);

    // Get initial position
    JPH::Vec3 position(transform->position.x, transform->position.y, transform->position.z);

    // Create character (constructor: settings, position, rotation, userData, physicsSystem)
    character = new JPH::CharacterVirtual(&settings, position, JPH::Quat::sIdentity(), 0, physicsSystem);
    character->SetListener(nullptr);
}

void CharacterController::Update(float deltaTime) {
    if (!character || !transform) {
        return;
    }

    ::PhysicsSystem& physics = ::PhysicsSystem::GetInstance();
    JPH::PhysicsSystem* physicsSystem = physics.GetJoltPhysicsSystem();
    JPH::TempAllocator* tempAllocator = physics.GetTempAllocator();

    // Apply gravity to velocity (CharacterVirtual doesn't do this automatically)
    // According to Jolt docs: "Note it's your own responsibility to apply gravity to the character velocity!"
    JPH::Vec3 currentVelocity = character->GetLinearVelocity();
    JPH::Vec3 gravity = physicsSystem->GetGravity();
    
    // Always apply gravity to Y component
    currentVelocity += gravity * deltaTime;
    
    // If on ground, clamp Y velocity to prevent sinking
    if (character->IsSupported() && currentVelocity.GetY() < 0.0f) {
        currentVelocity.SetY(0.0f);
    }
    
    character->SetLinearVelocity(currentVelocity);

    // Update character
    // CharacterVirtual::Update signature: Update(deltaTime, gravity, broadPhaseFilter, objectLayerFilter, bodyFilter, shapeFilter, tempAllocator)
    // Use default filters - construct with existing filter implementations
    static ObjectVsBroadPhaseLayerFilterImpl objectVsBroadPhaseFilter;
    static ObjectLayerPairFilterImpl objectLayerPairFilter;
    static JPH::DefaultBroadPhaseLayerFilter broadPhaseFilter(objectVsBroadPhaseFilter, Layers::MOVING);
    static JPH::DefaultObjectLayerFilter objectLayerFilter(objectLayerPairFilter, Layers::MOVING);
    character->Update(
        deltaTime,
        physicsSystem->GetGravity(),
        broadPhaseFilter,
        objectLayerFilter,
        JPH::BodyFilter(),
        JPH::ShapeFilter(),
        *tempAllocator
    );

    // Sync transform from character
    SyncTransformFromCharacter();
}

void CharacterController::SetLinearVelocity(const glm::vec3& velocity) {
    if (character) {
        character->SetLinearVelocity(JPH::Vec3(velocity.x, velocity.y, velocity.z));
    }
}

glm::vec3 CharacterController::GetLinearVelocity() const {
    if (character) {
        return JoltToGlmVec3(character->GetLinearVelocity());
    }
    return glm::vec3(0.0f);
}

void CharacterController::SetMaxSlopeAngle(float angleDegrees) {
    maxSlopeAngle = angleDegrees;
    if (character) {
        character->SetMaxSlopeAngle(glm::radians(angleDegrees));
    }
}

void CharacterController::SetMaxStrength(float strength) {
    maxStrength = strength;
    if (character) {
        character->SetMaxStrength(strength);
    }
}

void CharacterController::SetShapeSettings(float halfHeight, float radius) {
    this->halfHeight = halfHeight;
    this->radius = radius;
    // Note: Shape cannot be changed after creation, would need to recreate character
}

bool CharacterController::IsSupported() const {
    if (character) {
        return character->IsSupported();
    }
    return false;
}

glm::vec3 CharacterController::GetGroundNormal() const {
    if (character && character->IsSupported()) {
        return JoltToGlmVec3(character->GetGroundNormal());
    }
    return glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 CharacterController::GetPosition() const {
    if (transform) {
        return transform->position;
    }
    return glm::vec3(0.0f);
}

void CharacterController::SyncTransformFromCharacter() {
    if (!character || !transform) {
        return;
    }

    JPH::Vec3 position = character->GetPosition();
    transform->position = JoltToGlmVec3(position);
    transform->m_isDirty = true;
}

void CharacterController::SyncCharacterFromTransform() {
    if (!character || !transform) {
        return;
    }

    JPH::Vec3 position(transform->position.x, transform->position.y, transform->position.z);
    character->SetPosition(position);
}

glm::vec3 CharacterController::JoltToGlmVec3(const JPH::Vec3& vec) const {
    return glm::vec3(vec.GetX(), vec.GetY(), vec.GetZ());
}

void CharacterController::ProcessInput(GLFWwindow* window, float deltaTime, float moveSpeed) {
    if (!character) {
        return;
    }

    glm::vec3 moveDirection(0.0f);
    
    // Get current velocity
    JPH::Vec3 currentVelocity = character->GetLinearVelocity();
    glm::vec3 velocity = JoltToGlmVec3(currentVelocity);
    
    // WASD movement (horizontal only)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        moveDirection.z -= moveSpeed; // Forward (negative Z in OpenGL)
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        moveDirection.z += moveSpeed; // Backward
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        moveDirection.x -= moveSpeed; // Left
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        moveDirection.x += moveSpeed; // Right
    }
    
    // Apply horizontal movement (preserve Y velocity for gravity/jumping)
    velocity.x = moveDirection.x;
    velocity.z = moveDirection.z;
    // Keep Y velocity unchanged (gravity is applied in Update)
    
    character->SetLinearVelocity(JPH::Vec3(velocity.x, velocity.y, velocity.z));
}
