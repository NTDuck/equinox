#ifndef ECS_COORDINATOR_TPP
#define ECS_COORDINATOR_TPP

#include <ecs.hpp>


template <typename Component>
void ecs::Coordinator::RegisterComponent() const {
    mComponentManager->RegisterComponent<Component>();
}

template <typename Component>
void ecs::Coordinator::AddComponent(EntityID entityID, typename Component::Object const& component) const {
    mComponentManager->AddComponent<Component>(entityID, component);

    auto signature = mEntityManager->GetSignature(entityID);
    signature.set(mComponentManager->GetComponentID<Component>(), true);
    mEntityManager->SetSignature(entityID, signature);

    mSystemManager->EntitySignatureUpdatedCallback(entityID, signature);
}

template <typename Component>
void ecs::Coordinator::RemoveComponent(EntityID entityID) const {
    mComponentManager->RemoveComponent<Component>(entityID);

    auto signature = mEntityManager->GetSignature(entityID);
    signature.set(mComponentManager->GetComponentID<Component>(), false);
    mEntityManager->SetSignature(entityID, signature);

    mSystemManager->EntitySignatureUpdatedCallback(entityID, signature);
}

template <typename Component, std::size_t I>
decltype(auto) ecs::Coordinator::GetMember(EntityID entityID) const {
    return mComponentManager->GetMember<Component, I>(entityID);
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
    Signature signature;
    (signature.set(GetComponentID<Components>()), ...);
    SetSystemSignature<System>(signature);
}

#endif