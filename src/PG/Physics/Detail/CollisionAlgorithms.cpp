#include <PG/Physics/Detail/CollisionAlgorithms.hpp>
#include <PG/Physics/Shapes/Circle.hpp>
#include <PG/Physics/Shapes/LineSegment.hpp>
#include <PG/Physics/Shapes/Rectangle.hpp>
#include <PG/Physics/Body.hpp>
#include <PG/Physics/CollisionInfo.hpp>
#include <typeindex>
#include <unordered_map>
#include <cmath>
#include <cassert>
#include <array>

namespace pg
{
  typedef bool(*CollisionAlgorithm)(Body&, Body&, CollisionInfo&);

  gpm::Vector2F findClosestPointOnSegment(const gpm::Vector2F& a, const gpm::Vector2F& ab, const gpm::Vector2F& p)
  {
    const auto ap = p - a;
    const auto t = ap.getDotProduct(ab) / ab.getMagnitudeSquared();

    return gpm::Vector2F(a.x + ab.x * t, a.y + ab.y * t);
  }

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

        const auto s1points = s1.getVertices(b1.getPosition(), b1.getOrientation());
        const auto s2points = s2.getVertices(b2.getPosition(), b2.getOrientation());
        std::array<gpm::Vector2F, 4> s2lines;

        for (unsigned int i = 0; i < 4; ++i) {
          const auto& a = s2points[i];
          const auto& b = s2points[(i + 1) % 4];

          s2lines[i] = b - a;
        }

        for (auto& p : s1points) {
          for (unsigned int i = 0; i < 4; ++i) {
            {
              const auto& a = s2points[i];
              const auto& ab = s2lines[i];
              const auto ap = p - a;

              if (ab.getCrossProduct(ap).z >= 0) {
                break;
              }
            }

            if (i == 3) {
              float minMagnitude = FLT_MAX;

              for (unsigned int j = 0; j < 4; ++j) {
                const auto& a = s2points[j];
                const auto& ab = s2lines[j];

                const auto closestPoint = findClosestPointOnSegment(a, ab, p);
                const auto normal = closestPoint - p;

                if (normal.getMagnitudeSquared() < minMagnitude) {
                  minMagnitude = normal.getMagnitudeSquared();
                  info.normal = normal;
                  info.point = p;
                }
              }

              info.penetrationDistance = std::sqrt(minMagnitude);
              info.normal /= info.penetrationDistance;
              info.bodyA = &b1;
              info.bodyB = &b2;

              return true;
            }
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