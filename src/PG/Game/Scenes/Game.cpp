#include <PG/Game/Scenes/Game.hpp>
#include <PG/Game/Entities/Debris.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace pg
{
  GameScene::GameScene()
    : Scene()
  {
    addEntity(new DebrisEntity(m_world));
  }

  void GameScene::update(const float dt)
  {
    m_world.step(dt);

    Scene::update(dt);
  }
}
