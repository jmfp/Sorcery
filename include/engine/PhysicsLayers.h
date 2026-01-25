#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>

namespace Layers {
    static constexpr JPH::ObjectLayer NON_MOVING = 0;
    static constexpr JPH::ObjectLayer MOVING = 1;
    static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
    
    static constexpr JPH::BroadPhaseLayer NON_MOVING_BP(0);
    static constexpr JPH::BroadPhaseLayer MOVING_BP(1);
    static constexpr JPH::BroadPhaseLayer NUM_BP_LAYERS(2);
}

class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter {
    public:
        bool ShouldCollide(JPH::ObjectLayer a, JPH::ObjectLayer b) const override {
            return true; // Everything collides with everything
        }
};

class BroadPhaseLayerInterfaceImpl : public JPH::BroadPhaseLayerInterface {
    public:
        JPH::uint GetNumBroadPhaseLayers() const override {
            return 2;
        }
        JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override {
            return inLayer == Layers::NON_MOVING ? Layers::NON_MOVING_BP : Layers::MOVING_BP;
        }
};

class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter {
    public:
        bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override {
            return true;
        }
};