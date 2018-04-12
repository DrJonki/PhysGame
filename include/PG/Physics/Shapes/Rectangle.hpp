#pragma once

#include <PG/Physics/Shape.hpp>
#include <gpm/vector.hpp>

namespace pg
{
  class RectangleShape : public Shape
  {
  public:

    RectangleShape(const float x, const float y);

    RectangleShape(const gpm::Vector2F& size);

    float getInertia(const float mass) const override;

    gpm::RectF getLocalAABB() const override;

  private:

    gpm::Vector2F m_size;
  };
}