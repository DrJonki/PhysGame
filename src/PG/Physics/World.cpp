#include <PG/Physics/World.hpp>
#include <PG/Physics/Body.hpp>
#include <PG/Physics/CollisionInfo.hpp>
#include <cassert>
#include <algorithm>
#include <vector>
#include <unordered_set>

namespace
{
  template<typename T>
  inline void hash_combine(std::size_t& seed, const T& v)
  {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  }

  inline float perpDotProduct(const gpm::Vector2F& a, const gpm::Vector2F& b)
  {
    return a.x * b.y - a.y * b.x;
  }
}

namespace std
{
  template<typename T1, typename T2>
  struct hash<std::pair<T1, T2>>
  {
    std::size_t operator ()(const std::pair<T1, T2>& p) const
    {
      std::size_t seed1(0);
      hash_combine(seed1, p.first);
      hash_combine(seed1, p.second);

      std::size_t seed2(0);
      hash_combine(seed2, p.second);
      hash_combine(seed2, p.first);

      return std::min(seed1, seed2);
    }
  };
}

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
    std::vector<CollisionInfo> pairs;
    pairs.reserve(m_bodies.size());

    std::unordered_set<std::pair<Body*, Body*>> saved;

    CollisionInfo info;
    
    // TODO: detect which pairs have already been handled
    for (auto& i : m_bodies) {
      for (auto& j : m_bodies) {
        if (i != j && !saved.count(std::make_pair(j, i)) && i->checkCollision(*j, info)) {
          pairs.push_back(info);
        }
      }
    }

    // Resolve collisions
    for (auto& pair : pairs) {
      const auto A = pair.bodyA;
      const auto B = pair.bodyB;

      const auto& p = pair.point;
      const auto& n = pair.normal;

      const auto vAP = p - A->getVelocity();
      const auto vBP = p - B->getVelocity();
      const auto vAB = vAP - vBP;
      
      const auto e = A->getElasticity() * B->getElasticity();

      // Apply penetration correction
      {
        if (A->isStatic()) {
          B->setPosition(B->getPosition() + -pair.normal * pair.penetrationDistance);
        }
        else if (B->isStatic()) {
          A->setPosition(A->getPosition() + pair.normal * pair.penetrationDistance);
        }
        else {
          A->setPosition(A->getPosition() + pair.normal * pair.penetrationDistance * 0.5f * A->getLinearFactor());
          B->setPosition(B->getPosition() + -pair.normal * pair.penetrationDistance * 0.5f * B->getLinearFactor());
        }
      }

      const auto jNumerator = (-(1 + e) * vAB).getDotProduct(n);

      // Apply linear impulse
      {
        const auto j = jNumerator / n.getDotProduct(n * (A->getInverseMass() + B->getInverseMass()));

        A->applyImpulse(-j * n);
        B->applyImpulse(j * n);
      }

      // Apply angular impulse
      {
        const auto rAP = p - A->getPosition();
        const auto rBP = p - B->getPosition();

        const auto j =
          jNumerator /
          
          // Denominator
          (
            n.getDotProduct(n * (A->getInverseMass() + B->getInverseMass())) +
            std::pow(perpDotProduct(rAP, n), 2.f) * A->getInverseInertia()   +
            std::pow(perpDotProduct(rBP, n), 2.f) * B->getInverseInertia()
          );

        A->applyTorqueImpulse(perpDotProduct(p - A->getPosition(), j * n));
        B->applyTorqueImpulse(perpDotProduct(p - B->getPosition(), -j * n));
      }

      // Invoke collision callbacks
      {
        A->onCollision(*B, info);

        info.normal.negate();
        info.bodyA = B;
        info.bodyB = A;

        B->onCollision(*A, info);
      }
    }
  }
}