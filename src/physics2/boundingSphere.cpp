#include "boundingSphere.h"

#include "../core/math3d.h"

BoundingSphere::BoundingSphere(const Vector3f& center, const double& radius)
    : m_center(center), m_radius(radius) {}

// member functions
const Vector3f& BoundingSphere::getCenter() const { return m_center; }
float BoundingSphere::getRadius() const { return m_radius; }
// checks intersections
IntersectData BoundingSphere::intersectBoundingSphere(
    const BoundingSphere& sphere) const {
    float dV{(m_center - sphere.getCenter()).Length()};
    float radius_distance{m_radius + sphere.getRadius()};

    // the = is when the two spheres are touching
    if (dV <= radius_distance) {
        return IntersectData(true, dV - radius_distance);
    } else {
        return IntersectData(false, dV - radius_distance);
    }
}
