#pragma once

#include <PG/Physics/Shape.hpp>

namespace pg
{
  class CircleShape : public Shape
  {
  public:

    CircleShape(const float radius);

    ~CircleShape() override;

    float getInertia(const float mass) const override;

    gpm::RectF getLocalAABB() const override;

  private:

    float m_radius;
  };
}