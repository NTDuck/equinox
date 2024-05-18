#ifndef ENTITIES_HPP
#define ENTITIES_HPP

#include <array>
#include <queue>

#include <auxiliaries.hpp>


class EntityManager {
public:
    EntityManager();
    inline ~EntityManager() = default;
    
    type::EntityID CreateEntity();
    void EraseEntity(type::EntityID);
    type::Signature const& GetSignature(type::EntityID) const;
    void SetSignature(type::EntityID, type::Signature const&);

private:
    std::queue<type::EntityID> mAvailableEntities{};
    std::array<type::Signature, config::kMaxEntityID> mEntityToSignatureMap{};
    type::EntityID mEntityCount = 0;
};


#endif