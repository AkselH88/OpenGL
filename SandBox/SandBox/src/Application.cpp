#include "Window/Window.h"
#include "Scene/Scene.h"
#include "Scene/Level.h"
#include "Scene/Object.h"
#include "Scene/Character.h"

int main(void)
{
    Window window;
    if (!window.InitOK())
        return -1;

    //Camera camera;
     
    Scene::Level* world = new Scene::Level("res/glTF/terrain.glb");
    Scene::Character* player = new Scene::Character("res/glTF/main_faster.glb");

    world->Init();
    player->Init();

    while (window.IsOpen())
    {
        /* Update */
        window.OnUpdate();

        world->OnUpdate(window.FrameTime(), window.GetCamera());
        player->OnUpdate(window.FrameTime(), window.GetCamera());

        /* Draw */
        world->OnDraw();
        player->OnDraw();

        /* Render */
        window.OnRender();
    }
    delete world, world = nullptr;
    delete player, player = nullptr;

    window.OnTerminate();

    return 0;
}
