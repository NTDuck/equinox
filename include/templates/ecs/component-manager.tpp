#ifndef ECS_COMPONENT_MANAGER_TPP
#define ECS_COMPONENT_MANAGER_TPP

#include <ecs.hpp>
#include <utilities.hpp>


template <typename Component>
void ecs::ComponentManager::RegisterComponent() {
    auto componentTypeName = utility::GetTypeName<Component>();
    assert(mComponentIDs.find(componentTypeName) == mComponentIDs.end() && "Registering component type more than once");

    mComponentIDs.insert({ componentTypeName, mNextComponentID++ });
    mComponentArrays.insert({ componentTypeName, std::make_shared<ComponentArray<Component>>() });
}

template <typename Component>
ecs::ComponentID ecs::ComponentManager::GetComponentID() const {
    auto componentTypeName = utility::GetTypeName<Component>();
    assert(mComponentIDs.find(componentTypeName) != mComponentIDs.end() && "Component not registered before use");

    return mComponentIDs[componentTypeName];
}

template <typename Component>
void ecs::ComponentManager::AddComponent(EntityID entityID, Component const& component) {
    GetComponentArray<Component>()->Insert(entityID, component);
}

template <typename Component>
void ecs::ComponentManager::RemoveComponent(EntityID entityID) {
    GetComponentArray<Component>()->Erase(entityID);
}

template <typename Component>
Component& ecs::ComponentManager::GetComponent(EntityID entityID) {
    return GetComponentArray<Component>()->Get(entityID);
}

template <typename Component>
std::shared_ptr<ecs::ComponentArray<Component>> ecs::ComponentManager::GetComponentArray() {
    auto componentTypeName = utility::GetTypeName<Component>();
    assert(mComponentIDs.find(componentTypeName) != mComponentIDs.end() && "Component not registered before use");

    return std::static_pointer_cast<ComponentArray<Component>>(mComponentArrays[componentTypeName]);
}


#endif