#pragma once

#include <gpm/vector.hpp>
#include <set>

namespace pg
{
  class Body;

  class World
  {
  public:

    World(const gpm::Vector2F& gravity = gpm::Vector2F(0.f, -9.81f));

    /// \brief Run a single simulation step
    ///
    /// \param dt Delta-Time, may vary.
    /// \param substeps How many timestep-sized substeps are allowed to catch up in the simulation? Pass 0 for infinite.
    /// \param timestep The length of the simulation time step. Constant value is recommended.
    ///
    void step(const float dt, const std::size_t substeps = 1, const float timestep = 1.f / 60);

    World& setGravity(const gpm::Vector2F& gravity);

    gpm::Vector2F getGravity() const;

    World& addBody(Body& body);

    World& removeBody(Body& body);

  private:

    void simulate(const float timestep);

  private:

    gpm::Vector2F m_gravity;
    std::set<Body*> m_bodies;
    float m_timer;
  };
}