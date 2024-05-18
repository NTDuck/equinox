#ifndef ENTITY_MANAGER_CPP
#define ENTITY_MANAGER_CPP

#include <entities.hpp>
#include <auxiliaries.hpp>


EntityManager::EntityManager() {
    for (type::EntityID id = 0; id < config::kMaxEntityID; ++id) mAvailableEntities.push(id);
}

type::EntityID EntityManager::CreateEntity() {
    assert(mEntityCount < config::kMaxEntityID && "Entity limit exceeded");

    // Retrieve ID from the front of queue
    auto entityID = mAvailableEntities.front();
    mAvailableEntities.pop();
    ++mEntityCount;

    return entityID;
}

void EntityManager::EraseEntity(type::EntityID entityID) {
    assert(entityID < config::kMaxEntityID && "Entity out of range");

    // Invalidate signature of deleted entity
    mEntityToSignatureMap[entityID].reset();

    // Place its ID at the back of queue
    mAvailableEntities.push(entityID);
    --mEntityCount;
}

type::Signature const& EntityManager::GetSignature(type::EntityID entityID) const {
    assert(entityID < config::kMaxEntityID && "Entity out of range");
    return mEntityToSignatureMap[entityID];
}

void EntityManager::SetSignature(type::EntityID entityID, type::Signature const& signature) {
    assert(entityID < config::kMaxEntityID && "Entity out of range");
    mEntityToSignatureMap[entityID] = signature;
}


#endif