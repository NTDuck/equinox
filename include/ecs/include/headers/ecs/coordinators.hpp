#ifndef ECS_COORDINATORS_HPP
#define ECS_COORDINATORS_HPP

#include <memory>

#include <ecs/entities.hpp>
#include <ecs/components.hpp>
#include <ecs/systems.hpp>
#include <ecs/auxiliaries.hpp>


namespace ecs {
    class Coordinator {
    public:
        Coordinator();

        /* Entity methods */
        EntityID CreateEntity() const;
        void EraseEntity(EntityID) const;

        /* Components methods */
        template <typename Component>
        void RegisterComponent() const;

        template <typename... Components>
        void RegisterComponent() const;

        template <typename Component>
        void InsertComponent(EntityID, Component const&) const;

        template <typename Component>
        void EraseComponent(EntityID) const;

        template <typename Component>
        Component& GetComponent(EntityID) const;

        template <typename Component>
        ComponentID GetComponentID() const;

        /* System methods */
        template <typename System, typename... Args>
        std::shared_ptr<System> RegisterSystem(Args&&...) const;

        template <typename System>
        void SetSystemSignature(Signature const&) const;

        template <typename System, typename... Components>
        void SetSystemSignature() const;

    private:
        std::unique_ptr<EntityManager> mEntityManager;
        std::unique_ptr<ComponentManager> mComponentManager;
        std::unique_ptr<SystemManager> mSystemManager;
    };
}


#include <ecs/coordinators/coordinator.tpp>

#endif