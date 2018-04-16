#pragma once

#include <PG/Game/Entity.hpp>
#include <memory>
#include <vector>

namespace sf
{
  class Event;
  class RenderTarget;
}

namespace pg
{
  class Scene
  {
  private:

    friend class Engine;

  public:

    Scene();

    template<typename T>
    T& addEntity(T* entity);

  protected:

    virtual void update(const float dt);

    virtual void draw(sf::RenderTarget& target);

  private:

    virtual void onWindowEvent(const sf::Event& event);

  protected:

    std::vector<std::unique_ptr<Entity>> m_entities;
  };

  template<typename T>
  T& Scene::addEntity(T* entity)
  {
    static_assert(std::is_base_of<Entity, T>::value, "T is not derived from pg::Entity");

    m_entities.emplace_back(std::unique_ptr<T>(entity));
    return *entity;
  }
}