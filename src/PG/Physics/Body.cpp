#include <PG/Physics/Body.hpp>
#include <PG/Physics/Shape.hpp>
#include <PG/Physics/World.hpp>
#include <PG/Physics/Detail/Integrator.hpp>
#include <PG/Physics/Detail/CollisionAlgorithms.hpp>
#include <cmath>
#include <cassert>

namespace pg
{
  Body::Body(const float mass)
    : m_shapeRef(nullptr)
    , m_worldRef(nullptr)
  #ifdef PG_PHYSICS_INTERPOLATION
    , m_previousPosition()
    , m_previousOrientation(0.f)
    , m_interpolationAlpha(0.f)
  #endif
    , m_targetPosition()
    , m_targetOrientation(0.f)
    , m_mass(mass)
    , m_gravityScale(1.f, 1.f)
    , m_force()
    , m_velocity()
    , m_angularVelocity(0.f)
    , m_torque(0.f)
    , m_elasticity(0.f)
  {
  }

  Body::Body(const float mass, const Shape& shape)
    : Body(mass)
  {
    m_shapeRef = &shape;
    m_worldRef = nullptr;
  }

  Body::~Body()
  {
    if (getWorld()) {
      getWorld()->removeBody(*this);
    }
  }

  gpm::Vector2F Body::getPosition() const
  {
  #ifdef PG_PHYSICS_INTERPOLATION
    return m_previousPosition + m_interpolationAlpha * (m_targetPosition - m_previousPosition);
  #else
    return m_targetPosition;
  #endif
  }

  Body & Body::setPosition(const gpm::Vector2F & position)
  {
  #ifdef PG_PHYSICS_INTERPOLATION
    m_previousPosition = position;
  #endif

    m_targetPosition = position;
    return *this;
  }

  float Body::getOrientation() const
  {
  #ifdef PG_PHYSICS_INTERPOLATION
    return m_previousOrientation + m_interpolationAlpha * (m_targetOrientation - m_previousOrientation);
  #endif

    return m_targetOrientation;
  }

  Body & Body::setOrientation(const float orientation)
  {
  #ifdef PG_PHYSICS_INTERPOLATION
    m_previousOrientation = orientation;
  #endif

    m_targetOrientation = orientation;
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
    assert(m_shapeRef != nullptr);

    return m_shapeRef->getInertia(getMass());
  }

  float Body::getInverseInertia() const
  {
    return 1.f / getInertia();
  }

  bool Body::isStatic() const
  {
    return getMass() <= 0.f;
  }

  World * Body::getWorld()
  {
    return m_worldRef;
  }

  const World * Body::getWorld() const
  {
    return m_worldRef;
  }

  gpm::RectF Body::getAABB() const
  {
    assert(m_shapeRef != nullptr);

    const auto& pos = m_targetPosition;
    const auto& rot = m_targetOrientation;

    return m_shapeRef->getAABB(gpm::Matrix3x3F(
      std::cos(rot), -std::sin(rot), pos.x,
      std::sin(rot),  std::cos(rot), pos.y,
      0.f,            0.f,           1.f
    ));
  }

  Body & Body::setGravityScale(const gpm::Vector2F & gravityScale)
  {
    m_gravityScale = gravityScale;
    return *this;
  }

  Body & Body::setGravityScale(const float gravityScale)
  {
    return setGravityScale(gpm::Vector2F(gravityScale, gravityScale));
  }

  gpm::Vector2F Body::getGravityScale() const
  {
    return m_gravityScale;
  }

  Body & Body::setShape(const Shape * shape)
  {
    m_shapeRef = shape;
    return *this;
  }

  const Shape * Body::getShape() const
  {
    return m_shapeRef;
  }

  void Body::applyGravity()
  {
    applyForce(getWorld()->getGravity() * getGravityScale());
  }

  void Body::checkCollision(Body & other)
  {
    if (getShape() == nullptr || other.getShape() == nullptr) {
      return;
    }

    if (!getAABB().intersects(other.getAABB())) {
      return;
    }

    detail::collide(*getShape(), *other.getShape());
  }

  void Body::step(const float timestep)
  {
    if (isStatic() || m_shapeRef == nullptr) {
      return;
    }

    applyGravity();

  #ifdef PG_PHYSICS_INTERPOLATION
    m_previousPosition = m_targetPosition;
    m_previousOrientation = m_targetOrientation;
  #endif

    m_targetPosition = integrate(m_targetPosition, timestep, getVelocity());
    m_targetOrientation = integrate(m_targetOrientation, timestep, getAngularVelocity());
    m_velocity = integrate(getVelocity(), timestep, getInverseMass() * getTotalForce());
    m_angularVelocity = integrate(getAngularVelocity(), timestep, getInverseInertia() * getTotalTorque());

    clearForces();
  }

  void Body::interpolate(const float alpha)
  {
  #ifdef PG_PHYSICS_INTERPOLATION
    m_interpolationAlpha = alpha;
  #endif
  }
}