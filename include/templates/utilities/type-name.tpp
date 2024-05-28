#ifndef UTILITIES_TYPE_NAME_TPP
#define UTILITIES_TYPE_NAME_TPP

#include <typeinfo>
#include <string>
#include <string_view>

#include <utilities.hpp>
#include <auxiliaries.hpp>


#if __has_include(<cxxabi.h>)
    #include <cxxabi.h>
    #define HAS_CXXABI
#endif

template <typename T>
std::string_view utility::GetRawTypeName() {
    return typeid(T).name();
}

template <typename T>
std::string_view utility::GetTypeName() {
#if USE_RAW_TYPE_NAME || !defined(HAS_CXXABI)
    return GetRawTypeName<T>();
#else 
    auto impl = []() -> std::string {
        std::int32_t demangleStatus = -1;
        auto rawTypeName = GetRawTypeName<T>();

        std::unique_ptr<char, void(*)(void*)> demangledTypeName {
            abi::__cxa_demangle(rawTypeName.data(), nullptr, nullptr, &demangleStatus),
            std::free,
        };
        
        return demangleStatus == 0 ? demangledTypeName.get() : rawTypeName.data();
    };

    return impl().operator std::basic_string_view<char, std::char_traits<char>>();
#endif
}


#endif