#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <utilities.hpp>


class Application : public utility::Singleton<Application> {
friend utility::Singleton<Application>;
public:
    void Start();

private:
    ~Application();

    void Initialize();
    void GameLoop();
};


#endif