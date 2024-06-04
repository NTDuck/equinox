#ifndef ECS_SYSTEM_MANAGER_TPP
#define ECS_SYSTEM_MANAGER_TPP

#include <functional>
#include <memory>

#include <utilities.hpp>
#include <ecs.hpp>


template <typename System, typename... Args>
std::shared_ptr<System> ecs::SystemManager::RegisterSystem(Args&&... args) {
    auto systemTypeIndex = std::type_index(typeid(System));
    assert(mSystems.find(systemTypeIndex) == mSystems.end() && "Registering system more than once");

    auto system = std::make_shared<System>(std::forward<Args>(args)...);
    mSystems.insert({ systemTypeIndex, system });
    return system;
}

template <typename System>
void ecs::SystemManager::SetSignature(Signature const& signature) {
    auto systemTypeIndex = std::type_index(typeid(System));
    assert(mSystems.find(systemTypeIndex) != mSystems.end() && "System used before registered");    

    mSignatures.insert({ systemTypeIndex, signature });
}


#endif