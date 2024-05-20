#ifndef APPLICATION_TIMERS_TPP
#define APPLICATION_TIMERS_TPP

#include <queue>
#include <application.hpp>


template <FPSMonitoringMethod Method>
void Application::FPSMonitor<Method>::Start() {
    Timer::Start();
}

template <FPSMonitoringMethod Method>
template <typename... Args>
decltype(auto) Application::FPSMonitor<Method>::GetFPS(Args&&... args) const {
    return std::invoke(&GetFPS<Method>, this, std::forward<Args>(args)...);
}

/**
 * @note Returns `0` in the first frame i.e. the first `config::kFPS` calls.
*/
template <FPSMonitoringMethod Method>
template <FPSMonitoringMethod M>
std::enable_if_t<M == FPSMonitoringMethod::kFixedInterval, std::uint32_t> Application::FPSMonitor<Method>::GetFPS() const {
    static std::uint32_t frameCount = 0;
    static auto FPS = frameCount;
    static auto ticksOfPreviousFrame = GetTicks();

    ++frameCount;
    auto ticksOfCurrentFrame = GetTicks();

    if (ticksOfCurrentFrame - ticksOfPreviousFrame > config::fps::kTicksPerInterval) {
        FPS = frameCount;
        frameCount = 0;
        ticksOfPreviousFrame = ticksOfCurrentFrame;
    }

    return FPS;
}

/**
 * @note Different implementation but produces similar results to `FPSMonitoringMethod::kFixedInterval`.
*/
template <FPSMonitoringMethod Method>
template <FPSMonitoringMethod M>
std::enable_if_t<M == FPSMonitoringMethod::kFixedInterval_, std::uint32_t> Application::FPSMonitor<Method>::GetFPS(std::uint32_t dt) const {
    static std::uint32_t frameCount = 0;
    static auto FPS = frameCount;
    static std::int32_t ticksToNextInterval = config::fps::kTicksPerInterval;

    ++frameCount;
    ticksToNextInterval -= dt;

    if (ticksToNextInterval < 0) {
        FPS = frameCount;
        frameCount = 0;
        ticksToNextInterval = config::fps::kTicksPerInterval;
    }

    return FPS;
}

/**
 * @note Returns `0` the first `config::fps::kFramesPerInterval` calls.
*/
template <FPSMonitoringMethod Method>
template <FPSMonitoringMethod M>
std::enable_if_t<M == FPSMonitoringMethod::kFixedFrameTime, double> Application::FPSMonitor<Method>::GetFPS() const {
    static std::uint32_t frameCount = 0;
    static double FPS = 0;
    static auto ticksOfPreviousInterval = GetTicks();

    ++frameCount;

    if (frameCount > config::fps::kFramesPerInterval) {
        auto ticksOfCurrentInterval = GetTicks();
        FPS = static_cast<double>(1000) * frameCount / (ticksOfCurrentInterval - ticksOfPreviousInterval);
        frameCount = 0;
        ticksOfPreviousInterval = ticksOfCurrentInterval;
    }

    return FPS;
}

/**
 * @note Produces surprisingly good results. Except for the first call, which is inevitable.
*/
template <FPSMonitoringMethod Method>
template <FPSMonitoringMethod M>
std::enable_if_t<M == FPSMonitoringMethod::kRealTime, double> Application::FPSMonitor<Method>::GetFPS(std::uint32_t dt) const {
    return static_cast<double>(1000) / dt;
}

/**
 * @note Produces acceptable results. First call returns `inf`, subsequent calls promptly bring results to normal.
*/
template <FPSMonitoringMethod Method>
template <FPSMonitoringMethod M>
std::enable_if_t<M == FPSMonitoringMethod::kCommonAverage, double> Application::FPSMonitor<Method>::GetFPS() const {
    static const auto ticksOfFirstFrame = GetTicks();
    static std::uint32_t frameCount = 0;

    ++frameCount;
    auto dt = GetTicks() - ticksOfFirstFrame;
    return static_cast<double>(1000) * frameCount / dt;
}

/**
 * @note Produces slightly higher results. Consumes extra linear memory.
*/
template <FPSMonitoringMethod Method>
template <FPSMonitoringMethod M>
std::enable_if_t<M == FPSMonitoringMethod::kExactSampling, double> Application::FPSMonitor<Method>::GetFPS(std::uint32_t dt) const {
    static std::queue<std::uint32_t> sampledTicks;
    static std::uint32_t sumOfSampledTicks = 0;

    if (sampledTicks.size() > config::fps::kMaxExactSamplingSamples) {
        sumOfSampledTicks -= sampledTicks.front();
        sampledTicks.pop();
    }

    sumOfSampledTicks += dt;
    sampledTicks.push(dt);

    return static_cast<double>(1000) * sampledTicks.size() / sumOfSampledTicks;
}

/**
 * @note Produces noticeably higher results.
*/
template <FPSMonitoringMethod Method>
template <FPSMonitoringMethod M>
std::enable_if_t<M == FPSMonitoringMethod::kAverageSampling, double> Application::FPSMonitor<Method>::GetFPS(std::uint32_t dt) const {
    static double averageTicks = dt;
    static constexpr double contributionOfCurrentTicks = static_cast<double>(1) / config::fps::kMaxAverageSamplingSamples;

    averageTicks = averageTicks * (1 - contributionOfCurrentTicks) + dt * contributionOfCurrentTicks;
    return static_cast<double>(1000) / averageTicks;  
}


#endif