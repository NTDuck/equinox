#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <memory>

#include <systems.hpp>
#include <auxiliaries.hpp>


class Application {
    public:
        inline Application() = default;
        inline ~Application() = default;

        void Start();

    private:
        void Initialize();
        void GameLoop() const;

        void RegisterComponents() const;
        void RegisterSystems();
        void CreateEntities();

        ECS::EntityID mPlayerID;
        std::shared_ptr<MovementSystem> mMovementSystem;
};


#endif