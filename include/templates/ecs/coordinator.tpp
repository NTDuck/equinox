#ifndef ECS_COORDINATOR_TPP
#define ECS_COORDINATOR_TPP

#include <ecs.hpp>


template <typename Component>
void ecs::Coordinator::RegisterComponent() const {
    mComponentManager->RegisterComponent<Component>();
}

template <typename Component>
void ecs::Coordinator::AddComponent(EntityID entityID, Component const& component) const {
    mComponentManager->AddComponent<Component>(entityID, component);

    auto signature = mEntityManager->GetSignature(entityID);
    signature.set(mComponentManager->GetComponentID<Component>(), true);
    mEntityManager->SetSignature(entityID, signature);

    mSystemManager->EntitySignatureUpdatedCallback(entityID, signature);
}

template <typename Component, typename... Args>
void ecs::Coordinator::EmplaceComponent(EntityID entityID, Args&&... args) const {
    AddComponent<Component>(entityID, std::forward<Args>(args)...);
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


#endif