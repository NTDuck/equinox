#ifndef APPLICATION_TPP
#define APPLICATION_TPP

#include <application.hpp>


template <typename T, typename Allocator, typename Deleter>
Application::Dependency<T, Allocator, Deleter>::~Dependency() {
    mPointer.reset();
}

template <typename T, typename Allocator, typename Deleter>
bool Application::Dependency<T, Allocator, Deleter>::Empty() const noexcept {
    return Get() == nullptr;
}

template <typename T, typename Allocator, typename Deleter>
T* Application::Dependency<T, Allocator, Deleter>::Get() const noexcept {
    return mPointer.get();
}

template <typename T, typename Allocator, typename Deleter>
template <typename... Args>
void Application::Dependency<T, Allocator, Deleter>::Initialize(Allocator allocator, Deleter deleter, Args&&... args) {
    T* rawPointer = std::invoke(allocator, std::forward<Args>(args)...);
    assert(rawPointer && "Failed to create dependency");
    mPointer = std::shared_ptr<T>(rawPointer, deleter);    
}

template <typename... Args>
void Application::Window::Initialize(Args&&... args) {
    Application::Dependency<SDL_Window, decltype(SDL_CreateWindow), decltype(SDL_DestroyWindow)>::Initialize(std::forward<Args>(args)...);
    mID = SDL_GetWindowID(Get());
}


#endif