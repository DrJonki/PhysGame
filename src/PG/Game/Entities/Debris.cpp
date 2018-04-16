#include <PG/Game/Entities/Debris.hpp>
#include <PG/Physics/Shapes/Rectangle.hpp>
#include <PG/Physics/World.hpp>
#include <gpm/utility.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>

namespace pg
{
  DebrisEntity::DebrisEntity(World & world, const gpm::Vector2F& size)
    : Entity()
    , Body(1.f)
    , m_bodyShape()
    , m_drawable(sf::Vector2f(size.x, size.y))
  {
    m_bodyShape = std::make_unique<RectangleShape>(size);

    setShape(m_bodyShape.get());
    world.addBody(*this);

    m_drawable.setOrigin(sf::Vector2f(size.x / 2, size.y / 2));
  }

  void DebrisEntity::update(const float)
  {
    const auto currentPos = Body::getPosition();
    const auto currentRot = Body::getOrientation();

    m_drawable.setPosition(currentPos.x, -currentPos.y);
    m_drawable.setRotation(gpm::r2d(currentRot));
  }

  void DebrisEntity::draw(sf::RenderTarget & target)
  {
    target.draw(m_drawable);
  }
}
