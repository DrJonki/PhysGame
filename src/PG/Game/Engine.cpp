#include <PG/Game/Engine.hpp>
#include <PG/Game/Scene.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

namespace pg
{
  Engine* Engine::ms_instance = nullptr;

  Engine::Engine()
    : m_window(sf::VideoMode(800, 600), "PhysGame")
    , m_scenes()
    , m_running(true)
  {
    assert(ms_instance == nullptr);

    ms_instance = this;

    m_window.setVerticalSyncEnabled(true);
  }

  void Engine::exit()
  {
    assert(ms_instance != nullptr);

    ms_instance->m_running = false;
  }

  sf::RenderWindow & Engine::getWindow()
  {
    assert(ms_instance != nullptr);

    return ms_instance->m_window;
  }

  int Engine::operator()()
  {
    assert(ms_instance != nullptr);

    sf::Clock clock;

    while (m_running && !m_scenes.empty()) {
      while (m_scenes.size() > 1) {
        m_scenes.pop();
      }

      auto& scene = *m_scenes.front();
      
      scene.update(clock.restart().asSeconds());

      m_window.clear();
      scene.draw(m_window);
      m_window.display();

      sf::Event event;
      while (m_window.pollEvent(event)) {
        scene.onWindowEvent(event);
      }
    }

    return EXIT_SUCCESS;
  }
}
