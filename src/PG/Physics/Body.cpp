#include <PG/Physics/Body.hpp>
#include <PG/Physics/Shape.hpp>
#include <PG/Physics/Detail/Integrator.hpp>

namespace pg
{
  Body::Body(const float mass, const Shape& shape)
    : m_shapeRef(shape)
    , m_position()
    , m_orientation(0.f)
    , m_mass(mass)
    , m_force()
    , m_velocity()
    , m_angularVelocity(0.f)
    , m_torque(0.f)
    , m_elasticity(0.f)
  {}

  gpm::Vector2F Body::getPosition() const
  {
    return m_position;
  }

  Body & Body::setPosition(const gpm::Vector2F & position)
  {
    m_position = position;
    return *this;
  }

  float Body::getOrientation() const
  {
    return m_orientation;
  }

  Body & Body::setOrientation(const float orientation)
  {
    m_orientation = orientation;
    return *this;
  }

  gpm::Vector2F Body::getVelocity() const
  {
    return m_velocity;
  }

  Body & Body::setVelocity(const gpm::Vector2F & velocity)
  {
    m_velocity = velocity;
    return *this;
  }

  float Body::getAngularVelocity() const
  {
    return m_angularVelocity;
  }

  Body & Body::setAngularVelocity(const float angularVelocity)
  {
    m_angularVelocity = angularVelocity;
    return *this;
  }

  gpm::Vector2F Body::getTotalForce() const
  {
    return m_force;
  }

  Body & Body::applyForce(const gpm::Vector2F & force)
  {
    m_force += force;
    return *this;
  }

  float Body::getTotalTorque() const
  {
    return m_torque;
  }

  Body & Body::applyTorque(const float torque)
  {
    m_torque += torque;
    return *this;
  }

  Body & Body::clearForces()
  {
    m_force = gpm::Vector2F();
    m_torque = 0.f;

    return *this;
  }

  float Body::getElasticity() const
  {
    return m_elasticity;
  }

  Body & Body::setElasticity(const float elasticity)
  {
    m_elasticity = elasticity;
    return *this;
  }

  float Body::getMass() const
  {
    return m_mass;
  }

  float Body::getInverseMass() const
  {
    return 1.f / getMass();
  }

  float Body::getInertia() const
  {
    return m_shapeRef.getInertia(getMass());
  }

  float Body::getInverseInertia() const
  {
    return 1.f / getInertia();
  }

  void Body::step(const float dt)
  {
    m_position = integrate(m_position, dt, m_velocity);
    m_orientation = integrate(m_orientation, dt, m_angularVelocity);
    m_velocity = integrate(m_velocity, dt, getInverseMass() * getTotalForce());
    m_angularVelocity = integrate(m_angularVelocity, dt, getInverseInertia() * getTotalTorque());

    clearForces();
  }
}