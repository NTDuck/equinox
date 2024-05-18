#ifndef ECS_COORDINATOR_TPP
#define ECS_COORDINATOR_TPP

#include <coordinators.hpp>


template <typename Component>
void ECSCoordinator::RegisterComponent() const {
    mComponentManager->RegisterComponent<Component>();
}

template <typename Component>
void ECSCoordinator::InsertComponent(type::EntityID entityID, Component const& component) const {
    mComponentManager->InsertComponent<Component>(entityID, component);

    auto signature = mEntityManager->GetSignature(entityID);
    signature.set(mComponentManager->GetComponentID<Component>(), true);
    mEntityManager->SetSignature(entityID, signature);

    mSystemManager->UpdateSignatureCallback(entityID, signature);
}

template <typename Component>
void ECSCoordinator::EraseComponent(type::EntityID entityID) const {
    mComponentManager->EraseComponent<Component>(entityID);

    auto signature = mEntityManager->GetSignature(entityID);
    signature.set(mComponentManager->GetComponentID<Component>(), false);
    mEntityManager->SetSignature(entityID, signature);

    mSystemManager->UpdateSignatureCallback(entityID, signature);
}

template <typename Component>
Component& ECSCoordinator::GetComponent(type::EntityID entityID) const {
    return mComponentManager->GetComponent<Component>(entityID);
}

template <typename Component>
type::ComponentID ECSCoordinator::GetComponentID() const {
    return mComponentManager->GetComponentID<Component>();
}

template <typename System>
std::shared_ptr<System> ECSCoordinator::RegisterSystem() const {
    return mSystemManager->RegisterSystem<System>();
}

template <typename System>
void ECSCoordinator::SetSystemSignature(type::Signature const& signature) const {
    mSystemManager->SetSignature<System>(signature);
}


#endif