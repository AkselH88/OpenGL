#include "Engine.h"

Engine::Engine()
    : err(0), world(nullptr), player(nullptr)
{
    if (!window.InitOK())
        err = -1;

    //Camera camera;

    world = new Scene::Level("res/glTF/terrain.glb");
    player = new Scene::Character("res/glTF/main_faster.glb");

    world->Init();
    player->Init();
}

Engine::~Engine()
{
    Turmenate();
}

void Engine::Run()
{
    if (err == -1)
        return;

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
}

int Engine::Turmenate()
{
    delete world, world = nullptr;
    delete player, player = nullptr;

    window.OnTerminate();

    return err;
}

