#include "Engine.h"
#include "Scene.h"
#include "Input.h"
#include <box2d/b2_world.h>

int Engine::EngineStart(){
    Scene* scene = new Scene;

    Input* input = new Input; 

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        SDL_Log("SDL_Init failed (%s)", SDL_GetError());
        return 1;
    }

    if(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_AVIF | IMG_INIT_JXL | IMG_INIT_TIF | IMG_INIT_WEBP) < 0){
        SDL_Log("IMG_Init failed (%s)", SDL_GetError());
        return 1;
    }

    SDL_Window* window = nullptr;

    SDL_Renderer* renderer = nullptr;

    if(SDL_CreateWindowAndRenderer(scene->GetWindowWidth(), scene->GetWindowHeight(), SDL_WindowFlags::SDL_WINDOW_VULKAN, &window, &renderer) < 0){
        SDL_Log("SDL_CreateWindowAndRenderer failed (%s)", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_SetWindowTitle(window, scene->GetGameName().c_str());

    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    SDL_Event e;

	int32 velocityIterations = 6;
	int32 positionIterations = 2;

    m_physicsworld = new b2World({ 0.0f, 9.8f });

    m_physicsworld->SetWarmStarting(true);
    m_physicsworld->SetContinuousPhysics(true);

    double EndTime = 0;
    double delta = 0;

    scene->SetRenderer(renderer);
    scene->SetPhysicsWorld(m_physicsworld);
    scene->SetInput(input);
    scene->Start();

    while(IsRunning()){
        float time = (float)(SDL_GetTicks()) / 1000.0f;
        delta = time - EndTime;
        EndTime = time;

        while(SDL_PollEvent(&e) > 0){
            if(e.type == SDL_EVENT_KEY_DOWN && e.key.repeat == 0){
                input->SetKeyDown(e.key.keysym.sym);
            }
            if(e.type == SDL_EVENT_KEY_UP && e.key.repeat == 0){
                input->SetKeyUp(e.key.keysym.sym);
            }
            switch (e.type)
            {
            case SDL_EVENT_QUIT:
                SetRunning(false);
                break;
            case SDL_EVENT_MOUSE_MOTION:
                input->SetMousePos(Vector2(e.motion.x, e.motion.y));
                break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
                input->SetMouseKeyUp(e.button.button);
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                input->SetMouseKeyDown(e.button.button);
                break;
            }
        }
        input->keystates = SDL_GetKeyboardState(NULL);
        
        scene->Update(delta);

        m_physicsworld->Step(delta, velocityIterations, positionIterations);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        scene->Draw();
        SDL_RenderPresent(renderer);
    }

    scene->Clean();

    delete m_physicsworld;
    m_physicsworld = nullptr;
    delete scene;
    scene = nullptr;
    delete input;
    input = nullptr;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}