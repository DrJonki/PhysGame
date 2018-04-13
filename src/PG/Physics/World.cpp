#include <PG/Physics/World.hpp>
#include <PG/Physics/Body.hpp>
#include <cassert>
#include <algorithm>

namespace pg
{
  World::World(const gpm::Vector2F & gravity)
    : m_gravity(gravity)
    , m_bodies()
    , m_timer(0.f)
  {}

  void World::step(const float dt, const std::size_t substeps, const float timestep)
  {
    m_timer += dt;
    std::size_t stepsTaken = 0;

    while (m_timer >= timestep && ((substeps == 0) || stepsTaken++ < substeps)) {
      m_timer -= timestep;
      simulate(timestep);
    }

    const auto alpha = std::min(1.f, m_timer / timestep);
    for (auto& i : m_bodies) {
      i->interpolate(alpha);
    }
  }

  World & World::setGravity(const gpm::Vector2F & gravity)
  {
    m_gravity = gravity;
    return *this;
  }

  gpm::Vector2F World::getGravity() const
  {
    return m_gravity;
  }

  World & World::addBody(Body & body)
  {
    // Body must be unbound from old world first.
    assert(body.getWorld() == nullptr || body.getWorld() == this);

    body.m_worldRef = this;
    m_bodies.insert(&body);

    return *this;
  }

  World & World::removeBody(Body & body)
  {
    if (body.getWorld()) {
      // Body belongs to another world, must not remove here.
      assert(body.getWorld() == this);

      body.m_worldRef = nullptr;
      m_bodies.erase(&body);
    }

    return *this;
  }

  void World::simulate(const float timestep)
  {
    // Integrate
    for (auto& i : m_bodies) {
      i->step(timestep);
    }

    // Check collisions
    for (auto& i : m_bodies) {
      for (auto& j : m_bodies) {
        if (i != j) {
          i->checkCollision(*j);
        }
      }
    }
  }
}