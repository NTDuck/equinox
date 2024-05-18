#ifndef ECS_COORDINATOR_CPP
#define ECS_COORDINATOR_CPP

#include <coordinators.hpp>


ECSCoordinator::ECSCoordinator() {
    mEntityManager = std::make_unique<EntityManager>();
    mComponentManager = std::make_unique<ComponentManager>();
    mSystemManager = std::make_unique<SystemManager>();
}

type::EntityID ECSCoordinator::CreateEntity() const {
    return mEntityManager->CreateEntity();
}

void ECSCoordinator::EraseEntity(type::EntityID entityID) const {
    mEntityManager->EraseEntity(entityID);
    mComponentManager->EraseEntityCallback(entityID);
    mSystemManager->EraseEntityCallback(entityID);
}


ECSCoordinator global::coordinator;


#endif