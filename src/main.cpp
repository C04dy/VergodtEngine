#include "Engine/Engine.h"

int main(int argc, char** argv)
{
    Engine engine;
    
    try
    {
#ifdef NDEBUG
        engine.EngineStart();
#else
        engine.EngineStart(argc, argv);
#endif
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}