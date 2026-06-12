/*
 * Copyright (C) 2014 Benny Bobaganoosh
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "3DEngine.h"
#include "components/freeLook.h"
#include "components/freeMove.h"
#include "physics2/boundingSphere.h"
#include "testing.h"

class TestGame : public Game {
   public:
    TestGame() {}

    virtual void Init(const Window& window);

   protected:
   private:
    TestGame(const TestGame& other) = delete;
    void operator=(const TestGame& other) = delete;
};

void TestGame::Init(const Window& window) {
    Material bricks("bricks", Texture("bricks.jpg"), 0.0f, 0,
                    Texture("bricks_normal.jpg"), Texture("bricks_disp.png"),
                    0.03f, -0.5f);
    Material bricks2("bricks2", Texture("bricks2.jpg"), 0.0f, 0,
                     Texture("bricks2_normal.png"), Texture("bricks2_disp.jpg"),
                     0.04f, -1.0f);

    IndexedModel square;
    {
        square.AddVertex(1.0f, -1.0f, 0.0f);
        square.AddTexCoord(Vector2f(1.0f, 1.0f));
        square.AddVertex(1.0f, 1.0f, 0.0f);
        square.AddTexCoord(Vector2f(1.0f, 0.0f));
        square.AddVertex(-1.0f, -1.0f, 0.0f);
        square.AddTexCoord(Vector2f(0.0f, 1.0f));
        square.AddVertex(-1.0f, 1.0f, 0.0f);
        square.AddTexCoord(Vector2f(0.0f, 0.0f));
        square.AddFace(0, 1, 2);
        square.AddFace(2, 1, 3);
    }
    Mesh customMesh("square", square.Finalize());

    ////	AddToScene((new Entity(Vector3f(0, -1, 5), Quaternion(), 32.0f))
    ////		->AddComponent(new MeshRenderer(Mesh("terrain02.obj"),
    /// Material("bricks"))));
    //
    ////	AddToScene((new Entity(Vector3f(7,0,7)))
    ////		->AddComponent(new PointLight(Vector3f(0,1,0), 0.4f,
    /// Attenuation(0,0,1))));
    ////
    ////	AddToScene((new Entity(Vector3f(20,-11.0f,5),
    /// Quaternion(Vector3f(1,0,0), ToRadians(-60.0f)) *
    /// Quaternion(Vector3f(0,1,0), ToRadians(90.0f)))) /
    ///->AddComponent(new SpotLight(Vector3f(0,1,1), 0.4f,
    /// Attenuation(0,0,0.02f), ToRadians(91.1f), 7, 1.0f, 0.5f)));
    //
    //	AddToScene((new Entity(Vector3f(), Quaternion(Vector3f(1,0,0),
    // ToRadians(-45))))
    //		->AddComponent(new DirectionalLight(Vector3f(1,1,1), 0.4f,
    // 10, 80.0f, 1.0f)));
    //
    //	AddToScene((new Entity(Vector3f(0, 2, 0), Quaternion(Vector3f(0,1,0),
    // 0.4f), 1.0f))
    //		->AddComponent(new MeshRenderer(Mesh("plane3.obj"),
    // Material("bricks2")))
    //		->AddChild((new Entity(Vector3f(0, 0, 25)))
    //			->AddComponent(new MeshRenderer(Mesh("plane3.obj"),
    // Material("bricks2")))
    //			->AddChild((new Entity())
    //				->AddComponent(new
    // CameraComponent(Matrix4f().InitPerspective(ToRadians(70.0f),
    // window.GetAspect(), 0.1f, 1000.0f)))
    //				->AddComponent(new FreeLook(window.GetCenter()))
    //				->AddComponent(new FreeMove(10.0f)))));
    //
    ////	AddToScene((new Entity(Vector3f(24,-12,5),
    /// Quaternion(Vector3f(0,1,0), ToRadians(30.0f)))) /
    ///->AddComponent(new MeshRenderer(Mesh("sphere.obj"),
    /// Material("bricks"))));
    //
    ////	AddToScene((new Entity(Vector3f(0,0,7), Quaternion(), 1.0f))
    ////		->AddComponent(new MeshRenderer(Mesh("square"),
    /// Material("bricks2"))));
    //
    //
    //	//TODO: Temporary Physics Engine Code!
    //	PhysicsEngine physicsEngine;
    //
    //	physicsEngine.AddObject(PhysicsObject(
    //			new BoundingSphere(Vector3f(0.0f, 0.0f, 0.0f), 1.0f),
    //		   	Vector3f(0.0f, 0.0f, 1.141f/2.0f)));
    //
    //	physicsEngine.AddObject(PhysicsObject(
    //			new BoundingSphere(Vector3f(1.414f/2.0f * 7.0f,
    // 0.0f, 1.414f/2.0f * 7.0f), 1.0f),
    // Vector3f(-1.414f/2.0f, 0.0f, -1.414f/2.0f)));
    //
    //
    //	PhysicsEngineComponent* physicsEngineComponent
    //		= new PhysicsEngineComponent(physicsEngine);
    //
    //	for(unsigned int i = 0;
    //		i < physicsEngineComponent->GetPhysicsEngine().GetNumObjects();
    //		i++)
    //	{
    //
    //		AddToScene((new Entity(Vector3f(0,0,0), Quaternion(),
    //					1.0f))
    //			->AddComponent(new PhysicsObjectComponent(
    //					&physicsEngineComponent->GetPhysicsEngine().GetObject(i)))
    //			->AddComponent(new MeshRenderer(Mesh("sphere.obj"),
    // Material("bricks"))));
    //	}
    //
    //	AddToScene((new Entity())
    //		->AddComponent(physicsEngineComponent));

    //	AddToScene((new Entity(Vector3f(), Quaternion(Vector3f(1,0,0),
    // ToRadians(-45))))
    //		->AddComponent(new DirectionalLight(Vector3f(1,1,1), 0.4f,
    // 10, 80.0f, 1.0f)));

    AddToScene(
        (new Entity())
            ->AddComponent(new CameraComponent(Matrix4f().InitPerspective(
                ToRadians(70.0f), window.GetAspect(), 0.1f, 1000.0f)))
            ->AddComponent(new FreeLook(window.GetCenter()))
            ->AddComponent(new FreeMove(10.0f)));

    static const int CUBE_SIZE = 3;

    AddToScene((new Entity())
                   ->AddComponent(new PointLight(Vector3f(1, 1, 1),
                                                 (CUBE_SIZE * CUBE_SIZE) * 2,
                                                 Attenuation(0, 0, 1))));

    for (int i = -CUBE_SIZE; i <= CUBE_SIZE; i++) {
        for (int j = -CUBE_SIZE; j <= CUBE_SIZE; j++) {
            for (int k = -CUBE_SIZE; k <= CUBE_SIZE; k++) {
                if (i == -CUBE_SIZE || i == CUBE_SIZE || j == -CUBE_SIZE ||
                    j == CUBE_SIZE || k == -CUBE_SIZE || k == CUBE_SIZE) {
                    if (i == 0 || j == 0 || k == 0) {
                        AddToScene(
                            (new Entity(Vector3f(i * 2, j * 2, k * 2)))
                                ->AddComponent(new MeshRenderer(
                                    Mesh("sphere.obj"), Material("bricks"))));
                    } else {
                        AddToScene(
                            (new Entity(Vector3f(i * 2, j * 2, k * 2)))
                                ->AddComponent(new MeshRenderer(
                                    Mesh("cube.obj"), Material("bricks2"))));
                    }
                }
            }
        }
    }
}

#include <iostream>

int main() {
    BoundingSphere sphere1(Vector3f(0, 0, 0), 1);  // red
    BoundingSphere sphere2(Vector3f(1, 1, 1), 1);  // blue
    BoundingSphere sphere3(Vector3f(4, 3, 2), 1);  // purple
    BoundingSphere sphere4(Vector3f(0, 0, 2), 1);  // white

    IntersectData s1_s2{sphere1.intersectBoundingSphere(sphere2)};
    IntersectData s2_s1{sphere2.intersectBoundingSphere(sphere1)};
    IntersectData s3_s4{sphere3.intersectBoundingSphere(sphere4)};
    IntersectData s4_s1{sphere1.intersectBoundingSphere(sphere4)};

    std::cout << "Intersection 1 and 2: " << s1_s2.getDoesIntersect() << '\n';
    std::cout << "Intersection 2 and 1: " << s2_s1.getDoesIntersect() << '\n';
    std::cout << "Intersection 3 and 4: " << s3_s4.getDoesIntersect() << '\n';
    std::cout << "Intersection 4 and 1: " << s4_s1.getDoesIntersect() << '\n';

    std::cout << "Distance 1 and 2: " << s1_s2.getDistance() << '\n';
    std::cout << "Distance 2 and 1: " << s2_s1.getDistance() << '\n';
    std::cout << "Distance 3 and 4: " << s3_s4.getDistance() << '\n';
    std::cout << "Distance 4 and 1: " << s4_s1.getDistance() << '\n';

    return 0;
}
