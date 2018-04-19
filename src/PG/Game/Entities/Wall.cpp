#include <PG/Game/Entities/Wall.hpp>
#include <PG/Physics/World.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Vertex.hpp>

namespace pg
{
  WallEntity::WallEntity(World& world, const gpm::Vector2F & start, const gpm::Vector2F & end)
    : Entity()
    , Body(-1.f)
    , m_shape(start, end)
  {
    setShape(&m_shape);
    world.addBody(*this);
  }

  void WallEntity::draw(sf::RenderTarget & target)
  {
    const auto verts = m_shape.getVertices(getPosition(), getOrientation());

    const sf::Vertex vertices[] = {
      sf::Vertex(sf::Vector2f(verts[0].x, -verts[0].y)),
      sf::Vertex(sf::Vector2f(verts[1].x, -verts[1].y))
    };

    target.draw(vertices, 2u, sf::PrimitiveType::Lines);
  }
}
