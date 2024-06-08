#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <systems.hpp>
#include <services.hpp>
#include <ecs.hpp>
#include <utilities.hpp>


class Application : public utility::Singleton<Application> {
friend utility::Singleton<Application>;
public:
    void Start();

private:
    ~Application();

    void Initialize();
    void InitializeServices();
    void RegisterComponents();
    void RegisterSystems();

    void GameLoop();

    std::shared_ptr<ecs::Coordinator> mCoordinator;

    std::shared_ptr<systems::Movement> mMovementSystem;
    std::shared_ptr<systems::Render> mRenderSystem;
    std::shared_ptr<systems::ArbitraryLoop> mArbitraryLoopSystem;

    Window mWindow;
    Renderer mRenderer;

    Timer mTimer;
    FPSRegulator mFPSRegulator;
    FPSMonitor<config::fps::kFPSMonitoringMethod> mFPSMonitor;
};


#endif