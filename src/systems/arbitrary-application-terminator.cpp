#ifndef SYSTEMS_ARBITRARY_TERMINATOR_CPP
#define SYSTEMS_ARBITRARY_TERMINATOR_CPP

#include <systems.hpp>


systems::ArbitraryApplicationTerminator::ArbitraryApplicationTerminator() : ecs::ISystem(nullptr) {}

void systems::ArbitraryApplicationTerminator::Integrate(std::shared_ptr<EventManager::Event<SDL_QuitEvent>> event) noexcept {
    (void)(event);
    isRunning = false;
}

bool systems::ArbitraryApplicationTerminator::GetStatus() const noexcept {
    return isRunning;
}


#endif