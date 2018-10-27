#include "window.hpp"

#include <SDL2/SDL.h>

namespace one_dc {

using std::cerr;
using std::endl;

namespace graphics {
    window::window(const char* title_, size_t width_, size_t height_)
    {
        title = title_;
        width = width_;
        height = height_;

        if (!window::init())
            SDL_Quit();
    }

    window::~window()
    {
        SDL_DestroyWindow(sdl_window);
        SDL_Quit();
    }

    bool window::init()
    {
        SDL_version compiled = { 0, 0, 0 };
        SDL_version linked = { 0, 0, 0 };

        SDL_VERSION(&compiled);
        SDL_GetVersion(&linked);

        if (SDL_COMPILEDVERSION != SDL_VERSIONNUM(linked.major, linked.minor, linked.patch)) {
            cerr << "warning: SDL2 compiled and linked version mismatch: "
                 << compiled.major << " " << linked.major << endl;
            return false;
        }

        const int init_result = SDL_Init(SDL_INIT_EVERYTHING);
        if (init_result != 0) {
            const char* err_message = SDL_GetError();
            cerr << "Failed to initialize SDL: " << err_message << endl;
            return false;
        }

        sdl_window = SDL_CreateWindow(
            title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, static_cast<int>(width), static_cast<int>(height),
            ::SDL_WINDOW_OPENGL);

        if (sdl_window == nullptr) {
            const char* err_message = SDL_GetError();
            cerr << "Failed to create SDL window: " << err_message << endl;
            return false;
        }

        return true;
    }

    void window::update() const
    {
        swap_buffer();
    }

    void window::swap_buffer() const
    {
        SDL_GL_SwapWindow(sdl_window);
    }

    bool window::closed() const
    {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return true;
            }
        }

        return false;
    }

    SDL_Window* window::get_sdl_window() const
    {
        return sdl_window;
    }

    float window::get_time_from_init() const
    {
        std::uint32_t ms_from_library_initialization = SDL_GetTicks();
        float seconds = ms_from_library_initialization * 0.001f;
        return seconds;
    }
}
}
