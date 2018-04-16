#pragma once

namespace sf
{
  class RenderTarget;
}

namespace pg
{
  class Entity
  {
    friend class Scene;

  public:

    Entity();

    virtual ~Entity() = 0;

    Entity& erase();

    bool isErased() const;

  protected:

    virtual void update(const float dt);

    virtual void draw(sf::RenderTarget& target);

  private:

    bool m_erased;
  };
}