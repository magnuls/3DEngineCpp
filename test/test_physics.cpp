#include <gtest/gtest.h>

#include "../src/physics2/boundingSphere.h"
#include "../src/physics2/intersectData.h"

TEST(PhysicsTest, BoundingSphere) {
    BoundingSphere sphere1(Vector3f(0, 0, 0), 1);  // red
    BoundingSphere sphere2(Vector3f(1, 1, 1), 1);  // blue
    BoundingSphere sphere3(Vector3f(4, 3, 2), 1);  // purple
    BoundingSphere sphere4(Vector3f(0, 0, 2), 1);  // white
                                                   //
                                                   //
    IntersectData s1_s2{sphere1.intersectBoundingSphere(sphere2)};
    IntersectData s2_s1{sphere2.intersectBoundingSphere(sphere1)};
    IntersectData s3_s4{sphere3.intersectBoundingSphere(sphere4)};
    IntersectData s4_s1{sphere1.intersectBoundingSphere(sphere4)};

    EXPECT_TRUE(s1_s2.getDoesIntersect());
    EXPECT_TRUE(s2_s1.getDoesIntersect());
    EXPECT_FALSE(s3_s4.getDoesIntersect());
    EXPECT_TRUE(s4_s1.getDoesIntersect());
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
