#ifndef ECS_COORDINATOR_CPP
#define ECS_COORDINATOR_CPP

#include <ecs/coordinators.hpp>


ECS::Coordinator::Coordinator() {
    mEntityManager = std::make_unique<EntityManager>();
    mComponentManager = std::make_unique<ComponentManager>();
    mSystemManager = std::make_unique<SystemManager>();
}

ECS::EntityID ECS::Coordinator::CreateEntity() const {
    return mEntityManager->CreateEntity();
}

void ECS::Coordinator::EraseEntity(EntityID entityID) const {
    mEntityManager->EraseEntity(entityID);
    mComponentManager->EraseEntityCallback(entityID);
    mSystemManager->EraseEntityCallback(entityID);
}


#endif