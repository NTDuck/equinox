#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <memory>
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
    class Dependency {
    public:
        inline Dependency() = default;
        virtual ~Dependency();

        bool Empty() const noexcept;
        T* Get() const noexcept;

        template <Allocator allocator, Deallocator deallocator, typename... Args>
        void Initialize(Args&&...);

    protected:
        std::shared_ptr<T> mPointer;
    };

    class Window : public Dependency<SDL_Window, decltype(SDL_CreateWindow), decltype(SDL_DestroyWindow)> {
    public:
        void Initialize(std::string_view const&, Rect const&, std::uint32_t);

        std::uint32_t GetID() const;

    private:
        std::uint32_t mID = -1;
    };

    class Renderer : public Dependency<SDL_Renderer, decltype(SDL_CreateRenderer), decltype(SDL_DestroyRenderer)> {
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

        std::uint64_t GetTicks() const noexcept;
        State GetState() const noexcept;

    protected:
        State mState;
        std::uint64_t mStartTicks, mPausedTicks;
    };

    class FPSRegulator : Timer {
    public:
        void SetFPS(double);
        double GetFPS() const noexcept;

        void PreIntegrate();
        void PostIntegrate() const;

    private:
        std::uint32_t mTicksPerFrame;
    };

    class FPSCalculator : Timer {
    public:
        void Start();
        double GetFPS() noexcept;

    private:
        std::uint64_t mFrames = 0;
    };

    public:
        inline Application() = default;
        inline ~Application() = default;

        void Start();

    private:
        void Initialize();
        void StartGameLoop();

        void InitializeDependencies();
        void RegisterComponents() const;
        void RegisterSystems();
        void CreateEntities();

        ECS::EntityID mPlayerID;
        std::shared_ptr<MovementSystem> mMovementSystem;

        Window mWindow;
        Renderer mRenderer;

        Timer mTimer;
        FPSRegulator mFPSRegulator;
        FPSCalculator mFPSCalculator;
};


#include <application.tpp>

#endif