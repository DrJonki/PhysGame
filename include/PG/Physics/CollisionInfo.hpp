#pragma once

#include <gpm/vector.hpp>

namespace pg
{
  class Body;

  struct CollisionInfo
  {
    gpm::Vector2F point;
    gpm::Vector2F normal;

    Body* bodyB;
  };
}
