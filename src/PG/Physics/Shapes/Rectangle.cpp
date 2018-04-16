#include <PG/Physics/Shapes/Rectangle.hpp>
#include <cmath>

namespace pg
{
  RectangleShape::RectangleShape(const float x, const float y)
    : m_size(x, y)
  {}

  RectangleShape::RectangleShape(const gpm::Vector2F& size)
    : RectangleShape(size.x, size.y)
  {}

  float RectangleShape::getInertia(const float mass) const
  {
    return (1.f / 12.f) * mass * (std::pow(m_size.x, 2) + std::pow(m_size.y, 2));
  }

  gpm::RectF RectangleShape::getLocalAABB() const
  {
    return gpm::RectF(gpm::Vector2F(), m_size);
  }
}