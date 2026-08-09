#include "collisionDispatch.h"

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
