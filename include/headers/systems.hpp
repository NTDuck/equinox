#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include <memory>
#include <set>
#include <unordered_map>

#include <auxiliaries.hpp>


class ISystem {
public:
    virtual void Integrate(std::uint32_t dt) = 0;

    std::set<type::EntityID> mEntityIDs;
};

class MovementSystem : public ISystem {
public:
    void Integrate(std::uint32_t dt) override;
};

class SystemManager {
public:
    template <typename System>
    std::shared_ptr<System> RegisterSystem();

    template <typename System>
    void SetSignature(type::Signature const&);

    void EraseEntityCallback(type::EntityID) const;
    void UpdateSignatureCallback(type::EntityID, type::Signature const&) const;

private:
    std::unordered_map<std::string_view, type::Signature> mSystemSignatures;
    std::unordered_map<std::string_view, std::shared_ptr<ISystem>> mSystems;
};


#include <systems/system-manager.tpp>

#endif