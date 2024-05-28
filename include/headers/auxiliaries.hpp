#ifndef AUXILIARIES_HPP
#define AUXILIARIES_HPP

#include <cstdint>
#include <cstdlib>

#include <array>
#include <string_view>

#include <SDL.h>
#include <SDL_image.h>


/* Enumerations */

/**
 * @see https://qualified.one/blog/game_dev/six-methods-of-fps-calculation/
*/
enum class FPSMonitoringMethod {
    kFixedInterval,
    kFixedInterval_,
    kFixedFrameTime,
    kRealTime,
    kCommonAverage,
    kExactSampling,
    kAverageSampling,
};


/* Aliases */

using Point = SDL_Point;
using FPoint = SDL_FPoint;

using Rect = SDL_Rect;
using Color = SDL_Color;

using Ticks = std::uint64_t;
using FTicks = double;

using SpriteSheetID = std::uint16_t;
using SpriteID = std::uint16_t;


namespace config {
    #define USE_RAW_TYPE_NAME 1

    static constexpr std::uint16_t kMaxTextureID = 4;
    static constexpr SpriteSheetID kMaxSpriteSheetID = 1;

    static constexpr double kFPS = 60;

    static constexpr std::array<std::pair<std::string_view, Point>, kMaxSpriteSheetID> kSpriteSheetData {
        {{ "assets/graphics/characters/hana-caraka.png", { 40, 40 }, }},
    };

    namespace sdl {
        static constexpr std::uint32_t kInitFlags = SDL_INIT_EVENTS | SDL_INIT_TIMER;
        static constexpr std::uint32_t kInitFlagsImage = IMG_INIT_PNG;

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
            // { SDL_HINT_WINDOWS_NO_CLOSE_ON_ALT_F4, "1" },
        }};
    }

    namespace fps {
        static constexpr FPSMonitoringMethod kFPSMonitoringMethod = FPSMonitoringMethod::kRealTime;

        static constexpr Ticks kTicksPerInterval = 1000;
        static constexpr Ticks kFramesPerInterval = 16;
        static constexpr Ticks kMaxExactSamplingSamples = 128;
        static constexpr Ticks kMaxAverageSamplingSamples = 128;
    }
}


#endif