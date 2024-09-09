#include "App.h"

int main()
{
    App app;

    try
    {
        app.Init();
        app.Run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}