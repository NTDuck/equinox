#ifndef ECS_SYSTEM_MANAGER_TPP
#define ECS_SYSTEM_MANAGER_TPP

#include <cassert>
#include <functional>
#include <ecs/systems.hpp>


template <typename System, typename... Args>
std::shared_ptr<System> ecs::SystemManager::RegisterSystem(Args&&... args) {
    std::string_view systemTypeName = typeid(System).name();
    assert(mSystems.find(systemTypeName) == mSystems.end() && "Registering system more than once");

    auto system = std::make_shared<System>(std::forward<Args>(args)...);
    mSystems.insert({ systemTypeName, system });
    return system;
}

template <typename System>
void ecs::SystemManager::SetSignature(Signature const& signature) {
    std::string_view systemTypeName = typeid(System).name();
    assert(mSystems.find(systemTypeName) != mSystems.end() && "System used before registered");

    mSystemSignatures.insert({ systemTypeName, signature });
}


#endif