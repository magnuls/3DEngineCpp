#include <gtest/gtest.h>

#include <iomanip>

#include "../src/physics2/AABB.h"
#include "../src/physics2/boundingSphere.h"
#include "../src/physics2/collisionDispatch.h"
#include "../src/physics2/intersectData.h"
#include "../src/physics2/plane.h"

using namespace Physics;

TEST(PhysicsTest, BoundingSphere) {
    BoundingSphere sphere1(Vector3f(0, 0, 0), 1);  // red
    BoundingSphere sphere2(Vector3f(1, 1, 1), 1);  // blue
    BoundingSphere sphere3(Vector3f(4, 3, 2), 1);  // purple
    BoundingSphere sphere4(Vector3f(0, 0, 2), 1);  // white
                                                   //
                                                   //
    IntersectData s1_s2{collision(sphere1, sphere2)};
    IntersectData s2_s1{collision(sphere2, sphere1)};
    IntersectData s3_s4{collision(sphere3, sphere4)};
    IntersectData s4_s1{collision(sphere4, sphere1)};

    EXPECT_TRUE(s1_s2.m_doesIntersect);
    EXPECT_TRUE(s2_s1.m_doesIntersect);
    EXPECT_FALSE(s3_s4.m_doesIntersect);
    EXPECT_TRUE(s4_s1.m_doesIntersect);
}

int nthdigit(float& digit, const int n) {
    return std::trunc(std::pow(10, n) * digit);
}

TEST(PhysicsTest, AxisAlignedBoundingBox) {
    AABB box0(Vector3f(0, 0, 0), Vector3f(1, 1, 1));
    // This test below will throw a runtime error since second vector has an
    // attribute < first
    //  AABB box1(Vector3f(4, 4, 4), Vector3f(5, 8, 0));
    AABB box2(Vector3f(4, 2, 3), Vector3f(7, 6, 5));
    AABB box3(Vector3f(-399, -493, 233), Vector3f(9, -9, 300));
    AABB box4(Vector3f(0, 0, 0), Vector3f(5, 5, 5));
    AABB box5(Vector3f(3, 2, 4), Vector3f(8, 7, 9));
    AABB box6(Vector3f(10, 10, 10), Vector3f(15, 15, 15));
    AABB box7(Vector3f(0, 0, 0), Vector3f(3, 3, 3));

    // IntersectData b0_b1{collision(box0, box1)};
    IntersectData b0_b2{collision(box0, box2)};
    IntersectData b2_b3{collision(box2, box3)};
    // IntersectData b3_b0{collision(box3, box0)};
    IntersectData b4_b5{collision(box4, box5)};
    IntersectData b6_b7{collision(box6, box7)};

    EXPECT_FALSE(b6_b7.m_doesIntersect);
    EXPECT_NEAR(b6_b7.distance, 12.12, 0.01f);
    EXPECT_FALSE(b0_b2.m_doesIntersect);
    EXPECT_FALSE(b2_b3.m_doesIntersect);
    EXPECT_TRUE(b4_b5.m_doesIntersect);
    EXPECT_NEAR(b0_b2.distance, 3.74, 0.01f);
    EXPECT_NEAR(b2_b3.distance, 228.31, 0.01f);
}

