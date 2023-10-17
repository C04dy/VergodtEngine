#include <iostream>
#include "Engine/Engine.h"
#include "Sandbox/Scene.h"

int main()
{
    Scene scene;

    Engine engine;
    
    engine.EngineStart(&scene);

    return 0;
}