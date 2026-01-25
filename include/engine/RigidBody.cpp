#include <engine/RigidBody.h>
#include <engine/PhysicsSystem.h>
#include <engine/GameObject.h>
#include <engine/PhysicsLayers.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyInterface.h>
#include <Jolt/Physics/Body/BodyLock.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <algorithm>

RigidBody::RigidBody()
    : transform(nullptr),
    bodyID(),
    isBodyCreated(false),
    bodyType(RigidBodyType::DYNAMIC),
    mass(1.0f),
    linearDamping(0.5f),
    angularDamping(0.05f),
    shapeType(ShapeType::BOX),
    shapeDimensions(1.0f, 1.0f, 1.0f){}

void RigidBody::Start() {
    // Get transform from parent GameObject
    if (!parent) {
        return;
    }
    transform = parent->GetTransform();
    if (!transform) {
        return;
    }
    
    // Get physics system
    ::PhysicsSystem& physics = ::PhysicsSystem::GetInstance();
    JPH::BodyInterface& bodyInterface = physics.GetBodyInterface();
    
    // Create Jolt shape
    JPH::RefConst<JPH::Shape> joltShape = CreateJoltShape();
    if (!joltShape) {
        return;
    }
    
    // Convert transform to Jolt format
    JPH::Vec3 position = GlmToJoltVec3(transform->position);
    JPH::Quat rotation = GlmToJoltQuat(transform->rotation);
    
    // Create body settings
    JPH::BodyCreationSettings bodySettings(
        joltShape,
        position,
        rotation,
        GetJoltMotionType(),
        Layers::MOVING  // Use appropriate layer
    );
    
    // Configure mass for dynamic bodies
    if (bodyType == RigidBodyType::DYNAMIC) {
        bodySettings.mMassPropertiesOverride.mMass = mass;
        bodySettings.mLinearDamping = linearDamping;
        bodySettings.mAngularDamping = angularDamping;
    }
    
    // Create body in Jolt
    JPH::Body* body = bodyInterface.CreateBody(bodySettings);
    if (!body) {
        return;
    }
    bodyID = body->GetID();
    
    // Add body to simulation
    bodyInterface.AddBody(bodyID, JPH::EActivation::Activate);
    
    isBodyCreated = true;
    
    // Initial sync: push our transform to Jolt
    SyncTransformToPhysics();
}

// ============================================================================
// TYPE CONVERSION HELPERS
// ============================================================================

glm::vec3 RigidBody::JoltToGlmVec3(const JPH::Vec3& vec) const {
    return glm::vec3(vec.GetX(), vec.GetY(), vec.GetZ());
}

JPH::Vec3 RigidBody::GlmToJoltVec3(const glm::vec3& vec) const {
    return JPH::Vec3(vec.x, vec.y, vec.z);
}

JPH::Quat RigidBody::GlmToJoltQuat(const glm::vec3& eulerDegrees) const {
    glm::quat glmQuat = glm::quat(glm::radians(eulerDegrees));
    return JPH::Quat(glmQuat.x, glmQuat.y, glmQuat.z, glmQuat.w);
}

glm::vec3 RigidBody::JoltQuatToGlmEuler(const JPH::Quat& q) const {
    glm::quat glmQuat(q.GetW(), q.GetX(), q.GetY(), q.GetZ());
    glm::vec3 euler = glm::eulerAngles(glmQuat);
    return glm::degrees(euler);
}

// ============================================================================
// HELPER METHODS
// ============================================================================

JPH::EMotionType RigidBody::GetJoltMotionType() const {
    switch (bodyType) {
        case RigidBodyType::STATIC:
            return JPH::EMotionType::Static;
        case RigidBodyType::DYNAMIC:
            return JPH::EMotionType::Dynamic;
        case RigidBodyType::KINEMATIC:
            return JPH::EMotionType::Kinematic;
        default:
            return JPH::EMotionType::Dynamic;
    }
}

