#include <PG/Game/Randomizer.hpp>

namespace pg
{
  Randomizer::Randomizer()
    : m_randomGenerator(std::random_device()())
  {}

  Randomizer::Randomizer(const std::default_random_engine::result_type seed)
    : m_randomGenerator(seed)
  {}
}
