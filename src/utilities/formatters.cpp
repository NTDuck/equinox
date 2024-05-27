#ifndef UTILITIES_FORMATTERS_CPP
#define UTILITIES_FORMATTERS_CPP

#include <iostream>
#include <utilities.hpp>


std::ostream& operator<<(std::ostream& os, Point const& obj) {
    return os << "(" << obj.x << ", " << obj.y << ")";
}

std::ostream& operator<<(std::ostream& os, Rect const& obj) {
    return os << "(" << obj.x << ", " << obj.y << ", " << obj.w << ", " << obj.h << ")";
}


#endif