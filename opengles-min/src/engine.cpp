#include "engine.hpp"

#include <algorithm>
#include <array>
#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <SDL2/SDL.h>
#if __MINGW32__
#include <SDL2/SDL_opengl.h> // on windows for mingw
#else
#include <SDL2/SDL_opengles2.h>
#endif

#define OM_GL_CHECK()                                                          \
{                                                                              \
        const int err = glGetError();                                          \
        if (err != GL_NO_ERROR)                                                \
        {                                                                      \
            switch (err)                                                       \
            {                                                                  \
                case GL_INVALID_ENUM:                                          \
                    std::cerr << GL_INVALID_ENUM << std::endl;                 \
                    break;                                                     \
                case GL_INVALID_VALUE:                                         \
                    std::cerr << GL_INVALID_VALUE << std::endl;                \
                    break;                                                     \
                case GL_INVALID_OPERATION:                                     \
                    std::cerr << GL_INVALID_OPERATION << std::endl;            \
                    break;                                                     \
                case GL_INVALID_FRAMEBUFFER_OPERATION:                         \
                    std::cerr << GL_INVALID_FRAMEBUFFER_OPERATION              \
                              << std::endl;                                    \
                    break;                                                     \
                case GL_OUT_OF_MEMORY:                                         \
                    std::cerr << GL_OUT_OF_MEMORY << std::endl;                \
                    break;                                                     \
            }                                                                  \
            assert(false);                                                     \
        }                                                                      \
}                                                                              \

namespace my_engine_ns
{

static std::array<std::string_view, 9> event_names = {
    { /// input events
      "left_pressed", "right_pressed",
      "up_pressed", "down_pressed",
      "escape_pressed", "enter_pressed",
      "lctrl_pressed", "space_pressed",
      /// virtual console events
      "turn_off" }
};

std::ostream& operator<<(std::ostream& stream, const event e)
{
    std::uint32_t value   = static_cast<std::uint32_t>(e);
    std::uint32_t minimal = static_cast<std::uint32_t>(event::left_pressed);
    std::uint32_t maximal = static_cast<std::uint32_t>(event::turn_off);
    if (value >= minimal && value <= maximal)
    {
        stream << event_names[value];
        return stream;
    }
    else
    {
        throw std::runtime_error("too big event value");
    }
}

#pragma pack(push, 4)
struct bind
{
    SDL_Keycode      key;
    std::string_view name;
    event            event_pressed;
};
#pragma pack(pop)

const std::array<bind, 8> keys{
    { { SDLK_w, "up", event::up_pressed },
      { SDLK_a, "left", event::left_pressed },
      { SDLK_s, "down", event::down_pressed },
      { SDLK_d, "right", event::right_pressed },
      { SDLK_LCTRL, "lctrl", event::lctrl_pressed },
      { SDLK_SPACE, "space", event::space_pressed },
      { SDLK_ESCAPE, "escape", event::escape_pressed },
      { SDLK_RETURN, "enter", event::enter_pressed } }
};

static bool check_input(const SDL_Event& e, const bind*& result)
{
    using namespace std;

    const auto it = find_if(begin(keys), end(keys), [&](const bind& b) {
        return b.key == e.key.keysym.sym;
    });

    if (it != end(keys))
    {
        result = &(*it);

        return true;
    }
    return false;
}

class engine_impl final : public engine
{
public:
    /// create main window
    /// on success return empty string
    std::string initialize(std::string_view /*config*/) final
    {
        std::stringstream serr;

        SDL_version compiled;
        SDL_version linked;

        SDL_VERSION(&compiled);
        SDL_GetVersion(&linked);

        const char* platform_name = SDL_GetPlatform();
        if (platform_name != NULL) {
            std::cout << "Platform: " << platform_name << std::endl;
        } else {
            const char* err_message = SDL_GetError();
            serr << "error: faild to call SDL_GetPlatform: " << err_message << std::endl;
            return serr.str();
        }

        int cpu_count = SDL_GetCPUCount();
        if (cpu_count != 0) {
            std::cout << "Number of logical CPU cores: " << cpu_count << std::endl;
        } else {
            const char* err_message = SDL_GetError();
            serr << "error: faild to call SDL_GetCPUCount: " << err_message << std::endl;
            return serr.str();
        }

        int sys_ram = SDL_GetSystemRAM();
        if (sys_ram != 0) {
            std::cout << "RAM: " << sys_ram << std::endl;
        } else {
            const char* err_message = SDL_GetError();
            serr << "error: faild to call SDL_GetSystemRAM: " << err_message << std::endl;
            return serr.str();
        }

        SDL_bool is_3dnow_supported = SDL_Has3DNow();
        if (is_3dnow_supported == SDL_TRUE)
            std::cout << "3DNow support: Yes" << std::endl;
        else if (is_3dnow_supported != SDL_TRUE)
            std::cout << "3DNow support: No" << std::endl;

        if (SDL_COMPILEDVERSION != SDL_VERSIONNUM(linked.major, linked.minor, linked.patch)) {
            serr << "warning: SDL2 compiled and linked version mismatch: "
                 << static_cast<int>(compiled.major) << " " << static_cast<int>(linked.major) << std::endl;
        } else {
            std::cout << "SDL linking version: " << static_cast<int>(linked.major)
                 << '.' << static_cast<int>(linked.minor) << '.'
                 << static_cast<int>(linked.patch) << std::endl;

            std::cout << "SDL compiled version: " << static_cast<int>(compiled.major)
                 << '.' << static_cast<int>(compiled.minor)
                 << '.' << static_cast<int>(compiled.patch) << std::endl;
        }

        const int init_result = SDL_Init(SDL_INIT_EVERYTHING);
        if (init_result != 0) {
            const char* err_message = SDL_GetError();
            serr << "error: failed call SDL_Init: " << err_message << std::endl;
            return serr.str();
        }

        SDL_Window* const window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, 640, 480, ::SDL_WINDOW_OPENGL);

