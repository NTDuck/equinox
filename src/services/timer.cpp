#ifndef SERVICES_TIMER_CPP
#define SERVICES_TIMER_CPP

#include <SDL_timer.h>
#include <services.hpp>


Timer::Timer() {
    Stop();
}

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


#endif