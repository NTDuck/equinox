#ifndef ECS_COORDINATOR_TPP
#define ECS_COORDINATOR_TPP

#include <ecs/coordinators.hpp>


template <typename Component>
void ecs::Coordinator::RegisterComponent() const {
    mComponentManager->RegisterComponent<Component>();
}

template <typename... Components>
void ecs::Coordinator::RegisterComponents() const {
    (RegisterComponent<Components>(), ...);
}

template <typename Component>
void ecs::Coordinator::InsertComponent(EntityID entityID, Component const& component) const {
    mComponentManager->InsertComponent<Component>(entityID, component);

    auto signature = mEntityManager->GetSignature(entityID);
    signature.set(mComponentManager->GetComponentID<Component>(), true);
    mEntityManager->SetSignature(entityID, signature);

    mSystemManager->UpdateSignatureCallback(entityID, signature);
}

template <typename Component>
void ecs::Coordinator::EraseComponent(EntityID entityID) const {
    mComponentManager->EraseComponent<Component>(entityID);

    auto signature = mEntityManager->GetSignature(entityID);
    signature.set(mComponentManager->GetComponentID<Component>(), false);
    mEntityManager->SetSignature(entityID, signature);

    mSystemManager->UpdateSignatureCallback(entityID, signature);
}

template <typename Component>
Component& ecs::Coordinator::GetComponent(EntityID entityID) const {
    return mComponentManager->GetComponent<Component>(entityID);
}

template <typename Component>
ecs::ComponentID ecs::Coordinator::GetComponentID() const {
    return mComponentManager->GetComponentID<Component>();
}

template <typename System, typename... Args>
std::shared_ptr<System> ecs::Coordinator::RegisterSystem(Args&&... args) const {
    return mSystemManager->RegisterSystem<System>(std::forward<Args>(args)...);
}

template <typename System>
void ecs::Coordinator::SetSystemSignature(Signature const& signature) const {
    mSystemManager->SetSignature<System>(signature);
}

template <typename System, typename... Components>
void ecs::Coordinator::SetSystemSignature() const {
    ecs::Signature signature;
    (signature.set(GetComponentID<Components>()), ...);
    SetSystemSignature<System>(signature);
}


#endif