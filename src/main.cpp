#include "3DEngine.h"
#include "components/freeLook.h"
#include "components/freeMove.h"
#include "physics2/boundingSphere.h"

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

int main() {
    /*
     * This is for a visual test of the 3D Engine
     */
    TestGame game;
    Window window(1000, 600, "3D Engine Visual");

    RenderingEngine renderer(window);

    CoreEngine engine(60, &window, &renderer, &game);
    engine.Start();

    return 0;
}
