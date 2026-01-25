#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

class SorceryContactListener;

class PhysicsSystem {
    public:
        static PhysicsSystem& GetInstance();

        void Initialize();
        void Shutdown();
        void Step(float deltaTime);

        JPH::BodyInterface& GetBodyInterface() {return *bodyInterface;}
        JPH::PhysicsSystem* GetJoltPhysicsSystem() {return physicsSystem;}
        const JPH::NarrowPhaseQuery& GetNarrowPhaseQuery() {return physicsSystem->GetNarrowPhaseQuery();}
        JPH::TempAllocator* GetTempAllocator() {return tempAllocator;}
        SorceryContactListener* GetContactListener() {return contactListener;}

    private:
        PhysicsSystem() = default;
        ~PhysicsSystem() = default;
        PhysicsSystem(const PhysicsSystem&) = delete;
        PhysicsSystem& operator=(const PhysicsSystem&) = delete;

        JPH::PhysicsSystem* physicsSystem = nullptr;
        JPH::BodyInterface* bodyInterface = nullptr;
        JPH::TempAllocator* tempAllocator = nullptr;
        JPH::JobSystemThreadPool* jobSystem = nullptr;
        SorceryContactListener* contactListener = nullptr;

        static constexpr uint32_t MAX_BODIES = 1024;
        static constexpr uint32_t num_body_mutexes = 0;
        static constexpr uint32_t MAX_BODY_PAIRS = 1024;
        static constexpr uint32_t MAX_CONTACT_CONTRAINTS = 1024;
};