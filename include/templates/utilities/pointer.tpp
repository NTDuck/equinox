#ifndef DEPENDENCIES_POINTER_TPP
#define DEPENDENCIES_POINTER_TPP

#include <functional>
#include <type_traits>

#include <utilities.hpp>


template <typename T, decltype(auto) Allocator, decltype(auto) Deleter, typename Container>
template <typename... Args>
utility::Pointer<T, Allocator, Deleter, Container>::Pointer(Args&&... args) {
    Emplace(std::forward<Args>(args)...);
}

template <typename T, decltype(auto) Allocator, decltype(auto) Deleter, typename Container>
template <typename... Args>
utility::Pointer<T, Allocator, Deleter, Container>& utility::Pointer<T, Allocator, Deleter, Container>::Emplace(Args&&... args) {
    T* rawPointer = std::invoke(CallableWrapper<Allocator>{}, std::forward<Args>(args)...);
    assert(rawPointer && "Failed to construct dependency");
    mContainer = Container(rawPointer, CallableWrapper<Deleter>{});
    
    return *this;
}

template <typename T, decltype(auto) Allocator, decltype(auto) Deleter, typename Container>
utility::Pointer<T, Allocator, Deleter, Container>::~Pointer() {
    Reset();
}

template <typename T, decltype(auto) Allocator, decltype(auto) Deleter, typename Container>
T* utility::Pointer<T, Allocator, Deleter, Container>::Get() const noexcept {
    return mContainer.get();
}

template <typename T, decltype(auto) Allocator, decltype(auto) Deleter, typename Container>
void utility::Pointer<T, Allocator, Deleter, Container>::Reset() {
    return mContainer.reset();
}

template <typename T, decltype(auto) Allocator, decltype(auto) Deleter, typename Container>
T& utility::Pointer<T, Allocator, Deleter, Container>::operator*() const noexcept {
    return mContainer.operator*();
}

template <typename T, decltype(auto) Allocator, decltype(auto) Deleter, typename Container>
T* utility::Pointer<T, Allocator, Deleter, Container>::operator->() const noexcept {
    return mContainer.operator->();
}

template <typename T, decltype(auto) Allocator, decltype(auto) Deleter, typename Container>
utility::Pointer<T, Allocator, Deleter, Container>::operator bool() const noexcept {
    return mContainer.operator bool();
}


#endif