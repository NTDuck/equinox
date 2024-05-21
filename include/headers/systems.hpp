#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include <memory>
#include <auxiliaries.hpp>


class MovementSystem : public ecs::ISystem {
public:
    MovementSystem(std::shared_ptr<ecs::Coordinator>);
    void Integrate(std::uint32_t dt) override;
};


#endif