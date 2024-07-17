#include "Engine.h"
#include "Scene.h"
#include "Input.h"
#include "Window.h"
#include <box2d/b2_world.h>

void Engine::EngineStart(){
    Scene Scene;

    InputManager Input; 

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        std::cout << "SDL_Init failed";
        throw std::runtime_error(SDL_GetError());
    }

    if(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_AVIF | IMG_INIT_JXL | IMG_INIT_TIF | IMG_INIT_WEBP) < 0){
        std::cout << "IMG_Init failed";
        throw std::runtime_error(SDL_GetError());
    }

    Window* Windowptr = new Window(Scene.GetGameName(), Scene.GetWindowWidth(), Scene.GetWindowHeight());

    SDL_Event e;

	int32 VelocityIterations = 6;
	int32 PositionIterations = 2;

    m_physicsworld = new b2World({ 0.0f, 9.8f });

    m_physicsworld->SetWarmStarting(true);
    m_physicsworld->SetContinuousPhysics(true);

    double EndTime = 0;
    double Delta = 0;

    ssq::VM vm(1024, ssq::Libs::ALL);

    Scene.SetRenderer(Windowptr->GetRenderer());
    Scene.SetPhysicsWorld(m_physicsworld);
    Scene.SetInput(&Input);
    Scene.SetSquirrelVM(&vm);
    Scene.Start();

    while(IsRunning()){
        float Time = (float)(SDL_GetTicks()) / 1000.0f;
        Delta = Time - EndTime;
        EndTime = Time;

        if (Input.DoInput(&e) == false) {
            SetRunning(false);
            goto Quit;
        }

        Scene.Update(Delta);

        m_physicsworld->Step(Delta, VelocityIterations, PositionIterations);

        SDL_SetRenderDrawColor(Windowptr->GetRenderer(), 0, 0, 0, 255);
        SDL_RenderClear(Windowptr->GetRenderer());
        Scene.Draw();
        SDL_RenderPresent(Windowptr->GetRenderer());
    }
    
Quit:

    Scene.Clean();

    delete m_physicsworld;
    m_physicsworld = nullptr;
    delete Windowptr;
    Windowptr = nullptr;

    SDL_Quit();
    IMG_Quit();
}