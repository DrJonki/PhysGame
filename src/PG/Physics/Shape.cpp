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

    return local;
  }
}