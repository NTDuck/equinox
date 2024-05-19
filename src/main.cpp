#include <application.hpp>


extern "C" auto main(int, char**) -> std::int32_t {
    Application app;
    app.Start();

    return 0;
}