#pragma once

#include <PG/Game/Scene.hpp>
#include <PG/Physics/World.hpp>

namespace pg
{
  class GameScene : public Scene
  {
  public:

    GameScene();

  private:

    void update(const float dt) override;

    void onWindowEvent(const sf::Event& event) override;

  private:

    World m_world;
  };
}
