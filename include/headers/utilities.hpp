#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <memory>
#include <string_view>

#include <auxiliaries.hpp>


namespace utility {
    /**
     * @brief Enables `constexpr` string concatenation.
     * @see https://stackoverflow.com/questions/38955940/how-to-concatenate-static-strings-at-compile-time
    */
    template <std::string_view const&... Strs>
    struct Concatenate {
    private:
        // Concatenate all strings into an `std::array<char>`
        static constexpr inline auto impl() noexcept {
            constexpr std::size_t size = (Strs.size() + ...);
            std::array<char, size + 1> arr{};
            auto append = [pos = 0, &arr](auto const& s) mutable { for (auto c : s) arr[pos++] = c; };
            (append(Strs), ...);
            arr[size] = 0;
            return arr;
        }

        static constexpr inline auto mArray = impl();   // Static storage
        
    public:
        static constexpr std::string_view value { mArray.data(), mArray.size() - 1 };
    };

    template <typename T>
    class Singleton {
    public:
        template <typename... Args>
        static inline T& Get(Args&&... args) {
            static T instance(std::forward<Args>(args)...);
            return instance;
        }

        Singleton(Singleton const&) = delete;
        Singleton& operator=(Singleton const&) = delete;
        Singleton(Singleton&&) = delete;
        Singleton& operator=(Singleton&&) = delete;

    protected:
        constexpr inline explicit Singleton() = default;
        inline virtual ~Singleton() = default;
    };

    template <typename T, typename... Args>
    static inline T* DefaultAllocate(Args&&... args) {
        return new T(std::forward<Args>(args)...);
    }

    template <typename T>
    static inline void DefaultDelete(T* ptr) {
        delete ptr;
    }

    template <typename T, decltype(auto) Allocator, decltype(auto) Deleter, typename Container = std::shared_ptr<T>>
    class Pointer {
        template <decltype(auto) Callable>
        struct CallableWrapper {
            template <typename... Args>
            constexpr inline decltype(auto) operator()(Args&&... args) const {
                return std::invoke(Callable, std::forward<Args>(args)...);
            }
        };

    public:
        constexpr inline Pointer() noexcept {}
        constexpr inline Pointer(std::nullptr_t) noexcept {}
        ~Pointer();

        template <typename... Args>
        Pointer(Args&&...);

        template <typename... Args>
        Pointer<T, Allocator, Deleter, Container>& Emplace(Args&&...);

        T* Get() const noexcept;
        void Reset();

        T& operator*() const noexcept;
        T* operator->() const noexcept;
        explicit operator bool() const noexcept;

    private:
        Container mContainer;
    };
}

namespace logger {
    enum class Level {
        kEmergency,
        kAlert,
        kCritical,
        kError,
        kWarning,
        kNotice,
        kInformational,
        kDebug,
    };

    static constexpr std::array<std::string_view, static_cast<std::size_t>(Level::kDebug) - static_cast<std::size_t>(Level::kEmergency) + 1> kPrefixes {
        "emerg", "alert", "crit", "error", "warn", "notice", "info", "debug",
    };
    static constexpr std::string_view kDelimiter = " : ";

    template <Level L, std::string_view const&... Strs>
    struct formatter {
    private:
        static constexpr auto mPrefix = kPrefixes[static_cast<std::size_t>(L)];

    public:
        static constexpr inline auto value = utility::Concatenate<mPrefix, kDelimiter, Strs...>::value;
    };
}


#include <utilities/pointer.tpp>

#endif