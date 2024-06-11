#ifndef SERVICES_EVENT_MANAGER_CPP
#define SERVICES_EVENT_MANAGER_CPP

#include <services.hpp>


EventManager::EventManager(std::shared_ptr<ecs::Coordinator> coordinator) : mCoordinator(coordinator) {}

std::shared_ptr<ecs::IEvent> EventManager::Dequeue() {
    if (!SDL_PollEvent(&mNativeEventUnion))
        return nullptr;

    auto event = Discriminate(mNativeEventUnion);

    // Publish requires casting to proper type
    if (mSubscribedEventTypes.find(mNativeEventUnion.type) != mSubscribedEventTypes.end())
        event->Publish(mCoordinator, event);

    return event;
}

void EventManager::Enqueue(std::shared_ptr<ecs::IEvent> event) const {
    (void)(event);
}

void EventManager::SubscribeEventType(EventType eventType) {
    mSubscribedEventTypes.insert(eventType);
}

void EventManager::UnsubscribeEventType(EventType eventType) {
    mSubscribedEventTypes.erase(eventType);
}

/**
 * @see https://wiki.libsdl.org/SDL2/SDL_EventType
*/
std::shared_ptr<EventManager::IEvent> EventManager::Discriminate(NativeEventUnion const& event) {
    switch (event.type) {
    case SDL_QUIT:
        return std::make_shared<Event<SDL_QuitEvent>>(event.quit);

    case SDL_DISPLAYEVENT:
        return std::make_shared<Event<SDL_DisplayEvent>>(event.display);

    default:
        return nullptr;
    }
}

    // case SDL_WINDOWEVENT:
    //     return mCoordinator->PublishEvent(std::make_shared<SDL_WindowEvent>(event->window));

    // case SDL_KEYDOWN:
    // case SDL_KEYUP:
    //     return mCoordinator->PublishEvent(std::make_shared<SDL_KeyboardEvent>(event->key));

    // case SDL_TEXTEDITING:
    //     return mCoordinator->PublishEvent(std::make_shared<SDL_TextEditingEvent>(event->edit));
    
    // case SDL_TEXTINPUT:
    //     return mCoordinator->PublishEvent(std::make_shared<SDL_TextInputEvent>(event->text));

    // case SDL_KEYMAPCHANGED:
    //     return;

    // case SDL_MOUSEMOTION:
    //     return mCoordinator->PublishEvent(std::make_shared<SDL_MouseMotionEvent>(event->motion));

    // case SDL_MOUSEBUTTONDOWN:
    // case SDL_MOUSEBUTTONUP:
    //     return mCoordinator->PublishEvent(std::make_shared<SDL_MouseMotionEvent>(event->motion));

    // default:
    //     return;
    // }

// std::shared_ptr<EventQueue::IEvent> EventQueue::GetUnderlyingType(std::shared_ptr<Event> event) noexcept {
//     switch (event->type) {
//         // case 0:
//         //     return event.adevice;
        
//         // case 
//         //     return event.common;
//         //     return event.window;
//         //     return event.key;
//         //     return event.edit;
//         //     return event.editExt;
//         //     return event.text;

//             // return event.wheel;
//             // return event.jaxis;
//             // return event.jball;
//             // return event.jhat;
//             // return event.jbutton;
//             // return event.jdevice;
//             // return event.jbattery;
//             // return event.caxis;
//             // return event.cbutton;
//             // return event.cdevice;
//             // return event.ctouchpad;
//             // return event.csensor;
//             // return event.adevice;
//             // return event.sensor;
//             // return event.quit;
//             // return event.user;
//             // return event.syswm;
//             // return event.tfinger;
//             // return event.mgesture;
//             // return event.dgesture;
//             // return event.drop;
//     }
// }


#endif