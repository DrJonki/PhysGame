#include <PG/Physics/Detail/CollisionAlgorithms.hpp>
#include <PG/Physics/Shapes/Circle.hpp>
#include <PG/Physics/Shapes/LineSegment.hpp>
#include <PG/Physics/Shapes/Rectangle.hpp>
#include <PG/Physics/Body.hpp>
#include <typeindex>
#include <unordered_map>
#include <cmath>
#include <cassert>

namespace pg
{
  typedef void(*CollisionAlgorithm)(Body&, Body&);

  std::unordered_map<std::type_index, std::unordered_map<std::type_index, CollisionAlgorithm>> ns_algorithms = {

    // Circle-X
    std::make_pair(std::type_index(typeid(CircleShape)), std::unordered_map<std::type_index, CollisionAlgorithm>({

      // Circle-Circle
      std::make_pair(std::type_index(typeid(CircleShape)), [](Body& b1, Body& b2) {
        auto& s1 = static_cast<const CircleShape&>(*b1.getShape());
        auto& s2 = static_cast<const CircleShape&>(*b2.getShape());

        const auto distanceSq = (b2.getPosition() - b1.getPosition()).getMagnitudeSquared();
        const auto radSumSq = std::pow(s1.getRadius(), 2) + std::pow(s2.getRadius(), 2);

        if (distanceSq > radSumSq) {
          return;
        }


      }),

      // Circle-LineSegment
      std::make_pair(std::type_index(typeid(LineSegmentShape)), [](Body& b1, Body& b2) {

      }),

      // Circle-Rectangle
      std::make_pair(std::type_index(typeid(RectangleShape)), [](Body& b1, Body& b2) {

      }),
    })),

    // Rectangle-X
    std::make_pair(std::type_index(typeid(RectangleShape)), std::unordered_map<std::type_index, CollisionAlgorithm>({

      // Rectangle-Rectangle
      std::make_pair(std::type_index(typeid(RectangleShape)), [](Body& b1, Body& b2) {

      }),

      // Rectangle-LineSegment
      std::make_pair(std::type_index(typeid(RectangleShape)), [](Body& b1, Body& b2) {

      }),
    }))
  };

  namespace detail
  {
    void collide(Body & b1, Body & b2)
    {
      auto shapes = std::make_pair(std::type_index(typeid(b1.getShape())), std::type_index(typeid(b2.getShape())));

      // Find & call the correct algorithm
      for (int i = 0; i < 2; ++i) {
        auto itr = ns_algorithms.find(shapes.first);

        if (itr != ns_algorithms.end()) {
          auto itr2 = itr->second.find(shapes.second);

          if (itr2 != itr->second.end()) {
            return itr2->second(i == 0 ? b1 : b2, i == 0 ? b2 : b1);
          }
        }

        // Swap types & try again
        std::swap(shapes.first, shapes.second);
      }

      // No algorithm
      assert(false);
    }
  }
}