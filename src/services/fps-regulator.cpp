#ifndef SERVICES_FPS_REGULATOR_CPP
#define SERVICES_FPS_REGULATOR_CPP

#include <services.hpp>


void FPSRegulator::SetFPS(double FPS) {
    mTicksPerFrame = TicksPerSecond() / FPS;
}

/**
 * @note At least one call to `SetFPS()` must be made prior.
*/
FTicks FPSRegulator::GetFPS() const noexcept {
    return TicksPerSecond() / mTicksPerFrame;
}

void FPSRegulator::PreIntegrate() {
    Start();
}

void FPSRegulator::PostIntegrate() const {
    auto ticksOfCurrentFrame = GetTicks();
    if (ticksOfCurrentFrame < mTicksPerFrame) SDL_Delay(mTicksPerFrame - ticksOfCurrentFrame);
}


#endif