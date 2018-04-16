#pragma once

#include <gpm/vector.hpp>

namespace pg
{
  class Body;

  namespace detail
  {
    void collide(const Body& b1, const Body& b2);
  }
}
