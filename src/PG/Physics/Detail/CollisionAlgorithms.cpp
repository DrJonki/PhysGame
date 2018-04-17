#include <PG/Physics/Detail/CollisionAlgorithms.hpp>
#include <PG/Physics/Shapes/Circle.hpp>
#include <PG/Physics/Shapes/LineSegment.hpp>
#include <PG/Physics/Shapes/Rectangle.hpp>
#include <PG/Physics/Body.hpp>
#include <typeindex>
#include <unordered_map>
#include <cmath>
#include <cassert>
#include <array>

namespace pg
{
  typedef bool(*CollisionAlgorithm)(Body&, Body&, CollisionInfo&);

  std::unordered_map<std::type_index, std::unordered_map<std::type_index, CollisionAlgorithm>> ns_algorithms = {

    // Circle-X
    std::make_pair(std::type_index(typeid(CircleShape)), std::unordered_map<std::type_index, CollisionAlgorithm>({

      // Circle-Circle
      std::make_pair(std::type_index(typeid(CircleShape)), [](Body& b1, Body& b2, CollisionInfo& info)
      {
        auto& s1 = static_cast<const CircleShape&>(*b1.getShape());
        auto& s2 = static_cast<const CircleShape&>(*b2.getShape());

        const auto distanceSq = (b2.getPosition() - b1.getPosition()).getMagnitudeSquared();
        const auto radSumSq = std::pow(s1.getRadius(), 2) + std::pow(s2.getRadius(), 2);

        if (distanceSq > radSumSq) {
          return false;
        }

        return false;
      }),

      // Circle-LineSegment
      std::make_pair(std::type_index(typeid(LineSegmentShape)), [](Body& b1, Body& b2, CollisionInfo& info)
      {
        // TODO: implement
        return false;
      }),

      // Circle-Rectangle
      std::make_pair(std::type_index(typeid(RectangleShape)), [](Body& b1, Body& b2, CollisionInfo& info)
      {
        // TODO: implement
        return false;
      }),
    })),

    // Rectangle-X
    std::make_pair(std::type_index(typeid(RectangleShape)), std::unordered_map<std::type_index, CollisionAlgorithm>({

      // Rectangle-Rectangle
      std::make_pair(std::type_index(typeid(RectangleShape)), [](Body& b1, Body& b2, CollisionInfo& info)
      {
        auto& s1 = static_cast<const RectangleShape&>(*b1.getShape());
        auto& s2 = static_cast<const RectangleShape&>(*b2.getShape());

        const auto s1points = s1.getVertices(b1.getTransform());
        const auto s2points = s2.getVertices(b2.getTransform());

        for (unsigned int i = 0; i < s2points.size(); ++i) {
          const auto& a = s2points[i];
          const auto ab = s2points[i == (s2points.size() - 1) ? 0 : i + 1] - a;

          for (auto& p : s1points) {
            if (ab.getCrossProduct(p - a).z >= 0) {
              return false;
            }

            // p is inside 
          }
        }

        return false;
      }),

      // Rectangle-LineSegment
      std::make_pair(std::type_index(typeid(RectangleShape)), [](Body& b1, Body& b2, CollisionInfo& info)
      {
        return false;
      }),
    }))
  };

  namespace detail
  {
    bool collide(Body & b1, Body & b2, CollisionInfo& info)
    {
      auto shapes = std::make_pair(std::type_index(typeid(*b1.getShape())), std::type_index(typeid(*b2.getShape())));

      // Find & call the correct algorithm
      for (int i = 0; i < 2; ++i) {
        auto itr = ns_algorithms.find(shapes.first);

        if (itr != ns_algorithms.end()) {
          auto itr2 = itr->second.find(shapes.second);

          if (itr2 != itr->second.end()) {
            return itr2->second(i == 0 ? b1 : b2, i == 0 ? b2 : b1, info);
          }
        }

        // Swap types & try again
        shapes = std::make_pair(shapes.second, shapes.first);
      }

      // No algorithm
      return false;
    }
  }
}