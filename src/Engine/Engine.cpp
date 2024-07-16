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

    squall::VMStd vm;

    Scene.SetRenderer(Windowptr->GetRenderer());
    Scene.SetPhysicsWorld(m_physicsworld);
    Scene.SetInput(&Input);
    Scene.SetSquirrelVM(&vm);
    Scene.Start();

    while(IsRunning()){
        float Time = (float)(SDL_GetTicks()) / 1000.0f;
        Delta = Time - EndTime;
        EndTime = Time;

        while(SDL_PollEvent(&e) > 0){
            switch (e.type)
            {
            case SDL_EVENT_QUIT:
                SetRunning(false);
                goto Quit;
                break;
            case SDL_EVENT_MOUSE_MOTION:
                Input.SetMousePos(Vector2(e.motion.x, e.motion.y));
                break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
                Input.SetMouseKeyUp(e.button.button);
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                Input.SetMouseKeyDown(e.button.button);
                break;
            case SDL_EVENT_KEY_DOWN:
            if (e.key.repeat == 0)
                Input.SetKeyDown(e.key.keysym.sym);
                break;
            case SDL_EVENT_KEY_UP:
                Input.SetKeyUp(e.key.keysym.sym);
                break;
            }

        }
        Input.keystates = SDL_GetKeyboardState(NULL);
        Input.UpdateMouseInputNodes();
        Input.UpdateKeyboardInputNodes();

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