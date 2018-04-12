#pragma once

#include <gpm/rectangle.hpp>
#include <gpm/matrix.hpp>

namespace pg
{
  class Shape
  {
  protected:

    Shape();

  public:

    virtual ~Shape() = 0;

    virtual float getInertia(const float mass) const = 0;

    gpm::RectF getAABB(const gpm::Matrix3x3F& transform) const;

  private:

    virtual gpm::RectF getLocalAABB() const = 0;
  };
}