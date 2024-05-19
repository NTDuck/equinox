#ifndef ECS_SYSTEM_MANAGER_CPP
#define ECS_SYSTEM_MANAGER_CPP

#include <ecs/systems.hpp>


void ECS::SystemManager::EraseEntityCallback(EntityID entityID) const {
    for (auto const& pair : mSystems) {
        auto const& system = pair.second;
        system->mEntityIDs.erase(entityID);
    }
}

void ECS::SystemManager::UpdateSignatureCallback(EntityID entityID, Signature const& entitySignature) const {
    for (auto& pair : mSystems) {
        auto const& systemTypeName = pair.first;
        auto const& system = pair.second;
        auto const& systemSignature = mSystemSignatures.at(systemTypeName);

        if ((entitySignature & systemSignature) == systemSignature) system->mEntityIDs.insert(entityID);
        else system->mEntityIDs.erase(entityID);
    }
}


#endif