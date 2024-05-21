#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <memory>
#include <type_traits>
#include <string_view>

#include <SDL.h>
#include <systems.hpp>
#include <auxiliaries.hpp>


class Application {
    template <typename T>
    struct DefaultAllocator {
        template <typename... Args>
        T* operator()(Args&&...) const;
    };

    template <typename T>
    struct DefaultDeallocator {
        void operator()(T*) const;
    };

    template <typename T, typename Allocator = DefaultAllocator<T>, typename Deallocator = DefaultDeallocator<T>>
    class IDependency {
    public:
        inline IDependency() = default;
        virtual ~IDependency();

        bool Empty() const noexcept;
        T* Get() const noexcept;

        template <Allocator allocator, Deallocator deallocator, typename... Args>
        void Initialize(Args&&...);

    protected:
        std::shared_ptr<T> mPointer;
    };

    class Window : public IDependency<SDL_Window, decltype(SDL_CreateWindow), decltype(SDL_DestroyWindow)> {
    public:
        void Initialize(std::string_view const&, Rect const&, std::uint32_t);

        std::uint32_t GetID() const;

    private:
        std::uint32_t mID = -1;
    };

    class Renderer : public IDependency<SDL_Renderer, decltype(SDL_CreateRenderer), decltype(SDL_DestroyRenderer)> {
        class SpriteSheet {

        };

    public:
        void Initialize(Window const&, std::int32_t, std::uint32_t);

        void Clear() const;
        void Integrate() const;

        void SetDrawColor(Color const& color) const;
        void FillRect(Rect const&) const;
        void FillRect() const;
    };

    class Timer {
    public:
        enum class State {
            kIdle,
            kActive,
            kSuspended,
        };

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
        double GetFPS() const noexcept;

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
        static constexpr inline FTicks TicksPerSecond() noexcept { return 1000; }

        template <FPSMonitoringMethod M> std::enable_if_t<M == FPSMonitoringMethod::kFixedInterval, Ticks> GetFPS() const;
        template <FPSMonitoringMethod M> std::enable_if_t<M == FPSMonitoringMethod::kFixedInterval_, Ticks> GetFPS(Ticks dt) const;
        template <FPSMonitoringMethod M> std::enable_if_t<M == FPSMonitoringMethod::kFixedFrameTime, FTicks> GetFPS() const;
        template <FPSMonitoringMethod M> std::enable_if_t<M == FPSMonitoringMethod::kRealTime, FTicks> GetFPS(Ticks dt) const;
        template <FPSMonitoringMethod M> std::enable_if_t<M == FPSMonitoringMethod::kCommonAverage, FTicks> GetFPS() const;
        template <FPSMonitoringMethod M> std::enable_if_t<M == FPSMonitoringMethod::kExactSampling, FTicks> GetFPS(Ticks dt) const;
        template <FPSMonitoringMethod M> std::enable_if_t<M == FPSMonitoringMethod::kAverageSampling, FTicks> GetFPS(Ticks dt) const;
    };

    public:
        Application();
        ~Application();

        void Start();

    private:
        void Initialize();
        void StartGameLoop();

        void InitializeDependencies();
        void RegisterComponents() const;
        void RegisterSystems();
        void CreateEntities();

        ecs::EntityID mPlayerID;
        std::shared_ptr<MovementSystem> mMovementSystem;
        std::shared_ptr<ecs::Coordinator> mCoordinator;

        Window mWindow;
        Renderer mRenderer;

        Timer mTimer;
        FPSMonitor<config::fps::kFPSMonitoringMethod> mFPSMonitor;
        FPSRegulator mFPSRegulator;
};


#include <application/idependency.tpp>
#include <application/timers.tpp>

#endif