#include <PG/Physics/Shapes/Circle.hpp>

namespace pg
{
  CircleShape::CircleShape(const float radius)
    : m_radius(radius)
  {}

  CircleShape::~CircleShape()
  {}

  float CircleShape::getInertia(const float mass) const
  {
    return (1 / 2) * mass * m_radius;
  }

  gpm::RectF CircleShape::getLocalAABB() const
  {
    return gpm::RectF(0, 0, m_radius * 2, m_radius * 2);
  }
}
