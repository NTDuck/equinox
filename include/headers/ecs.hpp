#ifndef ECS_HPP
#define ECS_HPP

#include <cstdint>

#include <array>
#include <bitset>
#include <unordered_map>
#include <set>
#include <string_view>
#include <memory>
#include <queue>

#include <utilities.hpp>


/**
 * @see https://austinmorlan.com/posts/entity_component_system/
*/
namespace ecs {
    using EntityID = std::uint32_t;
    using ComponentID = std::uint16_t;

    namespace config {
        static constexpr EntityID kMaxEntityID = 1024;
        static constexpr ComponentID kMaxComponentID = 32;
    }

    using namespace ecs::config;
    using Signature = std::bitset<kMaxComponentID>;

    class EntityManager : public utility::Singleton<EntityManager> {
    public:
        EntityManager();

        EntityID CreateEntity();
        void DestroyEntity(EntityID);
        void SetSignature(EntityID, Signature const&);
        Signature const& GetSignature(EntityID);

    private:
        std::queue<EntityID> mAvailableEntityIDs{};
        std::array<Signature, kMaxEntityID> mSignatures{};
        std::size_t mEntityCount{};
    };

    class IComponentArray {
    public:
        virtual ~IComponentArray() = default;
        virtual void EntityDestroyedCallback(EntityID) = 0;
    };

    template <typename Component>
    class ComponentArray : public IComponentArray {
    public:
        void Insert(EntityID, Component const&);
        void Erase(EntityID);
        Component& Get(EntityID);
        void EntityDestroyedCallback(EntityID) override;

        template <typename... Args>
        void Emplace(EntityID, Args&&...);

    private:
        std::array<Component, kMaxEntityID> mComponentArray{};
        std::unordered_map<EntityID, std::size_t> mEntityToIndexMap{};   // Maps entity ID to array index
        std::unordered_map<std::size_t, EntityID> mIndexToEntityMap{};   // Maps array index to entity ID
        std::size_t mSize{};
    };

    class ComponentManager {
    public:
        template <typename Component>
        void RegisterComponent();

        template <typename Component>
        ComponentID GetComponentID() const;

        template <typename Component>
        void AddComponent(EntityID, Component const&);

        template <typename Component>
        void RemoveComponent(EntityID);
        
        template <typename Component>
        Component& GetComponent(EntityID);

        void EntityDestroyedCallback(EntityID);

    private:
        template <typename Component>
        std::shared_ptr<ComponentArray<Component>> GetComponentArray();

        std::unordered_map<std::string_view, ComponentID> mComponentIDs{};
        std::unordered_map<std::string_view, std::shared_ptr<IComponentArray>> mComponentArrays{};
        ComponentID mNextComponentID{};
    };

    class Coordinator;

    class ISystem {
    friend Coordinator;
    friend class SystemManager;
    public:
        ISystem(std::shared_ptr<Coordinator>);

    protected:
        std::set<EntityID> mEntityIDs{};
        std::shared_ptr<Coordinator> mCoordinator;
    };

    class SystemManager {
    public:
        template <typename System, typename... Args>
        std::shared_ptr<System> RegisterSystem(Args&&...);

        template <typename System>
        void SetSignature(Signature const&);

        void EntityDestroyedCallback(EntityID);
        void EntitySignatureUpdatedCallback(EntityID, Signature const&);

    private:
        std::unordered_map<std::string_view, Signature> mSignatures{};
        std::unordered_map<std::string_view, std::shared_ptr<ISystem>> mSystems{};
    };

    class Coordinator : public utility::Singleton<Coordinator> {
    friend utility::Singleton<Coordinator>;
    public:
        Coordinator();

        /* Entity methods */
        EntityID CreateEntity() const;
        void DestroyEntity(EntityID) const;

        /* Component methods */
        template <typename Component>
        void RegisterComponent() const;

        template <typename Component>
        void AddComponent(EntityID, Component const&) const;

        template <typename Component, typename... Args>
        void EmplaceComponent(EntityID, Args&&...) const;

        template <typename Component>
        void RemoveComponent(EntityID) const;

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


#include <ecs/component-array.tpp>
#include <ecs/component-manager.tpp>
#include <ecs/system-manager.tpp>
#include <ecs/coordinator.tpp>

#endif