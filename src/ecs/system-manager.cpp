#ifndef ECS_SYSTEM_MANAGER_CPP
#define ECS_SYSTEM_MANAGER_CPP

#include <ecs.hpp>


void ecs::SystemManager::EntityDestroyedCallback(EntityID entityID) {
    for (auto const& [systemTypeName, system] : mSystems)
        system->mEntityIDs.erase(entityID);
}

void ecs::SystemManager::EntitySignatureUpdatedCallback(EntityID entityID, Signature const& entitySignature) {
    for (auto const& [systemTypeIndex, system] : mSystems) {
        auto const& systemSignature = mSignatures[systemTypeIndex];
        
        if ((entitySignature & systemSignature) == systemSignature)
            system->mEntityIDs.insert(entityID);
        else
            system->mEntityIDs.erase(entityID);
    }
}


#endif