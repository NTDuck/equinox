#ifndef UTILITIES_FORMATTERS_CPP
#define UTILITIES_FORMATTERS_CPP

#include <ostream>
#include <utilities.hpp>


std::ostream& operator<<(std::ostream& os, Point const& obj) {
    return os << "(" << obj.x << ", " << obj.y << ")";
}

std::ostream& operator<<(std::ostream& os, Rect const& obj) {
    return os << "(" << obj.x << ", " << obj.y << ", " << obj.w << ", " << obj.h << ")";
}

std::ostream& operator<<(std::ostream& os, Color const& obj) {
    return os << "(" << static_cast<std::uint32_t>(obj.r) << ", " << static_cast<std::uint32_t>(obj.g) << ", " << static_cast<std::uint32_t>(obj.b) << ", " << static_cast<std::uint32_t>(obj.a) << ")";
}


#endif