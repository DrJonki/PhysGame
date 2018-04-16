#include <PG/Physics/Shapes/LineSegment.hpp>
#include <cmath>

namespace pg
{
  LineSegmentShape::LineSegmentShape(const gpm::Vector2F& start, const gpm::Vector2F& end)
    : Shape()
    , m_start(start)
    , m_end(end)
  {}

  float LineSegmentShape::getInertia(const float mass) const
  {
    return 0.0f;
  }

  gpm::RectF LineSegmentShape::getLocalAABB() const
  {
    return gpm::RectF(
      std::min(m_start.x, m_end.x), std::min(m_start.y, m_end.y),
      std::abs(m_start.x - m_end.x), std::abs(m_start.y - m_end.y)
    );
  }
}
