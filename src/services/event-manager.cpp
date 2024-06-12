#ifndef SERVICES_EVENT_MANAGER_CPP
#define SERVICES_EVENT_MANAGER_CPP

#include <services.hpp>


EventManager::EventManager(std::shared_ptr<ecs::Coordinator> coordinator) : mCoordinator(coordinator) {}

void EventManager::Integrate() const {
    while (Dequeue());
}

std::shared_ptr<ecs::IEvent> EventManager::Dequeue() const {
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

    case SDL_APP_TERMINATING:
    case SDL_APP_LOWMEMORY:
    case SDL_APP_WILLENTERBACKGROUND:
    case SDL_APP_DIDENTERBACKGROUND:
    case SDL_APP_WILLENTERFOREGROUND:
    case SDL_APP_DIDENTERFOREGROUND:
    case SDL_LOCALECHANGED:
        return nullptr;

    case SDL_DISPLAYEVENT:
        return std::make_shared<Event<SDL_DisplayEvent>>(event.display);

    case SDL_WINDOWEVENT:
        return std::make_shared<Event<SDL_WindowEvent>>(event.window);

    case SDL_SYSWMEVENT:
        return std::make_shared<Event<SDL_SysWMEvent>>(event.syswm);

    case SDL_KEYDOWN:
    case SDL_KEYUP:
        return std::make_shared<Event<SDL_KeyboardEvent>>(event.key);
        
    case SDL_TEXTEDITING:
        return std::make_shared<Event<SDL_TextEditingEvent>>(event.edit);

    case SDL_TEXTINPUT:
        return std::make_shared<Event<SDL_TextInputEvent>>(event.text);

    case SDL_KEYMAPCHANGED:
        return nullptr;

    case SDL_TEXTEDITING_EXT:
        return std::make_shared<Event<SDL_TextEditingExtEvent>>(event.editExt);

    case SDL_MOUSEMOTION:
        return std::make_shared<Event<SDL_MouseMotionEvent>>(event.motion);

    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
        return std::make_shared<Event<SDL_MouseButtonEvent>>(event.button);

    case SDL_MOUSEWHEEL:
        return std::make_shared<Event<SDL_MouseWheelEvent>>(event.wheel);

    case SDL_JOYAXISMOTION:
        return std::make_shared<Event<SDL_JoyAxisEvent>>(event.jaxis);

    case SDL_JOYBALLMOTION:
        return std::make_shared<Event<SDL_JoyBallEvent>>(event.jball);

    case SDL_JOYHATMOTION:
        return std::make_shared<Event<SDL_JoyHatEvent>>(event.jhat);

    case SDL_JOYBUTTONDOWN:
    case SDL_JOYBUTTONUP:
        return std::make_shared<Event<SDL_JoyButtonEvent>>(event.jbutton);

    case SDL_JOYDEVICEADDED:
    case SDL_JOYDEVICEREMOVED:
        return std::make_shared<Event<SDL_JoyDeviceEvent>>(event.jdevice);

    case SDL_JOYBATTERYUPDATED:
        return std::make_shared<Event<SDL_JoyBatteryEvent>>(event.jbattery);

    case SDL_CONTROLLERAXISMOTION:
        return std::make_shared<Event<SDL_ControllerAxisEvent>>(event.caxis);

    case SDL_CONTROLLERBUTTONDOWN:
    case SDL_CONTROLLERBUTTONUP:
        return std::make_shared<Event<SDL_ControllerButtonEvent>>(event.cbutton);

    case SDL_CONTROLLERDEVICEADDED:
    case SDL_CONTROLLERDEVICEREMOVED:
    case SDL_CONTROLLERDEVICEREMAPPED:
        return std::make_shared<Event<SDL_ControllerDeviceEvent>>(event.cdevice);

    case SDL_CONTROLLERTOUCHPADDOWN:
    case SDL_CONTROLLERTOUCHPADMOTION:
    case SDL_CONTROLLERTOUCHPADUP:
        return std::make_shared<Event<SDL_ControllerTouchpadEvent>>(event.ctouchpad);

    case SDL_CONTROLLERSENSORUPDATE:
        return std::make_shared<Event<SDL_ControllerSensorEvent>>(event.csensor);

    case SDL_CONTROLLERSTEAMHANDLEUPDATED:
        return nullptr;

    case SDL_FINGERDOWN:
    case SDL_FINGERUP:
    case SDL_FINGERMOTION:
        return std::make_shared<Event<SDL_TouchFingerEvent>>(event.tfinger);

    case SDL_DOLLARGESTURE:
        return std::make_shared<Event<SDL_DollarGestureEvent>>(event.dgesture);

    case SDL_DOLLARRECORD:
    case SDL_MULTIGESTURE:
    case SDL_CLIPBOARDUPDATE:
        return nullptr;
    
    case SDL_DROPFILE:
    case SDL_DROPTEXT:
    case SDL_DROPBEGIN:
    case SDL_DROPCOMPLETE:
        return std::make_shared<Event<SDL_DropEvent>>(event.drop);

    case SDL_AUDIODEVICEADDED:
    case SDL_AUDIODEVICEREMOVED:
        return std::make_shared<Event<SDL_AudioDeviceEvent>>(event.adevice);

    case SDL_SENSORUPDATE:
        return std::make_shared<Event<SDL_SensorEvent>>(event.sensor);

    case SDL_RENDER_TARGETS_RESET:
    case SDL_RENDER_DEVICE_RESET:
    case SDL_POLLSENTINEL:
    case SDL_USEREVENT:   // We won't be using this
        return nullptr;

    default:
        return nullptr;
    }
}


#endif