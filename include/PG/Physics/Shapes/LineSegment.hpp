#pragma once

#include <PG/Physics/Shape.hpp>
#include <gpm/vector.hpp>
#include <array>

namespace pg
{
  class LineSegmentShape : public Shape
  {
  public:

    LineSegmentShape(const gpm::Vector2F& start, const gpm::Vector2F& end);

    float getInertia(const float mass) const override;

    gpm::RectF getLocalAABB() const override;

    std::array<gpm::Vector2F, 2> getVertices(const gpm::Vector2F& position, const float orientation) const;

  private:

    gpm::Vector2F m_start;
    gpm::Vector2F m_end;
  };
}
