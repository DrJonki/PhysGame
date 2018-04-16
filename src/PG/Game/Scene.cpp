#include <PG/Game/Scene.hpp>
#include <PG/Game/Entity.hpp>
#include <PG/Game/Engine.hpp>
#include <SFML/Window/Event.hpp>
#include <algorithm>

namespace pg
{
  Scene::Scene()
    : m_entities()
  {}

  void Scene::update(const float dt)
  {
    for (auto& i : m_entities) {
      i->update(dt);
    }

    m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(), [](const std::unique_ptr<Entity>& entity) {
      return entity->isErased();
    }), m_entities.end());
  }

  void Scene::draw(sf::RenderTarget& target)
  {
    for (auto& i : m_entities) {
      i->draw(target);
    }
  }

  void Scene::onWindowEvent(const sf::Event& event)
  {
    if (event.type == sf::Event::EventType::Closed) {
      Engine::exit();
    }
  }
}
