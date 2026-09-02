#include "collisionDispatch.h"

#include <ranges>

#include "../src/core/math3d.h"
#include "AABB.h"
#include "boundingSphere.h"
#include "intersectData.h"
#include "plane.h"

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

template <>
IntersectData Physics::collision<AABB, BoundingSphere>(
    const AABB& A, const BoundingSphere& B) {
    Vector3f closest(
        std::clamp(B.getCenter().GetX(), A.getMin().GetX(), A.getMax().GetX()),
        std::clamp(B.getCenter().GetY(), A.getMin().GetY(), A.getMax().GetY()),
        std::clamp(B.getCenter().GetZ(), A.getMin().GetZ(), A.getMax().GetZ()));

    Vector3f diff{closest - B.getCenter()};
    float length{diff.Length()};
    // Condition for intersection
    IntersectData inter(length <= B.getRadius(), length);

    return inter;
}

template <>
IntersectData Physics::collision<BoundingSphere, AABB>(const BoundingSphere& B,
                                                       const AABB& A) {
    return Physics::collision(A, B);
}

template <>
IntersectData Physics::collision<BoundingSphere, Plane>(const BoundingSphere& B,
                                                        const Plane& P) {
    Plane normalized{P.normalize()};
    float dist{std::abs(normalized.getNorm().Dot(B.getCenter()) -
                        normalized.getScaler())};

    IntersectData inter(dist <= B.getRadius(), dist);
    return inter;
}

template <>
IntersectData Physics::collision<Plane, BoundingSphere>(
    const Plane& P, const BoundingSphere& B) {
    return collision(B, P);
}

template <>
IntersectData Physics::collision<Plane, Plane>(const Plane& P1,
                                               const Plane& P2) {
    Plane n1{P1.normalize()}, n2{P2.normalize()};
    IntersectData inter(true, 0);
    if (n1.getNorm().Cross(n2.getNorm()).Length() < 1e-6f) {
        inter.m_doesIntersect = false;
        inter.distance = std::abs(n1.getScaler() - n2.getScaler());
    }
    return inter;
}

template <>
IntersectData Physics::collision<Plane, AABB>(const Plane& P, const AABB& A) {
    // Normalized norm lmao
    Plane normalized{P.normalize()};
    Vector3f norm_norm{normalized.getNorm()};
    Vector3f A_center{(A.getMax() + A.getMin()) / 2};
    float radius{((A.getMax().GetX() - A.getMin().GetX()) / 2) *
                     std::abs(norm_norm.GetX()) +
                 ((A.getMax().GetY() - A.getMin().GetY()) / 2) *
                     std::abs(norm_norm.GetY()) +
                 ((A.getMax().GetZ() - A.getMin().GetZ()) / 2) *
                     std::abs(norm_norm.GetZ())};
    float distance{std::abs(norm_norm.Dot(A_center) - normalized.getScaler())};
    IntersectData inter(distance <= radius, distance);
    return inter;
}

template <>
IntersectData Physics::collision<AABB, Plane>(const AABB& A, const Plane& P) {
    return collision(P, A);
}
