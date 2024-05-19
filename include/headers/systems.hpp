#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include <auxiliaries.hpp>


class MovementSystem : public ECS::ISystem {
public:
    void Integrate(std::uint32_t dt) override;
};


#endif