#include "BaseGameApp.h"
#include "MainLoop.h"
#include <fstream>

#if defined __LINUX__ && __LINUX__ == 1
#include <pwd.h>
#endif

int RunGameEngine(int argc, char** argv)
{
    if (SDL_SetThreadPriority(SDL_THREAD_PRIORITY_HIGH) != 0)
    {
        LOG_WARNING("Failed to set high priority class to this process");
    }

    std::string userDirectory = "";

#if defined __ANDROID__ && __ANDROID__ == 1
    userDirectory = "/sdcard/claw/";
#elif defined __LINUX__ && __LINUX__ == 1
    const char* homedir;

    if ((homedir = getenv("HOME")) == NULL) 
    {
        homedir = getpwuid(getuid())->pw_dir;
    }
    assert(homedir != NULL);

    userDirectory = std::string(homeDir) + "/";

#elif defined __WINDOWS__ && _WINDOWS__ == 1
    configDir = "";
#endif

    LOG("Looking for: " + userDirectory + "config.xml");

    // Temporary hack - always prefer config in the same folder as binary to default config
    std::ifstream f("config.xml");
    if (f.good())
    {
        userDirectory = "";
        f.close();
    }

    LOG("Expecting config.xml in path: " + userDirectory + "config.xml");

    // Load options
    if (!g_pApp->LoadGameOptions(std::string(userDirectory + "config.xml").c_str()))
    {
        LOG_ERROR("Could not load game options. Exiting.");
        return -1;
    }

    g_pApp->GetGameConfig()->userDirectory = userDirectory;

    std::string savesFilePath = g_pApp->GetGameConfig()->userDirectory + g_pApp->GetGameConfig()->savesFile;
    LOG("Loaded with:\n\tConfig File: " + userDirectory + "config.xml" + "\n\tSaves File: " + savesFilePath);

    // Initialize game instance
    if (!g_pApp->Initialize(argc, argv))
    {
        LOG_ERROR("Failed to initialize. Exiting.");
        return -1;
    }

    // Run the game
    return g_pApp->Run();
}
