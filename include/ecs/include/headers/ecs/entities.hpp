#ifndef ECS_ENTITIES_HPP
#define ECS_ENTITIES_HPP

#include <array>
#include <queue>

#include <ecs/auxiliaries.hpp>


namespace ECS {
    class EntityManager {
    public:
        EntityManager();
        
        EntityID CreateEntity();
        void EraseEntity(EntityID);
        Signature const& GetSignature(EntityID) const;
        void SetSignature(EntityID, Signature const&);

    private:
        std::queue<EntityID> mAvailableEntityIDs;
        std::array<Signature, config::kMaxEntityID> mEntityToSignatureMap;
        EntityID mEntityCount = 0;
    };
}


#endif