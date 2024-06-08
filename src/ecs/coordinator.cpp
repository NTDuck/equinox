#ifndef ECS_COORDINATOR_CPP
#define ECS_COORDINATOR_CPP

#include <ecs.hpp>


ecs::Coordinator::Coordinator() {
    mEntityManager = std::make_unique<EntityManager>();
    mComponentManager = std::make_unique<ComponentManager>();
    mSystemManager = std::make_unique<SystemManager>();
    mEventManager = std::make_unique<EventManager>();
}

ecs::EntityID ecs::Coordinator::CreateEntity() const {
    return mEntityManager->CreateEntity();
}

void ecs::Coordinator::DestroyEntity(EntityID entityID) const {
    mEntityManager->DestroyEntity(entityID);
    mComponentManager->EntityDestroyedCallback(entityID);
    mSystemManager->EntityDestroyedCallback(entityID);
}


#endif