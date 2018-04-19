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

  bool segmentsIntersect(const gpm::Vector2F& p1, const gpm::Vector2F& q1, const gpm::Vector2F& p2, const gpm::Vector2F& q2)
  {
    auto onSegment = [](const gpm::Vector2F& p, const gpm::Vector2F& q, const gpm::Vector2F& r)
    {
      return q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) && q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y);
    };
    auto orientation = [](const gpm::Vector2F& p, const gpm::Vector2F& q, const gpm::Vector2F& r)
    {
      const int val = static_cast<int>((q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y));

      if (val == 0) return 0;

      return (val > 0) ? 1 : 2;
    };

    const auto o1 = orientation(p1, q1, p2);
    const auto o2 = orientation(p1, q1, q2);
    const auto o3 = orientation(p2, q2, p1);
    const auto o4 = orientation(p2, q2, q1);

    if (o1 != o2 && o3 != o4)
      return true;

    return (o1 == 0 && onSegment(p1, p2, q1))
        || (o2 == 0 && onSegment(p1, q2, q1))
        || (o3 == 0 && onSegment(p2, p1, q2))
        || (o4 == 0 && onSegment(p2, q1, q2));
  }

  std::unordered_map<std::type_index, std::unordered_map<std::type_index, CollisionAlgorithm>> ns_algorithms = {

    // Circle-X
    std::make_pair(std::type_index(typeid(CircleShape)), std::unordered_map<std::type_index, CollisionAlgorithm>({

      // Circle-Circle
      std::make_pair(std::type_index(typeid(CircleShape)), [](Body& b1, Body& b2, CollisionInfo& info)
      {
        /*auto& s1 = static_cast<const CircleShape&>(*b1.getShape());
        auto& s2 = static_cast<const CircleShape&>(*b2.getShape());

        const auto distanceSq = (b2.getPosition() - b1.getPosition()).getMagnitudeSquared();
        const auto radSumSq = std::pow(s1.getRadius(), 2) + std::pow(s2.getRadius(), 2);

        if (distanceSq > radSumSq) {
          return false;
        }*/

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
              for (unsigned int j = 0; j < 4; ++j) {
                const auto& a = s2points[j];
                const auto& ab = s2lines[j];

                if (segmentsIntersect(a, a + ab, b1.getPosition(), b2.getPosition())) {
                  const auto closestPoint = findClosestPointOnSegment(a, ab, p);

                  info.normal = closestPoint - p;
                  info.penetrationDistance = std::sqrt(info.normal.getMagnitudeSquared());
                  info.point = closestPoint;

                  break;
                }
              }

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
      std::make_pair(std::type_index(typeid(LineSegmentShape)), [](Body& b1, Body& b2, CollisionInfo& info)
      {
        auto& s1 = static_cast<const RectangleShape&>(*b1.getShape());
        auto& s2 = static_cast<const LineSegmentShape&>(*b2.getShape());

        const auto s1points = s1.getVertices(b1.getPosition(), b1.getOrientation());
        const auto s2points = s2.getVertices(b2.getPosition(), b2.getOrientation());
        const auto lineStart = s2points[0];
        const auto lineEnd = s2points[1];

        for (int i = 0; i < 4; ++i) {
          const auto& a = s1points[i];
          const auto& b = s1points[(i + 1) % 4];
          const auto& c = s1points[(i + 2) % 4];

          if (segmentsIntersect(a, b, lineStart, lineEnd) && segmentsIntersect(b, c, lineStart, lineEnd)) {
            const auto closestPoint = findClosestPointOnSegment(lineStart, lineEnd - lineStart, b);

            info.normal = closestPoint - b;
            info.penetrationDistance = std::sqrt(info.normal.getMagnitudeSquared());
            info.normal.normalize();
            info.point = closestPoint;
            info.bodyA = &b1;
            info.bodyB = &b2;

            return true;
          }
        }

        for (int i = 0; i < 4; ++i) {
          const auto& a = s1points[i];
          const auto& b = s1points[(i + 1) % 4];
          const auto& c = s1points[(i + 2) % 4];

          if (segmentsIntersect(a, b, lineStart, lineEnd) || segmentsIntersect(b, c, lineStart, lineEnd)) {
            const float epsilon = 0.99f;

            if (std::abs((b - a).getDotProduct(lineEnd - lineStart) >= epsilon || std::abs((b - c).getDotProduct(lineEnd - lineStart) >= epsilon))) {
              const auto closestPoint = findClosestPointOnSegment(lineStart, lineEnd - lineStart, b1.getPosition());

              info.normal = b1.getPosition() - closestPoint;
              info.penetrationDistance = 0.01f;
              info.normal.normalize();
              info.point = closestPoint;
              info.bodyA = &b1;
              info.bodyB = &b2;

              return true;
            }
          }
        }

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