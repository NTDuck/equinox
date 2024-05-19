#ifndef ECS_COORDINATOR_TPP
#define ECS_COORDINATOR_TPP

#include <ecs/coordinators.hpp>


template <typename Component>
void ECS::Coordinator::RegisterComponent() const {
    mComponentManager->RegisterComponent<Component>();
}

template <typename Component>
void ECS::Coordinator::InsertComponent(EntityID entityID, Component const& component) const {
    mComponentManager->InsertComponent<Component>(entityID, component);

    auto signature = mEntityManager->GetSignature(entityID);
    signature.set(mComponentManager->GetComponentID<Component>(), true);
    mEntityManager->SetSignature(entityID, signature);

    mSystemManager->UpdateSignatureCallback(entityID, signature);
}

template <typename Component>
void ECS::Coordinator::EraseComponent(EntityID entityID) const {
    mComponentManager->EraseComponent<Component>(entityID);

    auto signature = mEntityManager->GetSignature(entityID);
    signature.set(mComponentManager->GetComponentID<Component>(), false);
    mEntityManager->SetSignature(entityID, signature);

    mSystemManager->UpdateSignatureCallback(entityID, signature);
}

template <typename Component>
Component& ECS::Coordinator::GetComponent(EntityID entityID) const {
    return mComponentManager->GetComponent<Component>(entityID);
}

template <typename Component>
ECS::ComponentID ECS::Coordinator::GetComponentID() const {
    return mComponentManager->GetComponentID<Component>();
}

template <typename System>
std::shared_ptr<System> ECS::Coordinator::RegisterSystem() const {
    return mSystemManager->RegisterSystem<System>();
}

template <typename System>
void ECS::Coordinator::SetSystemSignature(Signature const& signature) const {
    mSystemManager->SetSignature<System>(signature);
}


#endif