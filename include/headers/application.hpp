#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <memory>
#include <functional>
#include <string_view>

#include <SDL.h>
#include <systems.hpp>
#include <auxiliaries.hpp>


class Application {
    template <typename T, typename Allocator, typename Deleter>
    class Dependency {
    public:
        inline Dependency() = default;
        virtual ~Dependency();

        bool Empty() const noexcept;
        T* Get() const noexcept;

        template <typename... Args>
        void Initialize(Allocator allocator, Deleter deleter, Args&&... args);

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

    public:
        inline Application() = default;
        inline ~Application() = default;

        void Start();

    private:
        void Initialize();
        void StartGameLoop() const;

        void InitializeExternalDependencies();
        void RegisterComponents() const;
        void RegisterSystems();
        void CreateEntities();

        ECS::EntityID mPlayerID;
        std::shared_ptr<MovementSystem> mMovementSystem;

        Window mWindow;
        Renderer mRenderer;
};


#include <application.tpp>

#endif