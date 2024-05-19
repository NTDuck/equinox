#ifndef ECS_ENTITY_MANAGER_CPP
#define ECS_ENTITY_MANAGER_CPP

#include <cassert>
#include <ecs/entities.hpp>


ECS::EntityManager::EntityManager() {
    for (EntityID id = 0; id < config::kMaxEntityID; ++id) mAvailableEntityIDs.push(id);
}

ECS::EntityID ECS::EntityManager::CreateEntity() {
    assert(mEntityCount < config::kMaxEntityID && "Entity limit exceeded");

    // Retrieve ID from the front of queue
    auto entityID = mAvailableEntityIDs.front();
    mAvailableEntityIDs.pop();
    ++mEntityCount;

    return entityID;
}

void ECS::EntityManager::EraseEntity(EntityID entityID) {
    assert(entityID < config::kMaxEntityID && "Entity out of range");

    // Invalidate signature of deleted entity
    mEntityToSignatureMap[entityID].reset();

    // Place its ID at the back of queue
    mAvailableEntityIDs.push(entityID);
    --mEntityCount;
}

ECS::Signature const& ECS::EntityManager::GetSignature(EntityID entityID) const {
    assert(entityID < config::kMaxEntityID && "Entity out of range");
    return mEntityToSignatureMap[entityID];
}

void ECS::EntityManager::SetSignature(EntityID entityID, Signature const& signature) {
    assert(entityID < config::kMaxEntityID && "Entity out of range");
    mEntityToSignatureMap[entityID] = signature;
}


#endif