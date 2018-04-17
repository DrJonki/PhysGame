#pragma once

#include <gpm/vector.hpp>

namespace pg
{
  class Body;
  struct CollisionInfo;

  namespace detail
  {
    bool collide(Body& b1, Body& b2, CollisionInfo& info);
  }
}
