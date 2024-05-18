#ifndef SYSTEM_MANAGER_TPP
#define SYSTEM_MANAGER_TPP

#include <systems.hpp>


template <typename System>
std::shared_ptr<System> SystemManager::RegisterSystem() {
    std::string_view systemTypeName = typeid(System).name();
    assert(mSystems.find(systemTypeName) == mSystems.end() && "Registering system more than once");

    auto system = std::make_shared<System>();
    mSystems.insert({ systemTypeName, system });
    return system;
}

template <typename System>
void SystemManager::SetSignature(type::Signature const& signature) {
    std::string_view systemTypeName = typeid(System).name();
    assert(mSystems.find(systemTypeName) != mSystems.end() && "System used before registered");

    mSystemSignatures.insert({ systemTypeName, signature });
}


#endif