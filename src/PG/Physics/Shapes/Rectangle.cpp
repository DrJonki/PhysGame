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
    return gpm::RectF(-m_size / 2.f, m_size);
  }

  std::array<gpm::Vector2F, 4> RectangleShape::getVertices(const gpm::Vector2F& position, const float orientation) const
  {
    const auto aabb = getLocalAABB();
    const auto mat = gpm::Matrix2x2F(
      std::cos(orientation), -std::sin(orientation),
      std::sin(orientation), std::cos(orientation)
    );

    return std::array<gpm::Vector2F, 4>({
      position + gpm::Vector2F(aabb.pos.x, aabb.getExtentY()) * mat,
      position + gpm::Vector2F(aabb.getExtentX(), aabb.getExtentY()) * mat,
      position + gpm::Vector2F(aabb.getExtentX(), aabb.pos.y) * mat,
      position + gpm::Vector2F(aabb.pos.x, aabb.pos.y) * mat
    });
  }
}