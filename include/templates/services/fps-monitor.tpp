#ifndef SERVICES_FPS_MONITOR_TPP
#define SERVICES_FPS_MONITOR_TPP

#include <queue>
#include <services.hpp>


template <FPSMonitoringMethod Method>
void FPSMonitor<Method>::Start() {
    Timer::Start();
}

template <FPSMonitoringMethod Method>
template <typename... Args>
decltype(auto) FPSMonitor<Method>::GetFPS(Args&&... args) const {
    return std::invoke(&GetFPS<Method>, this, std::forward<Args>(args)...);
}

/**
 * @note Returns `0` in the first frame i.e. the first `config::kFPS` calls.
*/
template <FPSMonitoringMethod Method>
template <FPSMonitoringMethod M>
std::enable_if_t<M == FPSMonitoringMethod::kFixedInterval, Ticks> FPSMonitor<Method>::GetFPS() const {
    static std::size_t frameCount = 0;
    static Ticks frameRate = frameCount;
    static auto ticksOfPreviousFrame = GetTicks();

    ++frameCount;
    auto ticksOfCurrentFrame = GetTicks();

    if (ticksOfCurrentFrame - ticksOfPreviousFrame > config::fps::kTicksPerInterval) {
        frameRate = frameCount;
        frameCount = 0;
        ticksOfPreviousFrame = ticksOfCurrentFrame;
    }

    return frameRate;
}

/**
 * @note Different implementation but produces similar results to `FPSMonitoringMethod::kFixedInterval`.
*/
template <FPSMonitoringMethod Method>
template <FPSMonitoringMethod M>
std::enable_if_t<M == FPSMonitoringMethod::kFixedInterval_, Ticks> FPSMonitor<Method>::GetFPS(Ticks dt) const {
    static std::size_t frameCount = 0;
    static Ticks frameRate = frameCount;
    static auto ticksToNextInterval = config::fps::kTicksPerInterval;

    ++frameCount;
    ticksToNextInterval -= dt;

    if (ticksToNextInterval < 0) {
        frameRate = frameCount;
        frameCount = 0;
        ticksToNextInterval = config::fps::kTicksPerInterval;
    }

    return frameRate;
}

/**
 * @note Returns `0` the first `config::fps::kFramesPerInterval` calls.
*/
template <FPSMonitoringMethod Method>
template <FPSMonitoringMethod M>
std::enable_if_t<M == FPSMonitoringMethod::kFixedFrameTime, FTicks> FPSMonitor<Method>::GetFPS() const {
    static std::size_t frameCount = 0;
    static FTicks frameRate = 0;
    static auto ticksOfPreviousInterval = GetTicks();

    ++frameCount;

    if (frameCount > config::fps::kFramesPerInterval) {
        auto ticksOfCurrentInterval = GetTicks();
        frameRate = TicksPerSecond() * frameCount / (ticksOfCurrentInterval - ticksOfPreviousInterval);
        frameCount = 0;
        ticksOfPreviousInterval = ticksOfCurrentInterval;
    }

    return frameRate;
}

/**
 * @note Produces surprisingly good results. Except for the first call, which is inevitable.
*/
template <FPSMonitoringMethod Method>
template <FPSMonitoringMethod M>
std::enable_if_t<M == FPSMonitoringMethod::kRealTime, FTicks> FPSMonitor<Method>::GetFPS(Ticks dt) const {
    return TicksPerSecond() / dt;
}

/**
 * @note Produces acceptable results. First call returns `inf`, subsequent calls promptly bring results to normal.
*/
template <FPSMonitoringMethod Method>
template <FPSMonitoringMethod M>
std::enable_if_t<M == FPSMonitoringMethod::kCommonAverage, FTicks> FPSMonitor<Method>::GetFPS() const {
    static const auto ticksOfFirstFrame = GetTicks();
    static std::size_t frameCount = 0;

    ++frameCount;
    auto dt = GetTicks() - ticksOfFirstFrame;
    return TicksPerSecond() * frameCount / dt;
}

/**
 * @note Produces slightly higher results. Consumes extra linear memory.
*/
template <FPSMonitoringMethod Method>
template <FPSMonitoringMethod M>
std::enable_if_t<M == FPSMonitoringMethod::kExactSampling, FTicks> FPSMonitor<Method>::GetFPS(Ticks dt) const {
    static std::queue<Ticks> sampledTicks;
    static Ticks sumOfSampledTicks = 0;

    if (sampledTicks.size() > config::fps::kMaxExactSamplingSamples) {
        sumOfSampledTicks -= sampledTicks.front();
        sampledTicks.pop();
    }

    sumOfSampledTicks += dt;
    sampledTicks.push(dt);

    return TicksPerSecond() * sampledTicks.size() / sumOfSampledTicks;
}

/**
 * @note Produces noticeably higher results.
*/
template <FPSMonitoringMethod Method>
template <FPSMonitoringMethod M>
std::enable_if_t<M == FPSMonitoringMethod::kAverageSampling, FTicks> FPSMonitor<Method>::GetFPS(Ticks dt) const {
    static FTicks averageTicks = dt;
    static constexpr double contributionOfCurrentTicks = static_cast<double>(1) / config::fps::kMaxAverageSamplingSamples;

    averageTicks = averageTicks * (1 - contributionOfCurrentTicks) + dt * contributionOfCurrentTicks;
    return TicksPerSecond() / averageTicks;  
}


#endif