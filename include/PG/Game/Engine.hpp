#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <queue>
#include <memory>
#include <cassert>

namespace pg
{
  class Scene;

  class Engine
  {
  public:

    Engine();

    static void exit();

    static sf::RenderWindow& getWindow();

    template<typename T>
    static T& pushScene(T* scene);

    int operator()();

  private:

    static Engine* ms_instance;

  private:

    sf::RenderWindow m_window;
    std::queue<std::unique_ptr<Scene>> m_scenes;
    bool m_running;
  };

  template<typename T>
  T& Engine::pushScene(T* scene)
  {
    assert(ms_instance != nullptr);

    ms_instance->m_scenes.push(std::unique_ptr<Scene>(scene));

    return *scene;
  }
}
