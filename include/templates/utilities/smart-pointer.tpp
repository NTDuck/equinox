#ifndef UTILITIES_POINTER_TPP
#define UTILITIES_POINTER_TPP

#include <functional>
#include <type_traits>

#include <utilities.hpp>


template <typename T, decltype(auto) Allocator, decltype(auto) Deleter, typename Container>
template <typename... Args>
utility::SmartPointer<T, Allocator, Deleter, Container>::SmartPointer(Args&&... args) {
    Reset(std::forward<Args>(args)...);
}

template <typename T, decltype(auto) Allocator, decltype(auto) Deleter, typename Container>
utility::SmartPointer<T, Allocator, Deleter, Container>::~SmartPointer() {
    Reset();
}

template <typename T, decltype(auto) Allocator, decltype(auto) Deleter, typename Container>
template <typename... Args>
void utility::SmartPointer<T, Allocator, Deleter, Container>::Reset(Args&&... args) {
    Reset();

    T* rawPointer = std::invoke(Allocator, std::forward<Args>(args)...);
    assert(rawPointer && "Failed to construct smart pointer");
    mContainer = Container(rawPointer, Deleter);
}

template <typename T, decltype(auto) Allocator, decltype(auto) Deleter, typename Container>
void utility::SmartPointer<T, Allocator, Deleter, Container>::Reset() noexcept {
    return mContainer.reset();
}

template <typename T, decltype(auto) Allocator, decltype(auto) Deleter, typename Container>
T* utility::SmartPointer<T, Allocator, Deleter, Container>::Get() const noexcept {
    return mContainer.get();
}

template <typename T, decltype(auto) Allocator, decltype(auto) Deleter, typename Container>
T& utility::SmartPointer<T, Allocator, Deleter, Container>::operator*() const noexcept {
    return mContainer.operator*();
}

template <typename T, decltype(auto) Allocator, decltype(auto) Deleter, typename Container>
T* utility::SmartPointer<T, Allocator, Deleter, Container>::operator->() const noexcept {
    return mContainer.operator->();
}

template <typename T, decltype(auto) Allocator, decltype(auto) Deleter, typename Container>
utility::SmartPointer<T, Allocator, Deleter, Container>::operator bool() const noexcept {
    return mContainer.operator bool();
}


#endif