JPH::RefConst<JPH::Shape> RigidBody::CreateJoltShape() const {
    switch (shapeType) {
        case ShapeType::BOX: {
            JPH::Vec3 halfExtents = GlmToJoltVec3(shapeDimensions * 0.5f);
            JPH::BoxShapeSettings boxSettings(halfExtents);
            JPH::ShapeSettings::ShapeResult result = boxSettings.Create();
            if (result.IsValid()) {
                return result.Get();
            }
            return nullptr;
        }
        
        case ShapeType::SPHERE: {
            float radius = std::max({shapeDimensions.x, shapeDimensions.y, shapeDimensions.z}) * 0.5f;
            JPH::SphereShapeSettings sphereSettings(radius);
            JPH::ShapeSettings::ShapeResult result = sphereSettings.Create();
            if (result.IsValid()) {
                return result.Get();
            }
            return nullptr;
        }
        
        default:
            return nullptr;
    }
}

// ============================================================================
// CONFIGURATION METHODS
// ============================================================================

void RigidBody::SetBodyType(RigidBodyType type) {
    bodyType = type;
    if (isBodyCreated) {
        JPH::BodyInterface& bodyInterface = ::PhysicsSystem::GetInstance().GetBodyInterface();
        bodyInterface.SetMotionType(bodyID, GetJoltMotionType(), JPH::EActivation::Activate);
    }
}

void RigidBody::SetMass(float newMass) {
    mass = newMass;
    if (isBodyCreated && bodyType == RigidBodyType::DYNAMIC) {
        JPH::BodyInterface& bodyInterface = ::PhysicsSystem::GetInstance().GetBodyInterface();
        JPH::BodyLockWrite lock(::PhysicsSystem::GetInstance().GetJoltPhysicsSystem()->GetBodyLockInterface(), bodyID);
        if (lock.Succeeded()) {
            JPH::Body& body = lock.GetBody();
            if (body.GetMotionProperties() != nullptr) {
                body.GetMotionProperties()->SetMassProperties(JPH::EAllowedDOFs::All, JPH::MassProperties());
                body.GetMotionProperties()->SetInverseMass(1.0f / mass);
            }
        }
    }
}

void RigidBody::SetLinearDamping(float damping) {
    linearDamping = damping;
    if (isBodyCreated && bodyType == RigidBodyType::DYNAMIC) {
        JPH::BodyInterface& bodyInterface = ::PhysicsSystem::GetInstance().GetBodyInterface();
        JPH::BodyLockWrite lock(::PhysicsSystem::GetInstance().GetJoltPhysicsSystem()->GetBodyLockInterface(), bodyID);
        if (lock.Succeeded()) {
            JPH::Body& body = lock.GetBody();
            if (body.GetMotionProperties() != nullptr) {
                body.GetMotionProperties()->SetLinearDamping(linearDamping);
            }
        }
    }
}

void RigidBody::SetAngularDamping(float damping) {
    angularDamping = damping;
    if (isBodyCreated && bodyType == RigidBodyType::DYNAMIC) {
        JPH::BodyInterface& bodyInterface = ::PhysicsSystem::GetInstance().GetBodyInterface();
        JPH::BodyLockWrite lock(::PhysicsSystem::GetInstance().GetJoltPhysicsSystem()->GetBodyLockInterface(), bodyID);
        if (lock.Succeeded()) {
            JPH::Body& body = lock.GetBody();
            if (body.GetMotionProperties() != nullptr) {
                body.GetMotionProperties()->SetAngularDamping(angularDamping);
            }
        }
    }
}

void RigidBody::SetShape(ShapeType shape, const glm::vec3& dimensions) {
    shapeType = shape;
    shapeDimensions = dimensions;
}

// ============================================================================
// TRANSFORM SYNCHRONIZATION
// ============================================================================

void RigidBody::SyncTransformToPhysics() {
    if (!isBodyCreated || !transform) return;
    
    JPH::BodyInterface& bodyInterface = ::PhysicsSystem::GetInstance().GetBodyInterface();
    
    JPH::Vec3 position = GlmToJoltVec3(transform->position);
    JPH::Quat rotation = GlmToJoltQuat(transform->rotation);
    
    bodyInterface.SetPosition(bodyID, position, JPH::EActivation::DontActivate);
    bodyInterface.SetRotation(bodyID, rotation, JPH::EActivation::DontActivate);
}

