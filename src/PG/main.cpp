#include <PG/Game/Engine.hpp>
#include <PG/Game/Scenes/Game.hpp>

int main()
{
  pg::Engine engine;

  engine.pushScene(new pg::GameScene());

  return engine();
}