TEST(PhysicsTest, AABB_vs_BS) {
    AABB box(Vector3f(0, 0, 0), Vector3f(5, 5, 5));

    // Sphere center inside the box
    BoundingSphere s_inside(Vector3f(2.5, 2.5, 2.5), 0.5);
    IntersectData r1{collision(box, s_inside)};
    EXPECT_TRUE(r1.m_doesIntersect);
    EXPECT_NEAR(r1.distance, 0.0, 0.01f);

    // Sphere overlaps a face
    BoundingSphere s_face(Vector3f(6, 2.5, 2.5), 1.5);
    IntersectData r2{collision(box, s_face)};
    EXPECT_TRUE(r2.m_doesIntersect);
    EXPECT_NEAR(r2.distance, 1.0, 0.01f);

    BoundingSphere s_tangent(Vector3f(6, 2.5, 2.5), 1.0);
    IntersectData r3{collision(box, s_tangent)};
    EXPECT_TRUE(r3.m_doesIntersect);
    EXPECT_NEAR(r3.distance, 1.0, 0.01f);

    // Sphere near a corner intersecting
    BoundingSphere s_corner_hit(Vector3f(5.5, 5.5, 5.5), 1.0);
    IntersectData r4{collision(box, s_corner_hit)};
    EXPECT_TRUE(r4.m_doesIntersect);
    EXPECT_NEAR(r4.distance, 0.87, 0.01f);

    // Sphere near a corner  missing
    BoundingSphere s_corner_miss(Vector3f(6, 6, 6), 1.0);
    IntersectData r5{collision(box, s_corner_miss)};
    EXPECT_FALSE(r5.m_doesIntersect);
    EXPECT_NEAR(r5.distance, 1.73, 0.01f);

    // Sphere near an edge missing
    BoundingSphere s_edge_miss(Vector3f(6, 6, 2.5), 1.0);
    IntersectData r6{collision(box, s_edge_miss)};
    EXPECT_FALSE(r6.m_doesIntersect);
    EXPECT_NEAR(r6.distance, 1.41, 0.01f);

    // Sphere far away
    BoundingSphere s_far(Vector3f(10, 10, 10), 1.0);
    IntersectData r7{collision(box, s_far)};
    EXPECT_FALSE(r7.m_doesIntersect);
    EXPECT_NEAR(r7.distance, 8.66, 0.01f);

    IntersectData r8{collision(s_face, box)};
    EXPECT_TRUE(r8.m_doesIntersect);
    EXPECT_NEAR(r8.distance, 1.0, 0.01f);

    IntersectData r9{collision(s_corner_miss, box)};
    EXPECT_FALSE(r9.m_doesIntersect);
    EXPECT_NEAR(r9.distance, 1.73, 0.01f);

    // Sphere approaching from neg side
    AABB box2(Vector3f(-2, -2, -2), Vector3f(2, 2, 2));
    BoundingSphere s_neg(Vector3f(-3, 0, 0), 1.5);
    IntersectData r10{collision(box2, s_neg)};
    EXPECT_TRUE(r10.m_doesIntersect);
    EXPECT_NEAR(r10.distance, 1.0, 0.01f);
}
TEST(PhysicsTest, Plane_Collision_Tests) {
    // 1. Box well above axis-aligned floor, no intersection
    Plane floor(Vector3f(0, 1, 0), 0);
    AABB box_above(Vector3f(0, 5, 0), Vector3f(2, 7, 2));
    // center=(1,6,1), half=(1,1,1), r=1, dist=6
    IntersectData r1{collision(floor, box_above)};
    EXPECT_FALSE(r1.m_doesIntersect);
    EXPECT_NEAR(r1.distance, 6.0, 0.01f);

    // 2. Box straddling plane, center on plane
    AABB box_straddle(Vector3f(-1, -1, -1), Vector3f(1, 1, 1));
    // center=(0,0,0), r=1, dist=0
    IntersectData r2{collision(floor, box_straddle)};
    EXPECT_TRUE(r2.m_doesIntersect);
    EXPECT_NEAR(r2.distance, 0.0, 0.01f);

    // 3. Box tangent — bottom face just touches plane
    AABB box_tangent(Vector3f(-1, 0, -1), Vector3f(1, 2, 1));
    // center=(0,1,0), r=1, dist=1, 1<=1
    IntersectData r3{collision(floor, box_tangent)};
    EXPECT_TRUE(r3.m_doesIntersect);
    EXPECT_NEAR(r3.distance, 1.0, 0.01f);

    // 4. Near miss — bottom face just above plane
    AABB box_near_miss(Vector3f(-1, 0.1, -1), Vector3f(1, 2.1, 1));
    // center=(0,1.1,0), r=1, dist=1.1, 1.1>1
    IntersectData r4{collision(floor, box_near_miss)};
    EXPECT_FALSE(r4.m_doesIntersect);
    EXPECT_NEAR(r4.distance, 1.1, 0.01f);

    // 5. Diagonal plane through origin, box at origin
    Plane diag(Vector3f(1, 1, 1), 0);
    // normalized norm = (1/√3, 1/√3, 1/√3)
    // r = 3/√3 = √3 ≈ 1.73, dist = 0
    IntersectData r5{collision(diag, box_straddle)};
    EXPECT_TRUE(r5.m_doesIntersect);
    EXPECT_NEAR(r5.distance, 0.0, 0.01f);

    // 6. Diagonal plane, box far away
    AABB box_far(Vector3f(5, 5, 5), Vector3f(7, 7, 7));
    // center=(6,6,6), r=√3≈1.73, dist=6√3≈10.39
    IntersectData r6{collision(diag, box_far)};
    EXPECT_FALSE(r6.m_doesIntersect);
    EXPECT_NEAR(r6.distance, 10.39, 0.01f);

    // 7. Flipped normal — same plane, same result
    Plane floor_flipped(Vector3f(0, -1, 0), 0);
    IntersectData r7{collision(floor_flipped, box_straddle)};
    EXPECT_TRUE(r7.m_doesIntersect);
    EXPECT_NEAR(r7.distance, 0.0, 0.01f);

    // 8. Box below an elevated plane
    Plane elevated(Vector3f(0, 1, 0), 10);
    AABB box_below(Vector3f(0, 0, 0), Vector3f(2, 2, 2));
    // center=(1,1,1), r=1, dist=|1-10|=9
    IntersectData r8{collision(elevated, box_below)};
    EXPECT_FALSE(r8.m_doesIntersect);
    EXPECT_NEAR(r8.distance, 9.0, 0.01f);

    // 9. Non-unit normal — tests normalization
    Plane scaled_floor(Vector3f(0, 3, 0), 0);
    IntersectData r9{collision(scaled_floor, box_tangent)};
    EXPECT_TRUE(r9.m_doesIntersect);
    EXPECT_NEAR(r9.distance, 1.0, 0.01f);

    // 10. Both orderings
    IntersectData r10{collision(box_straddle, floor)};
    EXPECT_TRUE(r10.m_doesIntersect);
    EXPECT_NEAR(r10.distance, 0.0, 0.01f);

    IntersectData r11{collision(box_far, diag)};
    EXPECT_FALSE(r11.m_doesIntersect);
    EXPECT_NEAR(r11.distance, 10.39, 0.01f);
}

