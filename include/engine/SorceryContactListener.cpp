#include <engine/SorceryContactListener.h>

SorceryContactListener::SorceryContactListener() {
}

JPH::ValidateResult SorceryContactListener::OnContactValidate(
    const JPH::Body& inBody1,
    const JPH::Body& inBody2,
    JPH::RVec3Arg inBaseOffset,
    const JPH::CollideShapeResult& inCollisionResult
) {
    return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
}

void SorceryContactListener::OnContactAdded(
    const JPH::Body& inBody1,
    const JPH::Body& inBody2,
    const JPH::ContactManifold& inManifold,
    JPH::ContactSettings& ioSettings
) {
}

void SorceryContactListener::OnContactPersisted(
    const JPH::Body& inBody1,
    const JPH::Body& inBody2,
    const JPH::ContactManifold& inManifold,
    JPH::ContactSettings& ioSettings
) {
}

void SorceryContactListener::OnContactRemoved(
    const JPH::SubShapeIDPair& inSubShapePair
) {
}
