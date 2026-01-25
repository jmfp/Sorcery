#pragma once

#include <engine/Component.h>
#include <engine/Transform.h>
#include <glm/glm.hpp>
#include <Jolt/Jolt.h>
#include <Jolt/Core/Reference.h>
#include <Jolt/Physics/Body/BodyID.h>
#include <Jolt/Physics/Body/Body.h>
#include <Jolt/Math/Vec3.h>
#include <Jolt/Math/Quat.h>
#include <Jolt/Physics/Collision/Shape/Shape.h>

enum class RigidBodyType {
    STATIC, // Doesn't move
    DYNAMIC, // Moves based on physics
    KINEMATIC // Moves based on velocity
};

enum class ShapeType {
    BOX,
    SPHERE,
    // TODO: Add Capsule
};

class RigidBody : public Component {
    public:
        RigidBody();
        ~RigidBody();
        void Start() override;
        void Update(float deltaTime) override;

        // configuration
        void SetBodyType(RigidBodyType type);
        void SetMass(float mass);
        void SetLinearDamping(float damping);
        void SetAngularDamping(float damping);
        void SetShape(ShapeType shape, const glm::vec3& dimensions);

        void SetLinearVelocity(const glm::vec3& velocity);
        glm::vec3 GetLinearVelocity() const;
        void SetAngularVelocity(const glm::vec3& velocity);
        glm::vec3 GetAngularVelocity() const;
        void AddForce(const glm::vec3& force);
        void AddTorque(const glm::vec3& torque);
        void AddImpulse(const glm::vec3& impulse);

        void SyncTransformToPhysics();
        void SyncTransformFromPhysics();

    private:
        Transform* transform;
        JPH::BodyID bodyID;
        bool isBodyCreated;

        RigidBodyType bodyType;
        float mass;
        float linearDamping;
        float angularDamping;
        ShapeType shapeType;
        glm::vec3 shapeDimensions;

        JPH::EMotionType GetJoltMotionType() const;
        JPH::RefConst<JPH::Shape> CreateJoltShape() const;
        glm::vec3 JoltToGlmVec3(const JPH::Vec3& vec) const;
        JPH::Vec3 GlmToJoltVec3(const glm::vec3& vec) const;
        JPH::Quat GlmToJoltQuat(const glm::vec3& eulerDegrees) const;
        glm::vec3 JoltQuatToGlmEuler(const JPH::Quat& q) const;

        float linearVelocity;
        float angularVelocity;
        float linearAcceleration;
        float angularAcceleration;
};