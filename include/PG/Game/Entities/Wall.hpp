#pragma once

#include <PG/Game/Entity.hpp>
#include <PG/Physics/Body.hpp>
#include <PG/Physics/Shapes/LineSegment.hpp>
#include <gpm/vector.hpp>

namespace pg
{
  class WallEntity : public Entity, public Body
  {
  public:

    WallEntity(World& world, const gpm::Vector2F& start, const gpm::Vector2F& end);

  private:

    void draw(sf::RenderTarget& target) override;

  private:

    LineSegmentShape m_shape;
  };
}
