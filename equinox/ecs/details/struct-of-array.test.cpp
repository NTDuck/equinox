// import <cstdint>;
// import <cassert>;

// import <limits>;

// import <string_view>;

#include <cstdint>
#include <cassert>
#include <limits>
#include <string_view>

import ecs.details;

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_CASE("Struct of Array") {
    constexpr std::uint32_t maxObjectID = 1024;
    using Object = std::tuple<std::string_view, std::int32_t>;

    ecs::details::StructOfArray<Object, maxObjectID> structOfArray;

    structOfArray.insert(0, { "Hello world", 4 });
    structOfArray.insert(1, { "", 0});
    structOfArray.insert(maxObjectID - 1, { "The unseen blade is the deadliest!", std::numeric_limits<std::int32_t>::min() });

    CHECK(structOfArray.get<1>(0) == 4);
    CHECK(structOfArray.get<0>(1).empty());
    CHECK(structOfArray.get<1>(maxObjectID - 1) < -1);
}