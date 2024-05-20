#ifndef APPLICATION_TIMER_CPP
#define APPLICATION_TIMER_CPP

#include <application.hpp>
#include <SDL_timer.h>


void Application::Timer::Start() {
    mState = State::kActive;

    mStartTicks = SDL_GetTicks64();
    mPausedTicks = 0;
}

void Application::Timer::Stop() {
    mState = State::kIdle;

    mStartTicks = 0;
    mPausedTicks = 0;
}

void Application::Timer::Pause() {
    if (mState != State::kActive) return;
    mState = State::kSuspended;

    mPausedTicks = SDL_GetTicks64() - mStartTicks;
    mStartTicks = 0;
}

void Application::Timer::Unpause() {
    if (mState != State::kSuspended) return;
    mState = State::kActive;

    mStartTicks = SDL_GetTicks64() - mPausedTicks;
    mPausedTicks = 0;
}

std::uint64_t Application::Timer::GetTicks() const noexcept {
    switch (mState) {      
        case State::kActive:
            return SDL_GetTicks64() - mStartTicks;

        case State::kSuspended:
            return mPausedTicks;

        default: return 0;
    }
}

Application::Timer::State Application::Timer::GetState() const noexcept {
    return mState;
}

void Application::FPSRegulator::SetFPS(double FPS) {
    mTicksPerFrame = 1000 / FPS;
}

/**
 * @note At least one call to `SetFPS()` must be made prior.
*/
double Application::FPSRegulator::GetFPS() const noexcept {
    return 1000 / mTicksPerFrame;
}

void Application::FPSRegulator::PreIntegrate() {
    Start();
}

void Application::FPSRegulator::PostIntegrate() const {
    auto ticksOfCurrentFrame = GetTicks();
    if (ticksOfCurrentFrame < mTicksPerFrame) SDL_Delay(mTicksPerFrame - ticksOfCurrentFrame);
}

void Application::FPSCalculator::Start() {
    Timer::Start();
}

/**
 * @note Called once per frame.
*/
double Application::FPSCalculator::GetFPS() noexcept {
    return mFrames++ / (GetTicks() / 1000.f);
}

#endif