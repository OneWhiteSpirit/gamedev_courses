#include "window.hpp"
#include "error.hpp"

#include <iostream>
#include <cassert>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>


namespace one_dc {

using std::cerr;
using std::endl;

namespace graphics {
    window::window()
    {
    }

    window::~window()
    {
        SDL_DestroyWindow(_sdl_window);
        SDL_Quit();
    }

    bool window::init(const char* _title, std::size_t _width, std::size_t _height)
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

        _sdl_window = SDL_CreateWindow(
            _title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, static_cast<int>(_width), static_cast<int>(_height),
            ::SDL_WINDOW_OPENGL);

        if (_sdl_window == nullptr) {
            const char* err_message = SDL_GetError();
            cerr << "Failed to create SDL window: " << err_message << endl;
            return false;
        }

        gl_context = SDL_GL_CreateContext(_sdl_window);
        if (gl_context == nullptr) {
            std::string msg("Failed to create opengl context: ");
            msg += SDL_GetError();
            cerr << msg << endl;
            return false;
        }

        int gl_major_ver = 0;
        int result = SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &gl_major_ver);
        assert(result == 0);
        int gl_minor_ver = 0;
        result = SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &gl_minor_ver);
        assert(result == 0);

        if (gl_major_ver <= 2 && gl_minor_ver < 1) {
            cerr << "current context opengl version: " << gl_major_ver << '.'
                 << gl_minor_ver << '\n'
                 << "need opengl version at least: 2.1\n"
                 << std::flush;
            return false;
        }

        if (glewInit() != GLEW_OK) {
            const char* err_message = SDL_GetError();
            cerr << "Failed to initialize GLEW: " << err_message << endl;
            return false;
        }
        ONE_DC_GL_CHECK();

        std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

        glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
        ONE_DC_GL_CHECK();

        //Set VSYNC
        SDL_GL_SetSwapInterval(1);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        return true;
    }

    void window::update()
    {
        SDL_GL_SwapWindow(_sdl_window);
    }

    bool window::closed()
    {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return true;
            }
        }

        return false;
    }

    SDL_Window* window::get_sdl_window()
    {
        return _sdl_window;
    }

    float window::get_time_from_init()
    {
        std::uint32_t ms_from_library_initialization = SDL_GetTicks();
        float seconds = ms_from_library_initialization * 0.001f;
        return seconds;
    }
}
}
