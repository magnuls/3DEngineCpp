#include "collisionDispatch.h"

#include <ranges>

#include "AABB.h"
#include "boundingSphere.h"
#include "intersectData.h"

using namespace Physics;

template <>
IntersectData Physics::collision<BoundingSphere, BoundingSphere>(
    const BoundingSphere& A, const BoundingSphere& B) {
    float dV{(A.getCenter() - B.getCenter()).Length()};
    float radius_distance{A.getRadius() + B.getRadius()};

    // the = is when the two spheres are touching
    if (dV <= radius_distance) {
        return IntersectData(true, dV - radius_distance);
    } else {
        return IntersectData(false, dV - radius_distance);
    }
}

// Overlap iff they overlap on every axis, x,y,z:
template <>
IntersectData Physics::collision<AABB, AABB>(const AABB& A, const AABB& B) {
    Vector3f d1{B.getMin() - A.getMax()};
    Vector3f d2{A.getMin() - B.getMax()};
    Vector3f max{d1.Max(d2)};
    if (std::ranges::all_of(max.cbegin(), max.cend(),
                            [&](const float& numb) { return numb <= 0; })) {
        return IntersectData(true, max.Length());
    }
    return IntersectData(false, max.Length());
}
