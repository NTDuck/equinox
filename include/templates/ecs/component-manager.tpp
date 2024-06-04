#ifndef ECS_COMPONENT_MANAGER_TPP
#define ECS_COMPONENT_MANAGER_TPP

#include <cassert>

#include <ecs.hpp>
#include <utilities.hpp>


template <typename Component>
void ecs::ComponentManager::RegisterComponent() {
    auto componentTypeIndex = std::type_index(typeid(Component));
    assert(mComponentIDs.find(componentTypeIndex) == mComponentIDs.end() && "Registering component type more than once");

    mComponentIDs.insert({ componentTypeIndex, mNextComponentID++ });
    mComponentArrays.insert({ componentTypeIndex, std::make_shared<ComponentArray<Component>>() });
}

template <typename Component>
ecs::ComponentID ecs::ComponentManager::GetComponentID() const {
    auto componentTypeIndex = std::type_index(typeid(Component));
    assert(mComponentIDs.find(componentTypeIndex) != mComponentIDs.end() && "Component not registered before use");

    return mComponentIDs.at(componentTypeIndex);
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

template <typename Component>
std::shared_ptr<ecs::ComponentArray<Component>> ecs::ComponentManager::GetComponentArray() {
    auto componentTypeIndex = std::type_index(typeid(Component));
    assert(mComponentIDs.find(componentTypeIndex) != mComponentIDs.end() && "Component not registered before use");

    return std::static_pointer_cast<ComponentArray<Component>>(mComponentArrays[componentTypeIndex]);
}


#endif