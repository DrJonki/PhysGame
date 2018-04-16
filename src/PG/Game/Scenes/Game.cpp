#include <PG/Game/Scenes/Game.hpp>
#include <PG/Game/Entities/Debris.hpp>
#include <PG/Game/Randomizer.hpp>
#include <PG/Game/Engine.hpp>
#include <gpm/utility.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace pg
{
  GameScene::GameScene()
    : Scene()
  {
    const auto levelWidth = 40.f;

    auto& window = Engine::getWindow();
    const auto windowSize = sf::Vector2f(window.getSize());
    const auto ratio = windowSize.x / windowSize.y;
    const auto levelSize = sf::Vector2f(levelWidth, levelWidth / ratio);

    window.setView(sf::View(
      sf::Vector2f(levelSize.x / 2.f, -levelSize.y / 2.f),
      levelSize
    ));

    // Generate debris
    {
      Randomizer rand;

      for (int i = 0; i < 3; ++i) {
        auto& debris = addEntity(new DebrisEntity(m_world, gpm::Vector2F(4, 4)));

        debris.setPosition(gpm::Vector2F(
          rand(levelSize.x / 10.f, levelSize.x - levelSize.x/ 10.f),
          levelSize.y
        ));
        debris.setOrientation(rand(0.f, 2 * gpm::pi_f));
        debris.applyTorque(rand(-500.f, 500.f));
      }
    }
  }

  void GameScene::update(const float dt)
  {
    m_world.step(dt);

    Scene::update(dt);
  }
}
