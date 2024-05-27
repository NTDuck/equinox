#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <iosfwd>   // https://stackoverflow.com/questions/373142/what-techniques-can-be-used-to-speed-up-c-compilation-times
#include <memory>
#include <functional>
#include <type_traits>
#include <string_view>

#include <auxiliaries.hpp>


constexpr inline Point operator-(Point const& obj) { return Point({ -obj.x, -obj.y }); }

constexpr inline Point operator+(Point const& lhs, Point const& rhs) { return Point({ lhs.x + rhs.x, lhs.y + rhs.y }); }
constexpr inline Point operator+(Point const& lhs, std::int32_t rhs) { return Point({ lhs.x + rhs, lhs.y + rhs }); }
constexpr inline Point operator+(int lhs, Point const& rhs) { return rhs + lhs; }
constexpr inline Point operator-(Point const& lhs, Point const& rhs) { return lhs + -rhs; }
constexpr inline Point operator-(Point const& lhs, std::int32_t rhs) { return lhs + -rhs; }
constexpr inline Point operator*(Point const& lhs, Point const& rhs) { return Point({ lhs.x * rhs.x, lhs.y * rhs.y }); }   // Dot product
constexpr inline Point operator*(Point const& lhs, std::int32_t rhs) { return Point({ lhs.x * rhs, lhs.y * rhs }); }
constexpr inline Point operator/(Point const& lhs, Point const& rhs) { return Point({ lhs.x / rhs.x, lhs.y / rhs.y }); }
constexpr inline Point operator/(Point const& lhs, std::int32_t rhs) { return Point({ lhs.x / rhs, lhs.y / rhs }); }

constexpr inline Point& operator++(Point& obj) { obj = obj + 1; return obj; }
constexpr inline Point operator++(Point& obj, std::int32_t) { auto old = obj; ++obj; return old; }
constexpr inline Point& operator--(Point& obj) { obj = obj - 1; return obj; }
constexpr inline Point operator--(Point& obj, std::int32_t) { auto old = obj; --obj; return old; }
constexpr inline Point& operator+=(Point& lhs, Point const& rhs) { lhs = lhs + rhs; return lhs; }
constexpr inline Point& operator+=(Point& lhs, std::int32_t rhs) { lhs = lhs + rhs; return lhs; }
constexpr inline Point& operator-=(Point& lhs, Point const& rhs) { lhs = lhs - rhs; return lhs; }
constexpr inline Point& operator-=(Point& lhs, std::int32_t rhs) { lhs = lhs - rhs; return lhs; }
constexpr inline Point& operator*=(Point& lhs, Point const& rhs) { lhs = lhs * rhs; return lhs; }
constexpr inline Point& operator*=(Point& lhs, std::int32_t rhs) { lhs = lhs * rhs; return lhs; }
constexpr inline Point& operator/=(Point& lhs, Point const& rhs) { lhs = lhs / rhs; return lhs; }
constexpr inline Point& operator/=(Point& lhs, std::int32_t rhs) { lhs = lhs / rhs; return lhs; }

constexpr inline bool operator==(Point const& lhs, Point const& rhs) noexcept { return lhs.x == rhs.x && lhs.y == rhs.y; }
constexpr inline bool operator!=(Point const& lhs, Point const& rhs) noexcept { return !(lhs == rhs); }
constexpr inline bool operator<(Point const& lhs, Point const& rhs) noexcept { return lhs.y == rhs.y ? lhs.y < rhs.y : lhs.x < rhs.x; }   // Simulates rendering order
constexpr inline bool operator>(Point const& lhs, Point const& rhs) noexcept { return rhs < lhs; }
constexpr inline bool operator<=(Point const& lhs, Point const& rhs) noexcept { return !(lhs > rhs); }
constexpr inline bool operator>=(Point const& lhs, Point const& rhs) noexcept { return !(lhs < rhs); }

constexpr inline bool operator==(Point const& lhs, std::int32_t rhs) noexcept { return lhs.x == rhs && lhs.y == rhs; }
constexpr inline bool operator!=(Point const& lhs, std::int32_t rhs) noexcept { return !(lhs == rhs); }
constexpr inline bool operator<(Point const& lhs, std::int32_t rhs) noexcept { return lhs.x < rhs && lhs.y < rhs; }   // Simulates rendering order
constexpr inline bool operator>(Point const& lhs, std::int32_t rhs) noexcept { return lhs.x > rhs && lhs.y > rhs; }
constexpr inline bool operator<=(Point const& lhs, std::int32_t rhs) noexcept { return !(lhs > rhs); }
constexpr inline bool operator>=(Point const& lhs, std::int32_t rhs) noexcept { return !(lhs < rhs); }

std::ostream& operator<<(std::ostream&, Point const&);
std::ostream& operator<<(std::ostream&, Rect const&);

namespace utility {
    /**
     * @brief Enables string concatenation at compile-time.
     * @see https://stackoverflow.com/questions/38955940/how-to-concatenate-static-strings-at-compile-time
    */
    template <typename CharType, std::basic_string_view<CharType> const&... Strs>
    struct BasicStringViewConcatenate {
    private:
        // Concatenate all strings into an `std::array<char>`
        static constexpr inline auto MakeBuffer() noexcept {
            constexpr std::size_t bufferSize = (Strs.size() + ...);
            std::array<CharType, bufferSize + 1> buffer{};

            auto appendToBuffer = [pos = 0, &buffer](auto const& s) mutable { for (auto c : s) buffer[pos++] = c; };
            
            (appendToBuffer(Strs), ...);
            buffer[bufferSize] = 0;
            
            return buffer;
        }

        static constexpr inline auto mBuffer = MakeBuffer();   // Static storage
        
    public:
        static constexpr std::basic_string_view<CharType> value { mBuffer.data(), mBuffer.size() - 1 };
    };

    template <std::string_view const&... Strs>
    using StringViewConcatenate = BasicStringViewConcatenate<char, Strs...>;

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

    template <typename T, decltype(auto) Allocator, decltype(auto) Deleter, typename Container = std::shared_ptr<T>>
    class SmartPointer {
    public:
        constexpr inline SmartPointer() noexcept {}
        constexpr inline SmartPointer(std::nullptr_t) noexcept {}
        ~SmartPointer();

        template <typename... Args>
        SmartPointer(Args&&...);

        template <typename... Args>
        void Reset(Args&&...);
        void Reset() noexcept;

        T* Get() const noexcept;
        T& operator*() const noexcept;
        T* operator->() const noexcept;

        explicit operator bool() const noexcept;

    private:
        Container mContainer;
    };

    /**
     * @see https://medium.com/@savas/template-metaprogramming-compile-time-loops-over-class-methods-a243dc346122
    */
    class For;

    /**
     * @brief Optimal parameter passing technique of type `T`. Selecting `OptimalParamType<T>` over `T` always result in marginally increased performance.
     * @note `OptimalParamType<T>` is implicitly convertible to `T`.
    */

    template <typename T>
    std::string_view GetRawTypeName();

    /**
     * @note Requires header `<cxxabi.h>`, which might not be present on all compilers.
    */
    template <typename T>
    std::string_view GetTypeName();
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
        static constexpr inline auto value = utility::StringViewConcatenate<mPrefix, kDelimiter, Strs...>::value;
    };
}


#include <utilities/smart-pointer.tpp>
#include <utilities/type-name.tpp>

#endif