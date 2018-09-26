#include <SDL_cpuinfo.h>
#include <SDL_platform.h>
#include <SDL_version.h>
#include <cstdlib>
#include <iostream>

int main()
{
    using std::cout;
    using std::endl;

    SDL_version compiled;
    SDL_version linked;

    const char* platform_name = SDL_GetPlatform();
    int cpu_count = SDL_GetCPUCount();
    int sys_ram = SDL_GetSystemRAM();
    SDL_bool is_3dnow_supported = SDL_Has3DNow();
    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);

    cout << "Platform: " << platform_name << endl;
    cout << "Number of logical CPU cores: " << cpu_count << endl;
    cout << "RAM: " << sys_ram << endl;
    cout << "3DNow support: ";
    if (is_3dnow_supported == SDL_TRUE)
        cout << "Yes" << endl;
    else if (is_3dnow_supported != SDL_TRUE)
        cout << "No" << endl;

    cout << "SDL linking version: " << static_cast<int>(linked.major)
         << '.' << static_cast<int>(linked.minor) << '.'
         << static_cast<int>(linked.patch) << endl;

    cout << "SDL compiled version: " << static_cast<int>(compiled.major)
         << '.' << static_cast<int>(compiled.minor)
         << '.' << static_cast<int>(compiled.patch) << endl;

    return cout ? EXIT_SUCCESS : EXIT_FAILURE;
}
