#include <coordinators.hpp>


auto main(void) -> std::int32_t {
    MasterCoordinator coordinator;
    coordinator.Start();

    return 0;
}