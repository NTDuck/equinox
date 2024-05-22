#ifndef DEPENDENCIES_FORMATTERS_CPP
#define DEPENDENCIES_FORMATTERS_CPP

#include <iostream>
#include <utilities.hpp>


std::ostream& utility::operator<<(std::ostream& os, Point const& obj) {
    return os << "(" << obj.x << ", " << obj.y << ")";
}

std::ostream& utility::operator<<(std::ostream& os, Rect const& obj) {
    return os << "(" << obj.x << ", " << obj.y << ", " << obj.w << ", " << obj.h << ")";
}


#endif