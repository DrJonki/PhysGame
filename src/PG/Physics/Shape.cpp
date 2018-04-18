#include <PG/Physics/Shape.hpp>

namespace pg
{
  Shape::Shape()
  {}

  Shape::~Shape()
  {}

  gpm::RectF Shape::getAABB(const gpm::Vector2F& position, const float orientation) const
  {
    const auto local = getLocalAABB();
    const auto mat = gpm::Matrix2x2F(
      std::cos(orientation), -std::sin(orientation),
      std::sin(orientation), std::cos(orientation)
    );

    const auto tl = position + gpm::Vector2F(local.pos.x, local.getExtentY()) * mat;
    const auto bl = position + gpm::Vector2F(local.pos.x, local.pos.y) * mat;
    const auto tr = position + gpm::Vector2F(local.getExtentX(), local.getExtentY()) * mat;
    const auto br = position + gpm::Vector2F(local.getExtentX(), local.pos.y) * mat;

    const float left   = std::min({ tl.x, bl.x, tr.x, br.x });
    const float right  = std::max({ tl.x, bl.x, tr.x, br.x });
    const float top    = std::max({ tl.y, bl.y, tr.y, br.y });
    const float bottom = std::min({ tl.y, bl.y, tr.y, br.y });

    return gpm::RectF(left, bottom, right - left, top - bottom);
  }
}