#ifndef COMPONENT_MANAGER_CPP
#define COMPONENT_MANAGER_CPP

#include <components.hpp>
#include <auxiliaries.hpp>


void ComponentManager::EraseEntityCallback(type::EntityID entityID) {
	for (auto const& pair : mComponentArrays) {
		auto const& componentArray = pair.second;
		componentArray->EraseEntityCallback(entityID);
	}
}


#endif