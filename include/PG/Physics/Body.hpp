#pragma once

#include <gpm/vector.hpp>

namespace pg
{
  class Shape;

  class Body
  {
    friend class World;

  public:

    enum class Type
    {
      Static,
      Dynamic,
    };

  public:

    Body(const float mass, const Shape& shape);

    gpm::Vector2F getPosition() const;

    Body& setPosition(const gpm::Vector2F& position);

    float getOrientation() const;

    Body& setOrientation(const float orientation);

    gpm::Vector2F getVelocity() const;

    Body& setVelocity(const gpm::Vector2F& velocity);

    float getAngularVelocity() const;

    Body& setAngularVelocity(const float angularVelocity);

    gpm::Vector2F getTotalForce() const;

    Body& applyForce(const gpm::Vector2F& force);

    float getTotalTorque() const;

    Body& applyTorque(const float torque);

    Body& clearForces();

    float getElasticity() const;
    
    Body& setElasticity(const float elasticity);

    float getMass() const;

    float getInverseMass() const;

    float getInertia() const;

    float getInverseInertia() const;

  private:

    void step(const float dt);

  private:

    const Shape& m_shapeRef;
    gpm::Vector2F m_position;
    float m_orientation;
    float m_mass;
    gpm::Vector2F m_force;
    gpm::Vector2F m_velocity;
    float m_angularVelocity;
    float m_torque;
    float m_elasticity;
  };
}
