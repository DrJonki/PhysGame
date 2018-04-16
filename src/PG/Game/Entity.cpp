#include <PG/Game/Entity.hpp>

namespace pg
{
  Entity::Entity()
    : m_erased(false)
  {}

  Entity::~Entity()
  {}

  Entity & Entity::erase()
  {
    m_erased = true;
    return *this;
  }

  bool Entity::isErased() const
  {
    return m_erased;
  }

  void Entity::update(const float)
  {}

  void Entity::draw(sf::RenderTarget&)
  {}
}
