#ifndef ECS_ISYSTEM_CPP
#define ECS_ISYSTEM_CPP

#include <ecs.hpp>


ecs::ISystem::ISystem(std::shared_ptr<Coordinator> coordinator) : mCoordinator(coordinator) {}


#endif