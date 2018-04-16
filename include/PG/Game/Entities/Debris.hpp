#pragma once

#include <PG/Game/Entity.hpp>
#include <PG/Physics/Body.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace pg
{
  class World;

  class DebrisEntity : public Entity, public Body
  {
  public:

    DebrisEntity(World& world);

  private:

    void update(const float dt) override;

    void draw(sf::RenderTarget& target) override;

  private:

    sf::RectangleShape m_drawable;
  };
}