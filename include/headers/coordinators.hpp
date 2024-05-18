#ifndef COORDINATORS_HPP
#define COORDINATORS_HPP

#include <memory>

#include <entities.hpp>
#include <components.hpp>
#include <systems.hpp>
#include <auxiliaries.hpp>


class ECSCoordinator {
public:
    ECSCoordinator();

    /* Entity methods */
    type::EntityID CreateEntity() const;
    void EraseEntity(type::EntityID) const;

    /* Components methods */
    template <typename Component>
    void RegisterComponent() const;

    template <typename Component>
    void InsertComponent(type::EntityID, Component const&) const;

    template <typename Component>
    void EraseComponent(type::EntityID) const;

    template <typename Component>
    Component& GetComponent(type::EntityID) const;

    template <typename Component>
    type::ComponentID GetComponentID() const;

    /* System methods */
    template <typename System>
    std::shared_ptr<System> RegisterSystem() const;

    template <typename System>
    void SetSystemSignature(type::Signature const&) const;

private:
    std::unique_ptr<EntityManager> mEntityManager;
    std::unique_ptr<ComponentManager> mComponentManager;
    std::unique_ptr<SystemManager> mSystemManager;
};

class MasterCoordinator {
    public:
        inline MasterCoordinator() = default;
        inline ~MasterCoordinator() = default;

        void Start();

    private:
        void Initialize();
        void GameLoop() const;

        void RegisterComponents() const;
        void RegisterSystems();
        void CreateEntities();

        // std::array<type::EntityID, config::kMaxEntityID> mEntityIDs;
        type::EntityID mPlayerID;
        std::shared_ptr<MovementSystem> mMovementSystem;
};

namespace global {
    extern ECSCoordinator coordinator;
}


#include <coordinators/ecs-coordinator.tpp>

#endif