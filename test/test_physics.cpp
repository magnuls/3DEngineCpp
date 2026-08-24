#include <gtest/gtest.h>

#include <iomanip>

#include "../src/physics2/AABB.h"
#include "../src/physics2/boundingSphere.h"
#include "../src/physics2/collisionDispatch.h"
#include "../src/physics2/intersectData.h"

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
    IntersectData b3_b0{collision(box3, box0)};
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
