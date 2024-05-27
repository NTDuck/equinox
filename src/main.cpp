#include <application.hpp>
#include <ecs.hpp>


extern "C" auto main(int, char**) -> std::int32_t {
    auto& application = Application::Get();
    application.Start();

    return 0;
}