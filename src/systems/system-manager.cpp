#ifndef SYSTEM_MANAGER_CPP
#define SYSTEM_MANAGER_CPP

#include <systems.hpp>


void SystemManager::EraseEntityCallback(type::EntityID entityID) const {
    for (auto const& pair : mSystems) {
        auto const& system = pair.second;
        system->mEntityIDs.erase(entityID);
    }
}

void SystemManager::UpdateSignatureCallback(type::EntityID entityID, type::Signature const& entitySignature) const {
    for (auto& pair : mSystems) {
        auto const& systemTypeName = pair.first;
        auto const& system = pair.second;
        auto const& systemSignature = mSystemSignatures.at(systemTypeName);

        if ((entitySignature & systemSignature) == systemSignature) system->mEntityIDs.insert(entityID);
        else system->mEntityIDs.erase(entityID);
    }
}


#endif