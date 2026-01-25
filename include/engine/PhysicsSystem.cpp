#include <engine/PhysicsSystem.h>
#include <engine/PhysicsLayers.h>
#include <thread>
#include <engine/SorceryContactListener.h>

namespace {
    using EnginePhysicsSystem = ::PhysicsSystem;
}

EnginePhysicsSystem& PhysicsSystem::GetInstance() {
    static EnginePhysicsSystem instance;
    return instance;
}

void EnginePhysicsSystem::Initialize() {
    JPH::RegisterDefaultAllocator();
    JPH::Factory::sInstance = new JPH::Factory();
    JPH::RegisterTypes();

    tempAllocator = new JPH::TempAllocatorImpl(10 * 1024 * 1024);
    jobSystem = new JPH::JobSystemThreadPool(
        JPH::cMaxPhysicsJobs,
        JPH::cMaxPhysicsBarriers,
        std::thread::hardware_concurrency() - 1
    );

    static BroadPhaseLayerInterfaceImpl broadPhaseLayerInterface;
    static ObjectVsBroadPhaseLayerFilterImpl objectVsBroadPhaseLayerFilter;
    static ObjectLayerPairFilterImpl objectLayerPairFilter;

    physicsSystem = new JPH::PhysicsSystem();
    physicsSystem->Init(
        MAX_BODIES,
        num_body_mutexes,
        MAX_BODY_PAIRS,
        MAX_CONTACT_CONTRAINTS,
        broadPhaseLayerInterface,
        objectVsBroadPhaseLayerFilter,
        objectLayerPairFilter
    );
    bodyInterface = &physicsSystem->GetBodyInterface();

    contactListener = new SorceryContactListener();
    physicsSystem->SetContactListener(contactListener);
}

void EnginePhysicsSystem::Step(float deltaTime) {
    if (physicsSystem) {
        physicsSystem->Update(deltaTime, 1, tempAllocator, jobSystem);
    }
}

void EnginePhysicsSystem::Shutdown() {
    delete contactListener;
    contactListener = nullptr;
    delete physicsSystem;
    physicsSystem = nullptr;
    delete jobSystem;
    jobSystem = nullptr;
    delete tempAllocator;
    tempAllocator = nullptr;
    delete JPH::Factory::sInstance;
    JPH::Factory::sInstance = nullptr;
}