#ifndef ECS_EVENT_MANAGER_TPP
#define ECS_EVENT_MANAGER_TPP

#include <cassert>
#include <ecs.hpp>


template <decltype(auto) Callback>
ecs::EventHandler<Callback>::EventHandler(std::shared_ptr<System> system) : mSystem(system) {}

template <decltype(auto) Callback>
void ecs::EventHandler<Callback>::Invoke(std::shared_ptr<IEvent> event) const {
    std::invoke(Callback, mSystem, std::static_pointer_cast<Event>(event));
}

template <typename Event>
void ecs::EventManager::Publish(std::shared_ptr<Event> event) {
    auto& eventHandlers = mSubscribers[typeid(Event)];
    for (auto const& eventHandler : eventHandlers)
        eventHandler->Invoke(event);
}

template <decltype(auto) Callback>
void ecs::EventManager::Subscribe(std::shared_ptr<typename EventHandler<Callback>::System> system) {
    auto& eventHandlers = mSubscribers[typeid(typename EventHandler<Callback>::Event)];
    eventHandlers.insert(std::make_unique<EventHandler<Callback>>(system));
}

template <decltype(auto) Callback>
void ecs::EventManager::Subscribe(typename EventHandler<Callback>::System* system) {
    auto& eventHandlers = mSubscribers[typeid(typename EventHandler<Callback>::Event)];
    eventHandlers.insert(std::make_unique<EventHandler<Callback>>(system));    
}


#endif