#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <dependencies.hpp>
#include <systems.hpp>
#include <auxiliaries.hpp>


class Application {
public:
    Application();
    ~Application();

    void Start();

private:
    void Initialize();
    void StartGameLoop();

    void InitializeDependencies();
    void RegisterComponents() const;
    void RegisterSystems();
    void CreateEntities();

    ecs::EntityID mPlayerID;
    std::shared_ptr<MovementSystem> mMovementSystem;
    std::shared_ptr<RenderSystem> mRenderSystem;
    std::shared_ptr<ecs::Coordinator> mCoordinator;

    Window mWindow;
    Renderer mRenderer;

    Timer mTimer;
    FPSMonitor<config::fps::kFPSMonitoringMethod> mFPSMonitor;
    FPSRegulator mFPSRegulator;
};


#endif