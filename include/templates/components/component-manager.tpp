#ifndef COMPONENT_MANAGER_TPP
#define COMPONENT_MANAGER_TPP

#include <components.hpp>
#include <auxiliaries.hpp>


template <typename Component>
void ComponentManager::RegisterComponent() {
	assert(std::is_pod<Component>::value && "Component is not of POD type");

    std::string_view componentTypeName = typeid(Component).name();
	assert(mComponentIDs.find(componentTypeName) == mComponentIDs.end() && "Registering component type more than once");

	// Add this component type to the component type map
	// Increment the value so that the next component registered will be different
	mComponentIDs.insert({ componentTypeName, mNextComponentID++ });

	// Create a ComponentArray pointer and add it to the component arrays map
	mComponentArrays.insert({ componentTypeName, std::make_shared<ComponentArray<Component>>() });
}

template <typename Component>
type::ComponentID ComponentManager::GetComponentID() const {
    std::string_view componentTypeName = typeid(Component).name();
	assert(mComponentIDs.find(componentTypeName) != mComponentIDs.end() && "Component not registered before use.");
	return mComponentIDs.at(componentTypeName);   // `operator[]` performs insertion with default-constructed value when key does not exist, therefore is not allowed in a `const` method
}

template <typename Component>
void ComponentManager::InsertComponent(type::EntityID entityID, Component const& component) {
	GetComponentArray<Component>()->InsertComponent(entityID, component);
}

template <typename Component>
void ComponentManager::EraseComponent(type::EntityID entityID) {
	GetComponentArray<Component>()->EraseComponent(entityID);
}

template <typename Component>
Component& ComponentManager::GetComponent(type::EntityID entityID) {
	return GetComponentArray<Component>()->GetComponent(entityID);
}

template <typename Component>
std::shared_ptr<ComponentArray<Component>> ComponentManager::GetComponentArray() {
    std::string_view componentTypeName = typeid(Component).name();
	assert(mComponentIDs.find(componentTypeName) != mComponentIDs.end() && "Component not registered before use.");
	return std::static_pointer_cast<ComponentArray<Component>>(mComponentArrays[componentTypeName]);
}


#endif