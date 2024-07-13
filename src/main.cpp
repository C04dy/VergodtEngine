#include "Engine/Engine.h"

int main()
{
    Engine engine;
    
    try {
        engine.EngineStart();
    } catch (const std::exception &e) {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;

}