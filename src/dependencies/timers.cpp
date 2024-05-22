#ifndef DEPENDENCIES_TIMERS_CPP
#define DEPENDENCIES_TIMERS_CPP

#include <SDL_timer.h>
#include <dependencies.hpp>


void Timer::Start() {
    mState = State::kActive;

    mStartTicks = SDL_GetTicks64();
    mPausedTicks = 0;
}

void Timer::Stop() {
    mState = State::kIdle;

    mStartTicks = 0;
    mPausedTicks = 0;
}

void Timer::Pause() {
    if (mState != State::kActive) return;
    mState = State::kSuspended;

    mPausedTicks = SDL_GetTicks64() - mStartTicks;
    mStartTicks = 0;
}

void Timer::Unpause() {
    if (mState != State::kSuspended) return;
    mState = State::kActive;

    mStartTicks = SDL_GetTicks64() - mPausedTicks;
    mPausedTicks = 0;
}

std::uint64_t Timer::GetTicks() const noexcept {
    switch (mState) {      
        case State::kActive:
            return SDL_GetTicks64() - mStartTicks;

        case State::kSuspended:
            return mPausedTicks;

        default: return 0;
    }
}

std::uint64_t Timer::GetDeltaTime() const noexcept {
    static Ticks ticksOfPreviousCall = 0;

    auto ticksOfCurrentCall = GetTicks();
    auto dt = ticksOfCurrentCall - ticksOfPreviousCall;
    ticksOfPreviousCall = ticksOfCurrentCall;

    return dt;
}

Timer::State Timer::GetState() const noexcept {
    return mState;
}

void FPSRegulator::SetFPS(double FPS) {
    mTicksPerFrame = 1000 / FPS;
}

/**
 * @note At least one call to `SetFPS()` must be made prior.
*/
FTicks FPSRegulator::GetFPS() const noexcept {
    return 1000 / mTicksPerFrame;
}

void FPSRegulator::PreIntegrate() {
    Start();
}

void FPSRegulator::PostIntegrate() const {
    auto ticksOfCurrentFrame = GetTicks();
    if (ticksOfCurrentFrame < mTicksPerFrame) SDL_Delay(mTicksPerFrame - ticksOfCurrentFrame);
}


#endif