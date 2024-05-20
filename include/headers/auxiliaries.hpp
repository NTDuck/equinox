#ifndef AUXILIARIES_HPP
#define AUXILIARIES_HPP

#include <cassert>
#include <iostream>

#include <SDL.h>
#include <ecs/ecs.hpp>


/* Aliases */
using Point = SDL_Point;
using Rect = SDL_Rect;
using Color = SDL_Color;

/* Operator Overloading */
constexpr inline Point operator-(Point const& obj) { return Point({ -obj.x, -obj.y }); }

constexpr inline Point operator+(Point const& lhs, Point const& rhs) { return Point({ lhs.x + rhs.x, lhs.y + rhs.y }); }
constexpr inline Point operator+(Point const& lhs, int rhs) { return Point({ lhs.x + rhs, lhs.y + rhs }); }
constexpr inline Point operator+(int lhs, Point const& rhs) { return rhs + lhs; }
constexpr inline Point operator-(Point const& lhs, Point const& rhs) { return lhs + -rhs; }
constexpr inline Point operator-(Point const& lhs, int rhs) { return lhs + -rhs; }
constexpr inline Point operator*(Point const& lhs, Point const& rhs) { return Point({ lhs.x * rhs.x, lhs.y * rhs.y }); }   // Dot product
constexpr inline Point operator*(Point const& lhs, int rhs) { return Point({ lhs.x * rhs, lhs.y * rhs }); }
constexpr inline Point operator/(Point const& lhs, Point const& rhs) { return Point({ lhs.x / rhs.x, lhs.y / rhs.y }); }
constexpr inline Point operator/(Point const& lhs, int rhs) { return Point({ lhs.x / rhs, lhs.y / rhs }); }

constexpr inline Point& operator++(Point& obj) { obj = obj + 1; return obj; }
constexpr inline Point operator++(Point& obj, int) { auto old = obj; ++obj; return old; }
constexpr inline Point& operator--(Point& obj) { obj = obj - 1; return obj; }
constexpr inline Point operator--(Point& obj, int) { auto old = obj; --obj; return old; }
constexpr inline Point& operator+=(Point& lhs, Point const& rhs) { lhs = lhs + rhs; return lhs; }
constexpr inline Point& operator+=(Point& lhs, int rhs) { lhs = lhs + rhs; return lhs; }
constexpr inline Point& operator-=(Point& lhs, Point const& rhs) { lhs = lhs - rhs; return lhs; }
constexpr inline Point& operator-=(Point& lhs, int rhs) { lhs = lhs - rhs; return lhs; }
constexpr inline Point& operator*=(Point& lhs, Point const& rhs) { lhs = lhs * rhs; return lhs; }
constexpr inline Point& operator*=(Point& lhs, int rhs) { lhs = lhs * rhs; return lhs; }
constexpr inline Point& operator/=(Point& lhs, Point const& rhs) { lhs = lhs / rhs; return lhs; }
constexpr inline Point& operator/=(Point& lhs, int rhs) { lhs = lhs / rhs; return lhs; }

constexpr inline bool operator==(Point const& lhs, Point const& rhs) noexcept { return lhs.x == rhs.x && lhs.y == rhs.y; }
constexpr inline bool operator!=(Point const& lhs, Point const& rhs) noexcept { return !(lhs == rhs); }
constexpr inline bool operator<(Point const& lhs, Point const& rhs) noexcept { return lhs.y == rhs.y ? lhs.y < rhs.y : lhs.x < rhs.x; }   // Simulates rendering order
constexpr inline bool operator>(Point const& lhs, Point const& rhs) noexcept { return rhs < lhs; }
constexpr inline bool operator<=(Point const& lhs, Point const& rhs) noexcept { return !(lhs > rhs); }
constexpr inline bool operator>=(Point const& lhs, Point const& rhs) noexcept { return !(lhs < rhs); }

namespace config {
    static constexpr Point kMapLowerBound = { 0, 0 };
    static constexpr Point kMapHigherBound = { 128, 128 };

    namespace sdl {
        static constexpr std::uint32_t kInitFlags = SDL_INIT_EVENTS | SDL_INIT_TIMER;
        // static constexpr std::uint32_t kInitFlagsImage = IMG_INIT_PNG;

        namespace window {
            static constexpr std::uint32_t kInitFlags = SDL_WINDOW_SHOWN;
            static constexpr std::string_view kTitle = "equinox";
            static constexpr Rect kSize = { SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720 };
        }

        namespace renderer {
            static constexpr std::uint32_t kInitFlags = SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED;
            static constexpr std::int32_t kDriverIndex = -1;
        }

        static constexpr std::array<std::pair<std::string_view, std::string_view>, 3> kHints {{
            { SDL_HINT_RENDER_SCALE_QUALITY, "0" },
            { SDL_HINT_RENDER_VSYNC, "1" },
            { SDL_HINT_WINDOWS_NO_CLOSE_ON_ALT_F4, "1" },
        }};
    }
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