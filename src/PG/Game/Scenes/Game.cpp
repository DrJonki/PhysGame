#include <PG/Game/Scenes/Game.hpp>
#include <PG/Game/Entities/Debris.hpp>
#include <PG/Game/Entities/Wall.hpp>
#include <PG/Game/Randomizer.hpp>
#include <PG/Game/Engine.hpp>
#include <gpm/utility.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>

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
      /*Randomizer rand;

      for (int i = 0; i < 3; ++i) {
        auto& debris = addEntity(new DebrisEntity(m_world, gpm::Vector2F(4, 4)));

        debris.setPosition(gpm::Vector2F(
          rand(levelSize.x / 10.f, levelSize.x - levelSize.x/ 10.f),
          levelSize.y
        ));
        debris.setOrientation(rand(0.f, 2 * gpm::pi_f));
        debris.applyTorque(rand(-500.f, 500.f));
      }*/
    }

    const float e = 0.4f;

    // Create walls
    {
      /*auto& floor = addEntity(new WallEntity(m_world, gpm::Vector2F(0.f, 0.f), gpm::Vector2F(levelSize.x, 0.f)));
      floor.setElasticity(e);*/

      auto& left = addEntity(new WallEntity(m_world, gpm::Vector2F(0.f, 0.f), gpm::Vector2F(0.f, levelSize.y)));
      left.setElasticity(e);

      auto& right = addEntity(new WallEntity(m_world, gpm::Vector2F(levelSize.x, 0.f), gpm::Vector2F(levelSize.x, levelSize.y)));
      right.setElasticity(e);

      auto& ceil = addEntity(new WallEntity(m_world, gpm::Vector2F(0.f, levelSize.y), gpm::Vector2F(levelSize.x, levelSize.y)));
      right.setElasticity(e);
    }

    auto& o1 = addEntity(new DebrisEntity(m_world, gpm::Vector2F(4, 6)));
    o1.setOrientation(gpm::pi_f / 6.f);
    o1.setPosition(gpm::Vector2F(levelSize.x / 2.f - 1.f, levelSize.y - 2));
    o1.setElasticity(e);

    auto& oo1 = addEntity(new DebrisEntity(m_world, gpm::Vector2F(4, 4)));
    oo1.setOrientation(gpm::pi_f / 6.f);
    oo1.setPosition(gpm::Vector2F(levelSize.x / 2.f, levelSize.y - 48));
    oo1.setVelocity(gpm::Vector2F(1.f, 35.f));
    oo1.setElasticity(e);

    auto& o2 = addEntity(new DebrisEntity(m_world, gpm::Vector2F(4, 4)));
    o2.setPosition(gpm::Vector2F(levelSize.x / 2.f, levelSize.y - 16));
    o2.setOrientation(gpm::pi_f * 0.05f);
    o2.setMass(-1.f);
    o2.setElasticity(e);

    auto& o3 = addEntity(new DebrisEntity(m_world, gpm::Vector2F(4, 4)));
    o3.setPosition(gpm::Vector2F(levelSize.x / 1.35f, levelSize.y - 32));
    o3.setOrientation(gpm::pi_f * -0.05f);
    o3.setMass(-1.f);
    o3.setElasticity(e);
  }

  void GameScene::update(const float dt)
  {
    m_world.step(dt);

    Scene::update(dt);

    for (auto& entity : Scene::m_entities) {
      auto body = dynamic_cast<DebrisEntity*>(entity.get());

      if (body && body->getPosition().y < -15.f) {
        body->erase();
      }
    }
  }

  void GameScene::onWindowEvent(const sf::Event & event)
  {
    Scene::onWindowEvent(event);

    static Randomizer rand;

    if (event.type == sf::Event::EventType::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left) {
      const auto worldPos = Engine::getWindow().mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

      const auto size = gpm::Vector2F(rand(2.f, 8.f), rand(2.f, 8.f));

      auto& entity = addEntity(new DebrisEntity(m_world, size));
      entity.setMass(size.getMagnitude() / 5.f);
      entity.setElasticity(rand(0.2f, 0.8f));
      entity.setPosition(gpm::Vector2F(worldPos.x, -worldPos.y));
      entity.setOrientation(rand(0.f, 2 * gpm::pi_f));
      entity.setVelocity(gpm::Vector2F(rand(-4.f, 4.f), rand(10.f, 35.f)));
    }
  }
}
