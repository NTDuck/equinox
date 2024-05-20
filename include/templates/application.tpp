#ifndef APPLICATION_TPP
#define APPLICATION_TPP

#include <functional>
#include <application.hpp>


template <typename T>
template <typename... Args>
T* Application::DefaultAllocator<T>::operator()(Args&&... args) const {
    return new T(std::forward<Args>(args)...);
}

template <typename T>
void Application::DefaultDeallocator<T>::operator()(T* ptr) const {
    delete ptr;
}

template <typename T, typename Allocator, typename Deallocator>
Application::Dependency<T, Allocator, Deallocator>::~Dependency() {
    mPointer.reset();
}

template <typename T, typename Allocator, typename Deallocator>
bool Application::Dependency<T, Allocator, Deallocator>::Empty() const noexcept {
    return Get() == nullptr;
}

template <typename T, typename Allocator, typename Deallocator>
T* Application::Dependency<T, Allocator, Deallocator>::Get() const noexcept {
    return mPointer.get();
}

template <typename T, typename Allocator, typename Deallocator>
template <Allocator allocator, Deallocator deallocator, typename... Args>
void Application::Dependency<T, Allocator, Deallocator>::Initialize(Args&&... args) {
    T* rawPointer = std::invoke(allocator, std::forward<Args>(args)...);
    assert(rawPointer && "Failed to allocate dependency");
    mPointer = std::shared_ptr<T>(rawPointer, deallocator);
}


#endif