void RigidBody::SyncTransformFromPhysics() {
    if (!isBodyCreated || !transform) return;
    
    JPH::BodyInterface& bodyInterface = ::PhysicsSystem::GetInstance().GetBodyInterface();
    
    JPH::Vec3 position = bodyInterface.GetPosition(bodyID);
    JPH::Quat rotation = bodyInterface.GetRotation(bodyID);
    
    transform->position = JoltToGlmVec3(position);
    transform->rotation = JoltQuatToGlmEuler(rotation);
    transform->m_isDirty = true;
}

// ============================================================================
// VELOCITY AND FORCE API
// ============================================================================

void RigidBody::SetLinearVelocity(const glm::vec3& velocity) {
    if (!isBodyCreated) return;
    
    JPH::BodyInterface& bodyInterface = ::PhysicsSystem::GetInstance().GetBodyInterface();
    bodyInterface.SetLinearVelocity(bodyID, GlmToJoltVec3(velocity));
}

glm::vec3 RigidBody::GetLinearVelocity() const {
    if (!isBodyCreated) return glm::vec3(0.0f);
    
    JPH::BodyInterface& bodyInterface = ::PhysicsSystem::GetInstance().GetBodyInterface();
    return JoltToGlmVec3(bodyInterface.GetLinearVelocity(bodyID));
}

void RigidBody::SetAngularVelocity(const glm::vec3& velocity) {
    if (!isBodyCreated) return;
    
    JPH::BodyInterface& bodyInterface = ::PhysicsSystem::GetInstance().GetBodyInterface();
    bodyInterface.SetAngularVelocity(bodyID, GlmToJoltVec3(velocity));
}

glm::vec3 RigidBody::GetAngularVelocity() const {
    if (!isBodyCreated) return glm::vec3(0.0f);
    
    JPH::BodyInterface& bodyInterface = ::PhysicsSystem::GetInstance().GetBodyInterface();
    return JoltToGlmVec3(bodyInterface.GetAngularVelocity(bodyID));
}

void RigidBody::AddForce(const glm::vec3& force) {
    if (!isBodyCreated || bodyType != RigidBodyType::DYNAMIC) return;
    
    JPH::BodyInterface& bodyInterface = ::PhysicsSystem::GetInstance().GetBodyInterface();
    bodyInterface.AddForce(bodyID, GlmToJoltVec3(force));
}

void RigidBody::AddTorque(const glm::vec3& torque) {
    if (!isBodyCreated || bodyType != RigidBodyType::DYNAMIC) return;
    
    JPH::BodyInterface& bodyInterface = ::PhysicsSystem::GetInstance().GetBodyInterface();
    bodyInterface.AddTorque(bodyID, GlmToJoltVec3(torque));
}

void RigidBody::AddImpulse(const glm::vec3& impulse) {
    if (!isBodyCreated || bodyType != RigidBodyType::DYNAMIC) return;
    
    JPH::BodyInterface& bodyInterface = ::PhysicsSystem::GetInstance().GetBodyInterface();
    bodyInterface.AddImpulse(bodyID, GlmToJoltVec3(impulse));
}

// ============================================================================
// UPDATE METHOD
// ============================================================================

void RigidBody::Update(float deltaTime) {
    if (!isBodyCreated || !transform) return;
    
    if (bodyType == RigidBodyType::KINEMATIC) {
    }
    
    if (bodyType == RigidBodyType::DYNAMIC) {
        SyncTransformFromPhysics();
    }
}

// ============================================================================
// DESTRUCTOR
// ============================================================================

RigidBody::~RigidBody() {
    if (isBodyCreated && !bodyID.IsInvalid()) {
        JPH::BodyInterface& bodyInterface = ::PhysicsSystem::GetInstance().GetBodyInterface();
        bodyInterface.RemoveBody(bodyID);
        bodyInterface.DestroyBody(bodyID);
    }
}