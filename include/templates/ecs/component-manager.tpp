#ifndef ECS_COMPONENT_MANAGER_TPP
#define ECS_COMPONENT_MANAGER_TPP

#include <cassert>

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

    return mComponentIDs.at(componentTypeName);
}

template <typename Component>
void ecs::ComponentManager::AddComponent(EntityID entityID, typename Component::Object const& component) {
    GetComponentArray<Component>()->Insert(entityID, component);
}

template <typename Component>
void ecs::ComponentManager::RemoveComponent(EntityID entityID) {
    GetComponentArray<Component>()->Erase(entityID);
}

template <typename Component, std::size_t I>
decltype(auto) ecs::ComponentManager::GetMember(EntityID entityID) {
    return GetComponentArray<Component>()->template Get<I>(entityID);
}

template <ecs::ext::ComponentMember M, typename ComponentMap>
decltype(auto) ecs::ComponentManager::GetMember(EntityID entityID) {
    using ComponentMapData = typename ComponentMap::Get<M>;
    return GetMember<typename ComponentMapData::Component, ComponentMapData::Index>(entityID);
}

template <typename Component>
std::shared_ptr<ecs::ComponentArray<Component>> ecs::ComponentManager::GetComponentArray() {
    auto componentTypeName = utility::GetTypeName<Component>();
    assert(mComponentIDs.find(componentTypeName) != mComponentIDs.end() && "Component not registered before use");

    return std::static_pointer_cast<ComponentArray<Component>>(mComponentArrays[componentTypeName]);
}


#endif