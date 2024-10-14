#include "Engine.h"
#include "Scene.h"
#include "Project.h"
#include "Input.h"
#include "Window.h"
#include <box2d/box2d.h>

/*static uint32_t s_AllocCount = 0;

bool CountAlloc = false;

void* operator new(size_t size)
{
    if (CountAlloc)
    {
        s_AllocCount++;

        std::cout << "Allocating " << size << " bytes\n";
    }

    return malloc(size);
}*/

void Engine::EngineStart(int argc, char** argv)
{
    Scene scene;

    InputManager input; 

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == false)
    {
        std::cout << "SDL_Init failed";
        throw std::runtime_error(SDL_GetError());
    }

    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_AVIF | IMG_INIT_JXL | IMG_INIT_TIF | IMG_INIT_WEBP) < 0)
    {
        std::cout << "IMG_Init failed";
        throw std::runtime_error(SDL_GetError());
    }

#ifdef NDEBUG
        Project project;
#else
        Project project(argc, argv);
#endif

    Window* window_pointer = new Window(project.GetGameName(), project.GetWindowWidth(), project.GetWindowHeight(), true);

    SDL_Event event;

    b2WorldDef world_def = b2DefaultWorldDef();

    world_def.gravity = (b2Vec2){ 0.0f, 9.5f };

    b2WorldId PhysicsWorldID = b2CreateWorld(&world_def);

    double end_time = 0;
    double delta = 0;

    scene.SetRenderer(window_pointer->GetRenderer());
    scene.SetProject(&project);
    scene.SetPhysicsWorldID(PhysicsWorldID);
    scene.SetInput(&input);

    scene.Start();

    //SetRunning(false);

    while (IsRunning())
    {
        float time = (float)(SDL_GetTicks()) / 1000.0f;
        delta = time - end_time;
        end_time = time;

        if (input.DoInput(&event) == false)
        {
            SetRunning(false);
            break;
        }

        scene.Update(delta);

        b2World_Step(PhysicsWorldID, 1.0f / 60.0f, 4);

        SDL_SetRenderDrawColor(window_pointer->GetRenderer(), 0, 0, 0, 255);
        SDL_RenderClear(window_pointer->GetRenderer());
        scene.Draw();
        SDL_RenderPresent(window_pointer->GetRenderer());
    }

    scene.Clean();

    b2DestroyWorld(PhysicsWorldID);
    delete window_pointer;
    window_pointer = nullptr;

    SDL_Quit();
    IMG_Quit();
}