#include <PG/Physics/Shape.hpp>

namespace pg
{
  Shape::Shape()
  {}

  Shape::~Shape()
  {}

  gpm::RectF Shape::getAABB(const gpm::Matrix3x3F& transform) const
  {
    const auto local = getLocalAABB();

    const auto tl = transform * gpm::Vector3F(
      local.pos.x, local.getExtentY(), 1.f
    );
    const auto bl = transform * gpm::Vector3F(
      local.pos.x, local.pos.y, 1.f
    );
    const auto tr = transform * gpm::Vector3F(
      local.getExtentX(), local.getExtentY(), 1.f
    );
    const auto br = transform * gpm::Vector3F(
      local.getExtentX(), local.pos.y, 1.f
    );

    const float left   = std::min({ tl.x, bl.x, tr.x, br.x });
    const float right  = std::max({ tl.x, bl.x, tr.x, br.x });
    const float top    = std::max({ tl.y, bl.y, tr.y, br.y });
    const float bottom = std::min({ tl.y, bl.y, tr.y, br.y });

    return gpm::RectF(left, bottom, right - left, top - bottom);
  }
}