#ifndef ECS_SYSTEMS_HPP
#define ECS_SYSTEMS_HPP

#include <memory>
#include <set>
#include <string_view>
#include <unordered_map>

#include <ecs/auxiliaries.hpp>


namespace ecs {
    class Coordinator;

    class ISystem {
    public:
        explicit ISystem(std::shared_ptr<Coordinator>);
        virtual void Integrate(std::uint32_t dt) = 0;

        std::set<EntityID> mEntityIDs;

    protected:
        std::shared_ptr<Coordinator> mCoordinator;
    };

    class SystemManager {
    public:
        template <typename System, typename... Args>
        std::shared_ptr<System> RegisterSystem(Args&&...);

        template <typename System>
        void SetSignature(Signature const&);

        void EraseEntityCallback(EntityID) const;
        void UpdateSignatureCallback(EntityID, Signature const&) const;

    private:
        std::unordered_map<std::string_view, Signature> mSystemSignatures;
        std::unordered_map<std::string_view, std::shared_ptr<ISystem>> mSystems;
    };
}


#include <ecs/systems/system-manager.tpp>

#endif