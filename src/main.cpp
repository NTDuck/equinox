#include <iostream>
#include <application.hpp>


extern "C" auto main(int, char**) -> std::int32_t {
    try {
        Application app;
        app.Start();
    } catch (std::exception& exception) {
        std::cout << exception.what() << std::endl;
        return 1;
    }

    return 0;
}