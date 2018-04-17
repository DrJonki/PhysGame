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

  std::array<gpm::Vector2F, 4> RectangleShape::getVertices(const gpm::Matrix3x3F & transform) const
  {
    const auto aabb = getLocalAABB();

    const auto p3 = std::array<gpm::Vector3F, 4>({
      transform * gpm::Vector3F(aabb.pos.x, aabb.getExtentY(), 0.f),
      transform * gpm::Vector3F(aabb.getExtentX(), aabb.getExtentY(), 0.f),
      transform * gpm::Vector3F(aabb.getExtentX(), aabb.pos.y, 0.f),
      transform * gpm::Vector3F(aabb.pos.x, aabb.pos.y, 0.f)
    });

    return std::array<gpm::Vector2F, 4>({
      gpm::Vector2F(p3[0].x, p3[0].y),
      gpm::Vector2F(p3[1].x, p3[1].y),
      gpm::Vector2F(p3[2].x, p3[2].y),
      gpm::Vector2F(p3[3].x, p3[3].y)
    });
  }
}