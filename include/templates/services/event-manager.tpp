#ifndef SERVICES_EVENT_MANAGER_TPP
#define SERVICES_EVENT_MANAGER_TPP

#include <services.hpp>


template <typename NativeEvent>
EventManager::Event<NativeEvent>::Event(NativeEvent const& event) : NativeEvent(event) {}

/**
 * @note Should rightfully be static, but C++ standard prohibits.
*/
template <typename NativeEvent>
void EventManager::Event<NativeEvent>::Publish(std::shared_ptr<ecs::Coordinator> coordinator, std::shared_ptr<ecs::IEvent> event) const {
    coordinator->PublishEvent(std::static_pointer_cast<Event<NativeEvent>>(event));
}



#endif