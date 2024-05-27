#ifndef ECS_ENTITY_MANAGER_CPP
#define ECS_ENTITY_MANAGER_CPP

#include <cassert>
#include <ecs.hpp>


ecs::EntityManager::EntityManager() {
    for (EntityID entityID = 0; entityID < kMaxEntityID; ++entityID)
        mAvailableEntityIDs.push(entityID);
}

ecs::EntityID ecs::EntityManager::CreateEntity() {
    assert(mEntityCount < kMaxEntityID && "Entity limit exceeded");

    // Take available entityID at the front of queue
    auto entityID = mAvailableEntityIDs.front();
    mAvailableEntityIDs.pop();
    ++mEntityCount;

    return entityID;
}

void ecs::EntityManager::DestroyEntity(EntityID entityID) {
    assert(entityID < kMaxEntityID && "Entity out of range");

    // Invalidate entity signature
    mSignatures[entityID].reset();

    // Place entityID at the back of queue
    mAvailableEntityIDs.push(entityID);
    --mEntityCount;
}

void ecs::EntityManager::SetSignature(EntityID entityID, Signature const& signature) {
    assert(entityID < kMaxEntityID && "Entity out of range");
    mSignatures[entityID] = signature;
}

ecs::Signature const& ecs::EntityManager::GetSignature(EntityID entityID) {
    assert(entityID < kMaxEntityID && "Entity out of range");
    return mSignatures[entityID];
}


#endif