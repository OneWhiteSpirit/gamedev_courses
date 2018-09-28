#include <SDL_cpuinfo.h>
#include <SDL_platform.h>
#include <SDL_version.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <cstdlib>
#include <array>
#include <algorithm>
#include <string_view>

#pragma pack(push, 4)
struct bind
{
    SDL_Keycode      key;
    std::string_view name;
};
#pragma pack(pop)

void check_input(const SDL_Event& e)
{
    using namespace std;

    const array<::bind, 8> keys{ { { SDLK_w, "up" },
        { SDLK_a, "left" },
        { SDLK_s, "down" },
        { SDLK_d, "right" },
        { SDLK_LCTRL, "button_one" },
        { SDLK_SPACE, "button_two" },
        { SDLK_ESCAPE, "select" },
        { SDLK_RETURN, "start" } } };

    const auto it = find_if(begin(keys), end(keys), [&](const ::bind& b) {
        return b.key == e.key.keysym.sym;
    });

    if (it != end(keys)) {
        cout << it->name << ' ';
        if (e.type == SDL_KEYDOWN) {
            cout << "is pressed" << endl;
        } else {
            cout << "is released" << endl;
        }
    }
}

int main()
{
    using std::cout;
    using std::endl;

    SDL_version compiled;
    SDL_version linked;

    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);

    const char* platform_name = SDL_GetPlatform();
    if (platform_name != nullptr) {
        cout << "Platform: " << platform_name << endl;
    } else {
        const char* err_message = SDL_GetError();
        std::cerr << "error: faild to call SDL_GetPlatform: " << err_message << endl;
    }

    int cpu_count = SDL_GetCPUCount();
    if (cpu_count != 0) {
        cout << "Number of logical CPU cores: " << cpu_count << endl;
    } else {
        const char* err_message = SDL_GetError();
        std::cerr << "error: faild to call SDL_GetCPUCount: " << err_message << endl;
    }

    int sys_ram = SDL_GetSystemRAM();
    if (sys_ram != 0) {
        cout << "RAM: " << sys_ram << endl;
    } else {
        const char* err_message = SDL_GetError();
        std::cerr << "error: faild to call SDL_GetSystemRAM: " << err_message << endl;
    }

    SDL_bool is_3dnow_supported = SDL_Has3DNow();
    if (is_3dnow_supported == SDL_TRUE)
        cout << "3DNow support: Yes" << endl;
    else if (is_3dnow_supported != SDL_TRUE)
        cout << "3DNow support: No" << endl;

    if (SDL_COMPILEDVERSION != SDL_VERSIONNUM(linked.major, linked.minor, linked.patch)) {
        std::cerr << "warning: SDL2 compiled and linked version mismatch: "
             << static_cast<int>(compiled.major) << " " << static_cast<int>(linked.major) << endl;
    } else {
        cout << "SDL linking version: " << static_cast<int>(linked.major)
             << '.' << static_cast<int>(linked.minor) << '.'
             << static_cast<int>(linked.patch) << endl;

        cout << "SDL compiled version: " << static_cast<int>(compiled.major)
             << '.' << static_cast<int>(compiled.minor)
             << '.' << static_cast<int>(compiled.patch) << endl;
    }

    const int init_result = SDL_Init(SDL_INIT_EVERYTHING);
    if (init_result != 0) {
        const char* err_message = SDL_GetError();
        std::cerr << "error: failed call SDL_Init: " << err_message << endl;
        return EXIT_FAILURE;
    }

    SDL_Window* const window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 640, 480, ::SDL_WINDOW_OPENGL);

    if (window == nullptr) {
        const char* err_message = SDL_GetError();
        std::cerr << "error: failed call SDL_CreateWindow: " << err_message << endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    bool continue_loop = true;
    while (continue_loop) {
        SDL_Event sdl_event;

        while (SDL_PollEvent(&sdl_event)) {
            switch (sdl_event.type) {
            case SDL_KEYDOWN:
                check_input(sdl_event);
                break;
            case SDL_KEYUP:
                check_input(sdl_event);
                break;
            case SDL_QUIT:
                continue_loop = false;
                break;
            default:
                break;
            }
        }
    }

    SDL_DestroyWindow(window);

    SDL_Quit();

    return EXIT_SUCCESS;
}
