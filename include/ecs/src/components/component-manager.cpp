#ifndef ECS_COMPONENT_MANAGER_CPP
#define ECS_COMPONENT_MANAGER_CPP

#include <ecs/components.hpp>


void ecs::ComponentManager::EraseEntityCallback(EntityID entityID) {
	for (auto const& pair : mComponentArrays) {
		auto const& componentArray = pair.second;
		componentArray->EraseEntityCallback(entityID);
	}
}


#endif