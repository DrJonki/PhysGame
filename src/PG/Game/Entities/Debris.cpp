#include <PG/Game/Entities/Debris.hpp>
#include <PG/Physics/Shapes/Rectangle.hpp>
#include <PG/Physics/World.hpp>
#include <gpm/utility.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>

namespace pg
{
  DebrisEntity::DebrisEntity(World & world)
    : Entity()
    , Body(1.f)
    , m_drawable(sf::Vector2f(40, 40))
  {
    static RectangleShape shape = RectangleShape(40, 40);

    setPosition(gpm::Vector2F(200, -600));

    setShape(&shape);
    world.addBody(*this);
  }

  void DebrisEntity::update(const float)
  {
    const auto currentPos = Body::getPosition();
    const auto currentRot = Body::getOrientation();

    m_drawable.setPosition(currentPos.x, currentPos.y);
    m_drawable.setRotation(gpm::r2d(currentRot));
  }

  void DebrisEntity::draw(sf::RenderTarget & target)
  {
    target.draw(m_drawable);
  }
}
