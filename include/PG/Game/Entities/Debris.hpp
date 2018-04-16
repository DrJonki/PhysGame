#pragma once

#include <PG/Game/Entity.hpp>
#include <PG/Physics/Body.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <memory>

namespace pg
{
  class World;

  class DebrisEntity : public Entity, public Body
  {
  public:

    DebrisEntity(World& world, const gpm::Vector2F& size);

  private:

    void update(const float dt) override;

    void draw(sf::RenderTarget& target) override;

  private:

    std::unique_ptr<Shape> m_bodyShape;
    sf::RectangleShape m_drawable;
  };
}