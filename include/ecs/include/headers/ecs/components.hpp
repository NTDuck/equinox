#ifndef ECS_COMPONENTS_HPP
#define ECS_COMPONENTS_HPP

#include <memory>
#include <string_view>
#include <unordered_map>

#include <ecs/auxiliaries.hpp>


namespace ecs {
    class IComponentArray {
    public: 
        inline virtual ~IComponentArray() = default;
        virtual void EraseEntityCallback(EntityID) = 0;
    };

    template <typename Component>
    class ComponentArray : public IComponentArray {
    public:
        inline std::size_t Size() { return mSize; }
        inline bool Empty() { return mSize == 0; }

        void InsertComponent(EntityID, Component const&);
        void EraseComponent(EntityID);
        Component& GetComponent(EntityID);
        void EraseEntityCallback(EntityID) override;

    private:
        std::array<Component, config::kMaxEntityID> mComponentArray;
        std::unordered_map<EntityID, std::size_t> mEntityToIndexMap;   // Maps entity ID to array index
        std::unordered_map<std::size_t, EntityID> mIndexToEntityMap;   // Maps array index to entity ID

        std::size_t mSize = 0;
    };

    class ComponentManager {
    public:
        template <typename Component>
        void RegisterComponent();

        template <typename Component>
        ComponentID GetComponentID() const;

        template <typename Component>
        void InsertComponent(EntityID, Component const&);

        template <typename Component>
        void EraseComponent(EntityID);

        template <typename Component>
        Component& GetComponent(EntityID);

        void EraseEntityCallback(EntityID);

    private:
        template <typename Component>
        std::shared_ptr<ComponentArray<Component>> GetComponentArray();

        std::unordered_map<std::string_view, ComponentID> mComponentIDs;
        std::unordered_map<std::string_view, std::shared_ptr<IComponentArray>> mComponentArrays;
        ComponentID mNextComponentID = 0;
    };
}


#include <ecs/components/component-array.tpp>
#include <ecs/components/component-manager.tpp>

#endif