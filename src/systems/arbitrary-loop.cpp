#ifndef SYSTEMS_ARBITRARY_LOOP_CPP
#define SYSTEMS_ARBITRARY_LOOP_CPP

#include <components.hpp>
#include <systems.hpp>


systems::ArbitraryLoop::ArbitraryLoop(std::shared_ptr<ecs::Coordinator> coordinator) : ecs::ISystem(coordinator) {}

void systems::ArbitraryLoop::OnArbitraryEvent(std::shared_ptr<events::ArbitraryEvent> event) const {
    using namespace components;

    auto& position = mCoordinator->GetMember<Transform, Transform::kPosition>(event->entityID);
    position = { 0, 0 };
}


#endif