#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include <memory>

#include <dependencies.hpp>
#include <auxiliaries.hpp>


class MovementSystem : public ecs::ISystem {
public:
    MovementSystem(std::shared_ptr<ecs::Coordinator>);
    void Integrate(std::uint32_t dt) override;
};

class RenderSystem : public ecs::ISystem {
public:
    RenderSystem(std::shared_ptr<ecs::Coordinator>, Renderer const&);
    void Initialize(std::array<std::pair<std::string_view, Point>, config::kMaxSpriteSheetID> const&);
    void Integrate(std::uint32_t dt) override;

private:
    static Rect GetDstRect(Rect const&, Point const&) noexcept;

    Renderer const& mRenderer;
    std::array<SpriteSheet, config::kMaxSpriteSheetID> mSpriteSheets;
};


#endif