TEST(PhysicsTest, Plane_vs_BoundingSphere) {
    // 1. Sphere well above axis-aligned floor — miss
    Plane floor(Vector3f(0, 1, 0), 0);
    BoundingSphere s_above(Vector3f(0, 5, 0), 1.0);
    IntersectData r1{collision(floor, s_above)};
    EXPECT_FALSE(r1.m_doesIntersect);
    EXPECT_NEAR(r1.distance, 5.0, 0.01f);

    // 2. Sphere partially through floor — hit
    BoundingSphere s_partial(Vector3f(0, 0.5, 0), 1.0);
    IntersectData r2{collision(floor, s_partial)};
    EXPECT_TRUE(r2.m_doesIntersect);
    EXPECT_NEAR(r2.distance, 0.5, 0.01f);

    // 3. Sphere tangent to floor — hit
    BoundingSphere s_tangent(Vector3f(0, 1, 0), 1.0);
    IntersectData r3{collision(floor, s_tangent)};
    EXPECT_TRUE(r3.m_doesIntersect);
    EXPECT_NEAR(r3.distance, 1.0, 0.01f);

    // 4. Near miss — just above
    BoundingSphere s_near(Vector3f(0, 1.5, 0), 1.0);
    IntersectData r4{collision(floor, s_near)};
    EXPECT_FALSE(r4.m_doesIntersect);
    EXPECT_NEAR(r4.distance, 1.5, 0.01f);

    // 5. Center on plane — hit, distance 0
    BoundingSphere s_on(Vector3f(3, 0, 3), 2.0);
    IntersectData r5{collision(floor, s_on)};
    EXPECT_TRUE(r5.m_doesIntersect);
    EXPECT_NEAR(r5.distance, 0.0, 0.01f);

    // 6. Sphere below plane — miss
    BoundingSphere s_below(Vector3f(0, -3, 0), 1.0);
    IntersectData r6{collision(floor, s_below)};
    EXPECT_FALSE(r6.m_doesIntersect);
    EXPECT_NEAR(r6.distance, 3.0, 0.01f);

    // 7. Sphere from negative side — hit
    BoundingSphere s_neg_hit(Vector3f(0, -0.5, 0), 1.0);
    IntersectData r7{collision(floor, s_neg_hit)};
    EXPECT_TRUE(r7.m_doesIntersect);
    EXPECT_NEAR(r7.distance, 0.5, 0.01f);

    // 8. Elevated plane, sphere below it — miss
    Plane elevated(Vector3f(0, 1, 0), 10);
    BoundingSphere s_low(Vector3f(0, 8, 0), 1.0);
    IntersectData r8{collision(elevated, s_low)};
    EXPECT_FALSE(r8.m_doesIntersect);
    EXPECT_NEAR(r8.distance, 2.0, 0.01f);

    // 9. Non-unit normal — tests normalization
    Plane scaled(Vector3f(0, 5, 0), 0);
    IntersectData r9{collision(scaled, s_tangent)};
    EXPECT_TRUE(r9.m_doesIntersect);
    EXPECT_NEAR(r9.distance, 1.0, 0.01f);

    // 10. Both orderings
    IntersectData r10{collision(s_above, floor)};
    EXPECT_FALSE(r10.m_doesIntersect);
    EXPECT_NEAR(r10.distance, 5.0, 0.01f);

    IntersectData r11{collision(s_partial, floor)};
    EXPECT_TRUE(r11.m_doesIntersect);
    EXPECT_NEAR(r11.distance, 0.5, 0.01f);
}

