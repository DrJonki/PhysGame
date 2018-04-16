#pragma once

#include <gpm/vector.hpp>
#include <gpm/rectangle.hpp>

namespace pg
{
  class Shape;
  class World;

  class Body
  {
    friend class World;

  public:

    Body(const float mass);

    /// \brief Body constructor
    ///
    /// \param mass Mass of this body. Pass <=0 to create a static body.
    /// \param shape The collision shape to use.
    ///
    Body(const float mass, const Shape& shape);

    ~Body();

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

    bool isStatic() const;

    World* getWorld();

    const World* getWorld() const;

    gpm::RectF getAABB() const;

    Body& setGravityScale(const gpm::Vector2F& gravityScale);

    Body& setGravityScale(const float gravityScale);

    gpm::Vector2F getGravityScale() const;

    Body& setShape(const Shape* shape);

    const Shape* getShape() const;

  private:

    void applyGravity();

    void checkCollision(const Body& other);

    void step(const float timestep);

    void interpolate(const float alpha);

  private:

    const Shape* m_shapeRef;
    World* m_worldRef;

  #ifdef PG_PHYSICS_INTERPOLATION
    gpm::Vector2F m_previousPosition;
    float m_previousOrientation;
    float m_interpolationAlpha;
  #endif

    gpm::Vector2F m_targetPosition;
    float m_targetOrientation;

    float m_mass;
    gpm::Vector2F m_gravityScale;
    gpm::Vector2F m_force;
    gpm::Vector2F m_velocity;
    float m_angularVelocity;
    float m_torque;
    float m_elasticity;
  };
}
