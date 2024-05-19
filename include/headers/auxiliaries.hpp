#ifndef AUXILIARIES_HPP
#define AUXILIARIES_HPP

#include <cassert>
#include <iostream>

#include <ecs/ecs.hpp>


typedef struct SDL_Point {
    int x, y;
} SDL_Point;

using Point = SDL_Point;

namespace config {
    static constexpr Point kMapLowerBound = { 0, 0 };
    static constexpr Point kMapHigherBound = { 128, 128 };
}

namespace utility {
    /**
     * @brief Enables `constexpr` string concatenation.
     * @see https://stackoverflow.com/questions/38955940/how-to-concatenate-static-strings-at-compile-time
    */
    template <std::string_view const&... Strs>
    struct strcat {
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

        static constexpr inline auto mArr = impl();   // Static storage
        
    public:
        static constexpr std::string_view value { mArr.data(), mArr.size() - 1 };
    };
}

namespace global {
    extern ECS::Coordinator ECSCoordinator;
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
        static constexpr inline auto value = utility::strcat<mPrefix, kDelimiter, Strs...>::value;
    };
}


#endif