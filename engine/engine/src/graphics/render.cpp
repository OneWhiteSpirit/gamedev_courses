#include "render.hpp"
#include "error.hpp"

#include <fstream>
#include <sstream>
#include <vector>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

namespace one_dc {

using std::cerr;
using std::endl;

namespace graphics {

    render::render()
    {
    }

    render::~render()
    {
        clear_render_data();
    }

    void render::clear() const
    {
        glClearDepth(1.0);
        ONE_DC_GL_CHECK();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ONE_DC_GL_CHECK();
    }

    void render::clear_render_data() const
    {
        SDL_GL_DeleteContext(gl_context);
        ONE_DC_GL_CHECK();
    }

    bool render::init(SDL_Window* sdl_window)
    {
        using std::cerr;
        using std::endl;

        gl_context = SDL_GL_CreateContext(sdl_window);
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

        return true;
    }
}
}