TEST(PhysicsTest, Plane_vs_Plane) {
    // 1. Two parallel planes, same normal — distance 5
    Plane p1(Vector3f(0, 1, 0), 0);
    Plane p2(Vector3f(0, 1, 0), 5);
    IntersectData r1{collision(p1, p2)};
    EXPECT_FALSE(r1.m_doesIntersect);
    EXPECT_NEAR(r1.distance, 5.0, 0.01f);

    // 2. Same plane — distance 0
    Plane p3(Vector3f(0, 1, 0), 3);
    Plane p4(Vector3f(0, 1, 0), 3);
    IntersectData r2{collision(p3, p4)};
    EXPECT_FALSE(r2.m_doesIntersect);
    EXPECT_NEAR(r2.distance, 0.0, 0.01f);

    // 3. Perpendicular planes — intersect
    Plane p5(Vector3f(0, 1, 0), 0);
    Plane p6(Vector3f(1, 0, 0), 0);
    IntersectData r3{collision(p5, p6)};
    EXPECT_TRUE(r3.m_doesIntersect);
    EXPECT_NEAR(r3.distance, 0.0, 0.01f);

    // 4. Angled planes — intersect
    Plane p7(Vector3f(0, 1, 0), 0);
    Plane p8(Vector3f(1, 1, 0), 0);
    IntersectData r4{collision(p7, p8)};
    EXPECT_TRUE(r4.m_doesIntersect);
    EXPECT_NEAR(r4.distance, 0.0, 0.01f);

    // 5. Non-unit parallel normals — tests normalization
    Plane p9(Vector3f(0, 3, 0), 0);
    Plane p10(Vector3f(0, 7, 0), 14);
    // After normalization: (0,1,0) d=0 and (0,1,0) d=2
    IntersectData r5{collision(p9, p10)};
    EXPECT_FALSE(r5.m_doesIntersect);
    EXPECT_NEAR(r5.distance, 2.0, 0.01f);

    // 6. Parallel along diagonal
    Plane p11(Vector3f(1, 1, 1), 0);
    Plane p12(Vector3f(1, 1, 1), 3);
    // After normalization: d1=0, d2=3/√3=√3≈1.73
    IntersectData r6{collision(p11, p12)};
    EXPECT_FALSE(r6.m_doesIntersect);
    EXPECT_NEAR(r6.distance, 1.73, 0.01f);

    // 7. Both orderings
    IntersectData r7{collision(p2, p1)};
    EXPECT_FALSE(r7.m_doesIntersect);
    EXPECT_NEAR(r7.distance, 5.0, 0.01f);

    IntersectData r8{collision(p6, p5)};
    EXPECT_TRUE(r8.m_doesIntersect);
    EXPECT_NEAR(r8.distance, 0.0, 0.01f);
}

TEST(PhysicsTest, Plane_vs_AABB) {
    // 1. Box well above axis-aligned floor — miss
    Plane floor(Vector3f(0, 1, 0), 0);
    AABB box_above(Vector3f(0, 5, 0), Vector3f(2, 7, 2));
    // center=(1,6,1), half=(1,1,1), r=1, dist=6
    IntersectData r1{collision(floor, box_above)};
    EXPECT_FALSE(r1.m_doesIntersect);
    EXPECT_NEAR(r1.distance, 6.0, 0.01f);

    // 2. Box straddling plane — center on plane
    AABB box_straddle(Vector3f(-1, -1, -1), Vector3f(1, 1, 1));
    // center=(0,0,0), r=1, dist=0
    IntersectData r2{collision(floor, box_straddle)};
    EXPECT_TRUE(r2.m_doesIntersect);
    EXPECT_NEAR(r2.distance, 0.0, 0.01f);

    // 3. Box tangent — bottom face just touches
    AABB box_tangent(Vector3f(-1, 0, -1), Vector3f(1, 2, 1));
    // center=(0,1,0), r=1, dist=1, 1<=1
    IntersectData r3{collision(floor, box_tangent)};
    EXPECT_TRUE(r3.m_doesIntersect);
    EXPECT_NEAR(r3.distance, 1.0, 0.01f);

    // 4. Near miss — bottom face just above
    AABB box_near_miss(Vector3f(-1, 1.1, -1), Vector3f(1, 3.1, 1));
    // center=(0,2.1,0), r=1, dist=2.1
    IntersectData r4{collision(floor, box_near_miss)};
    EXPECT_FALSE(r4.m_doesIntersect);
    EXPECT_NEAR(r4.distance, 2.1, 0.01f);
}

// 5. Diagonal plane through origin,
/*
Type of Assertions
 EXPECT_EQ(a, b);      // a == b
 EXPECT_NE(a, b);      // a != b
 EXPECT_TRUE(expr);    // expr is true
 EXPECT_FALSE(expr);   // expr is false
 EXPECT_LT(a, b);      // a < b
 EXPECT_GT(a, b);      // a > b
 EXPECT_THROW(expr, ExceptionType);  // expr throws that exception
*/
