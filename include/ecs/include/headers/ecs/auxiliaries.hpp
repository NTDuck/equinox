#ifndef ECS_AUXILIARIES_HPP
#define ECS_AUXILIARIES_HPP

#include <cstddef>
#include <cstdint>

#include <bitset>


namespace ECS {
    using EntityID = std::uint32_t;
    using ComponentID = std::uint8_t;
    
    namespace config {
        static constexpr EntityID kMaxEntityID = 1e3;
        static constexpr ComponentID kMaxComponentID = 32;
    }

    using Signature = std::bitset<config::kMaxComponentID>;
}


#endif