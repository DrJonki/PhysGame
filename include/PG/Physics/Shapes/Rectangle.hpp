#pragma once

#include <PG/Physics/Shape.hpp>
#include <gpm/vector.hpp>
#include <array>

namespace pg
{
  class RectangleShape : public Shape
  {
  public:

    RectangleShape(const float x, const float y);

    RectangleShape(const gpm::Vector2F& size);

    float getInertia(const float mass) const override;

    gpm::RectF getLocalAABB() const override;

    std::array<gpm::Vector2F, 4> getVertices(const gpm::Vector2F& position, const float orientation) const;

  private:

    gpm::Vector2F m_size;
  };
}