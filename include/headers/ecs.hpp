#ifndef ECS_HPP
#define ECS_HPP

#include <cstdint>

#include <array>
#include <bitset>
#include <unordered_map>
#include <set>
#include <memory>
#include <tuple>
#include <typeindex>
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

    template <typename... Args>
    struct IComponent {
        IComponent() = delete;
        ~IComponent() = delete;

        using Object = std::tuple<Args...>;
    };

    class IComponentArray {
    public:
        virtual ~IComponentArray() = default;
        virtual void EntityDestroyedCallback(EntityID) = 0;
    };

    /**
     * @note `Component` must derive from `IComponent<Args...>`.
    */
    template <typename Component>
    class ComponentArray : public IComponentArray, public utility::StructOfArray<EntityID, config::kMaxEntityID, typename Component::Object> {
    public:
        void EntityDestroyedCallback(EntityID) override;
    };

    class ComponentManager {
    public:
        template <typename Component>
        void RegisterComponent();

        template <typename Component>
        ComponentID GetComponentID() const;

        template <typename Component>
        void AddComponent(EntityID, typename Component::Object const&);

        template <typename Component>
        void RemoveComponent(EntityID);
        
        template <typename Component, std::size_t I>
        decltype(auto) GetMember(EntityID);

        void EntityDestroyedCallback(EntityID);

    private:
        template <typename Component>
        std::shared_ptr<ComponentArray<Component>> GetComponentArray();

        std::unordered_map<std::type_index, ComponentID> mComponentIDs{};
        std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>> mComponentArrays{};
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
        std::unordered_map<std::type_index, Signature> mSignatures{};
        std::unordered_map<std::type_index, std::shared_ptr<ISystem>> mSystems{};
    };

    struct IEvent {
        virtual ~IEvent() = default;
    };

    struct IEventHandler {
        virtual void Invoke(std::shared_ptr<IEvent>) const = 0;
    };

    /**
     * @tparam Callback `void(System::*)(std::shared_ptr<Event>)`
    */
    template <decltype(auto) Callback>
    struct EventHandler : IEventHandler {
        using System = typename utility::StaticFunctionPointer<decltype(Callback)>::Class;
        using Event = typename std::tuple_element_t<0, typename utility::StaticFunctionPointer<decltype(Callback)>::Params>::element_type;

        EventHandler(std::shared_ptr<System>);
        void Invoke(std::shared_ptr<IEvent>) const override;

    private:
        std::shared_ptr<System> mSystem;
    };

    class EventManager {
    public:
        template <typename Event>
        void Publish(std::shared_ptr<Event>);

        template <decltype(auto) Callback>
        void Subscribe(std::shared_ptr<typename EventHandler<Callback>::System>);

        template <decltype(auto) Callback>
        void Unsubscribe(std::shared_ptr<typename EventHandler<Callback>::System>);

    private:
        using EventHandlers = std::set<std::unique_ptr<IEventHandler>>;
        std::unordered_map<std::type_index, EventHandlers> mSubscribers;
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
        void AddComponent(EntityID, typename Component::Object const&) const;

        template <typename Component>
        void RemoveComponent(EntityID) const;

        template <typename Component, std::size_t I>
        decltype(auto) GetMember(EntityID) const;

        template <typename Component>
        ComponentID GetComponentID() const;

        /* System methods */
        template <typename System, typename... Args>
        std::shared_ptr<System> RegisterSystem(Args&&...) const;

        template <typename System>
        void SetSystemSignature(Signature const&) const;

        template <typename System, typename... Components>
        void SetSystemSignature() const;

        /* Event methods */
        template <typename Event>
        void PublishEvent(std::shared_ptr<Event>) const;

        template <decltype(auto) Callback>
        void SubscribeEvent(std::shared_ptr<typename EventHandler<Callback>::System>) const;

    private:
        std::unique_ptr<EntityManager> mEntityManager;
        std::unique_ptr<ComponentManager> mComponentManager;
        std::unique_ptr<SystemManager> mSystemManager;
        std::unique_ptr<EventManager> mEventManager;
    };
}


#include <ecs/component-array.tpp>
#include <ecs/component-manager.tpp>
#include <ecs/system-manager.tpp>
#include <ecs/event-manager.tpp>
#include <ecs/coordinator.tpp>

#endif