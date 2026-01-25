#pragma once

#include <engine/Component.h>
#include <Jolt/Jolt.h>
#include <glm/glm.hpp>

// Forward declarations
typedef struct GLFWwindow GLFWwindow;
namespace JPH {
    class CharacterVirtual;
}

class Transform;

class CharacterController : public Component {
    public:
        CharacterController();
        ~CharacterController();

        void Start() override;
        void Update(float deltaTime) override;

        // Movement control
        void SetLinearVelocity(const glm::vec3& velocity);
        glm::vec3 GetLinearVelocity() const;
        
        // Character settings
        void SetMaxSlopeAngle(float angleDegrees);
        void SetMaxStrength(float strength);
        void SetShapeSettings(float halfHeight, float radius);

        // Query character state
        bool IsSupported() const;
        glm::vec3 GetGroundNormal() const;
        glm::vec3 GetPosition() const;
        
        // Input handling
        void ProcessInput(GLFWwindow* window, float deltaTime, float moveSpeed = 5.0f);

    private:
        Transform* transform = nullptr;
        JPH::CharacterVirtual* character = nullptr;
        
        float halfHeight = 1.0f;
        float radius = 0.5f;
        float maxSlopeAngle = 45.0f;
        float maxStrength = 100.0f;

        void SyncTransformFromCharacter();
        void SyncCharacterFromTransform();
        
        glm::vec3 GlmToJoltVec3(const glm::vec3& vec) const;
        glm::vec3 JoltToGlmVec3(const JPH::Vec3& vec) const;
};