        if (window == nullptr) {
            const char* err_message = SDL_GetError();
            serr << "error: failed call SDL_CreateWindow: " << err_message << std::endl;
            SDL_Quit();
            return serr.str();
        }

        SDL_Renderer* const renderer = SDL_CreateRenderer(window, -1, 0);

        if (renderer == nullptr) {
            const char* err_message = SDL_GetError();
            serr << "error: failed call SDL_CreateRenderer: " << err_message << std::endl;
            SDL_Quit();
            return serr.str();
        }

        const int draw_color = SDL_SetRenderDrawColor(renderer, 255, 255, 0, 1);

        if (draw_color != 0) {
            const char* err_message = SDL_GetError();
            serr << "error: failed call SDL_SetRenderDrawColor: " << err_message << std::endl;
            return serr.str();
        }

        return "";
    }
    /// pool event from input queue
    bool read_input(event& e) final
    {
        using namespace std;

        SDL_Delay(20);

        // collect all events from SDL
        SDL_Event sdl_event;
        if (SDL_PollEvent(&sdl_event))
        {
            const bind* binding = nullptr;

            if (sdl_event.type == SDL_QUIT)
            {
                e = event::turn_off;
                return true;
            }
            else if (sdl_event.type == SDL_KEYDOWN)
            {
                if (check_input(sdl_event, binding))
                {
                    e = binding->event_pressed;
                    return true;
                }
            }
        }
        return false;
    }
    void uninitialize() final {}
};

static bool already_exist = false;

engine* create_engine()
{
    if (already_exist)
    {
        throw std::runtime_error("engine already exist");
    }
    engine* result = new engine_impl();
    already_exist  = true;
    return result;
}

void destroy_engine(engine* e)
{
    if (already_exist == false)
    {
        throw std::runtime_error("engine not created");
    }
    if (nullptr == e)
    {
        throw std::runtime_error("e is nullptr");
    }
    delete e;
}

engine::~engine() {}

} // end namespace my_engine_ns
