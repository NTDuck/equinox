#ifndef SERVICES_HPP
#define SERVICES_HPP

#include <SDL.h>
#include <SDL_image.h>

#include <utilities.hpp>
#include <auxiliaries.hpp>


class Window : public utility::SmartPointer<SDL_Window, SDL_CreateWindow, SDL_DestroyWindow> {
public:
    void Initialize(std::string_view const&, Rect const&, std::uint32_t);
    std::uint32_t GetID() const;

private:
    std::uint32_t mID = -1;
};

class SpriteSheet;

class Renderer : public utility::SmartPointer<SDL_Renderer, SDL_CreateRenderer, SDL_DestroyRenderer> {
public:
    void Initialize(Window const&, std::int32_t, std::uint32_t);

    void Clear() const;
    void RenderPresent() const;

    void SetDrawColor(Color const&) const;
    void FillRect(Rect const&) const;
    void FillRect() const;

    void RenderCopy(SpriteSheet const&, Rect const&, Rect const&) const;
};

class SpriteSheet : public utility::SmartPointer<SDL_Texture, IMG_LoadTexture, SDL_DestroyTexture> {
public:
    void Initialize(Renderer const&, std::string_view, Point const&);
    Rect GetSrcRect(SpriteID) const noexcept;

private:
    Point mSpriteSize;
    std::int32_t mSpriteColumnCount;
};

class Timer {
public:
    enum class State {
        kIdle,
        kActive,
        kSuspended,
    };

    static constexpr inline FTicks TicksPerSecond() noexcept { return 1000; }

    Timer();

    void Start();
    void Stop();
    void Pause();
    void Unpause();

    Ticks GetTicks() const noexcept;
    Ticks GetDeltaTime() const noexcept;
    State GetState() const noexcept;

protected:
    State mState;
    Ticks mStartTicks, mPausedTicks;
};

class FPSRegulator : Timer {
public:
    void SetFPS(double);
    FTicks GetFPS() const noexcept;

    void PreIntegrate();
    void PostIntegrate() const;

private:
    Ticks mTicksPerFrame;
};

template <FPSMonitoringMethod Method>
class FPSMonitor : Timer {
public:
    void Start();

    template <typename... Args>
    decltype(auto) GetFPS(Args&&...) const;

private:
    template <FPSMonitoringMethod M> std::enable_if_t<M == FPSMonitoringMethod::kFixedInterval, Ticks> GetFPS() const;
    template <FPSMonitoringMethod M> std::enable_if_t<M == FPSMonitoringMethod::kFixedInterval_, Ticks> GetFPS(Ticks dt) const;
    template <FPSMonitoringMethod M> std::enable_if_t<M == FPSMonitoringMethod::kFixedFrameTime, FTicks> GetFPS() const;
    template <FPSMonitoringMethod M> std::enable_if_t<M == FPSMonitoringMethod::kRealTime, FTicks> GetFPS(Ticks dt) const;
    template <FPSMonitoringMethod M> std::enable_if_t<M == FPSMonitoringMethod::kCommonAverage, FTicks> GetFPS() const;
    template <FPSMonitoringMethod M> std::enable_if_t<M == FPSMonitoringMethod::kExactSampling, FTicks> GetFPS(Ticks dt) const;
    template <FPSMonitoringMethod M> std::enable_if_t<M == FPSMonitoringMethod::kAverageSampling, FTicks> GetFPS(Ticks dt) const;
};


#include <services/fps-monitor.tpp>

#endif