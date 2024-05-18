#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <memory>
#include <type_traits>
#include <unordered_map>

#include <auxiliaries.hpp>


namespace components {
    struct Transform {
        type::Point position;
        double rotation;
    };

    struct Motion {
        type::Point velocity;
        type::Point acceleration;
    };

    struct Sprite;
    struct Collision;
    struct Combat;
    struct Follow;
    struct Joystick;
};

class IComponentArray {
public: 
    inline virtual ~IComponentArray() = default;
    virtual void EraseEntityCallback(type::EntityID) = 0;
};

template <typename Component>
class ComponentArray : public IComponentArray {
public:
    inline ComponentArray() = default;
    inline ~ComponentArray() = default;

    inline std::size_t size() { return mSize; }
    inline bool empty() { return mSize == 0; }

    void InsertComponent(type::EntityID, Component const&);
    void EraseComponent(type::EntityID);
    Component& GetComponent(type::EntityID);
    void EraseEntityCallback(type::EntityID) override;

private:
    std::array<Component, config::kMaxEntityID> mComponentArray;
    std::unordered_map<type::EntityID, std::size_t> mEntityToIndexMap;   // Maps entity ID to array index
    std::unordered_map<std::size_t, type::EntityID> mIndexToEntityMap;   // Maps array index to entity ID

    std::size_t mSize = 0;
};

class ComponentManager {
public:
    template <typename Component>
    void RegisterComponent();

    template <typename Component>
    type::ComponentID GetComponentID() const;

    template <typename Component>
    void InsertComponent(type::EntityID, Component const&);

    template <typename Component>
    void EraseComponent(type::EntityID);

    template <typename Component>
    Component& GetComponent(type::EntityID);

    void EraseEntityCallback(type::EntityID);

private:
    template <typename Component>
    std::shared_ptr<ComponentArray<Component>> GetComponentArray();

    std::unordered_map<std::string_view, type::ComponentID> mComponentIDs;
    std::unordered_map<std::string_view, std::shared_ptr<IComponentArray>> mComponentArrays;
    type::ComponentID mNextComponentID = 0;
};


#include <components/component-array.tpp>
#include <components/component-manager.tpp>

#endif