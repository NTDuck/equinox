#ifndef ECS_SYSTEM_MANAGER_TPP
#define ECS_SYSTEM_MANAGER_TPP

#include <functional>
#include <memory>

#include <utilities.hpp>
#include <ecs.hpp>


template <typename System, typename... Args>
std::shared_ptr<System> ecs::SystemManager::RegisterSystem(Args&&... args) {
    auto systemTypeName = utility::GetTypeName<System>();
    assert(mSystems.find(systemTypeName) == mSystems.end() && "Registering system more than once");

    auto system = std::make_shared<System>(std::forward<Args>(args)...);
    mSystems.insert({ systemTypeName, system });
    return system;
}

template <typename System>
void ecs::SystemManager::SetSignature(Signature const& signature) {
    auto systemTypeName = utility::GetTypeName<System>();
    assert(mSystems.find(systemTypeName) != mSystems.end() && "System used before registered");    

    mSignatures.insert({ systemTypeName, signature });
}


#endif