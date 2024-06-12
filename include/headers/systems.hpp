#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include <services.hpp>
#include <events.hpp>
#include <ecs.hpp>


namespace systems {
    class Movement : public ecs::ISystem {
    public:
        Movement(std::shared_ptr<ecs::Coordinator>);
        void Integrate(double) const;
    };

    class Render : public ecs::ISystem {
    public:
        Render(std::shared_ptr<ecs::Coordinator>, Renderer const&);
        void Initialize(std::array<std::pair<std::string_view, Point>, config::kMaxSpriteSheetID> const&);
        void Integrate() const;

    private:
        static Rect GetDstRect(Rect const&, Point const&) noexcept;

        Renderer const& mRenderer;
        std::array<SpriteSheet, config::kMaxSpriteSheetID> mSpriteSheets;
    };

    class PlayerInput : public ecs::ISystem {
    public:
        PlayerInput(std::shared_ptr<ecs::Coordinator>);
        void OnKeyboardEvent(std::shared_ptr<EventManager::Event<SDL_KeyboardEvent>>) const;
    };

    class ArbitraryLoop : public ecs::ISystem {
    public:
        ArbitraryLoop(std::shared_ptr<ecs::Coordinator>);
        void OnArbitraryEvent(std::shared_ptr<events::ArbitraryEvent>) const;
    };

    class ArbitraryApplicationTerminator : public ecs::ISystem {
    public:
        ArbitraryApplicationTerminator();
        void Integrate(std::shared_ptr<EventManager::Event<SDL_QuitEvent>>) noexcept;
        bool GetStatus() const noexcept;
        
    private:
        bool isRunning = true;
    };
}


#endif