#include "window.hpp"

#include <cassert>
#include <vector>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define ONE_DC_GL_CHECK()                                                   \
    {                                                                       \
        const GLenum err = glGetError();                                    \
        if (err != GL_NO_ERROR) {                                           \
            switch (err) {                                                  \
            case GL_INVALID_ENUM:                                           \
                std::cerr << "GL_INVALID_ENUM" << std::endl;                \
                break;                                                      \
            case GL_INVALID_VALUE:                                          \
                std::cerr << "GL_INVALID_VALUE" << std::endl;               \
                break;                                                      \
            case GL_INVALID_OPERATION:                                      \
                std::cerr << "GL_INVALID_OPERATION" << std::endl;           \
                break;                                                      \
            case GL_INVALID_FRAMEBUFFER_OPERATION:                          \
                std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION"             \
                          << std::endl;                                     \
                break;                                                      \
            case GL_OUT_OF_MEMORY:                                          \
                std::cerr << "GL_OUT_OF_MEMORY" << std::endl;               \
                break;                                                      \
            }                                                               \
            std::cerr << __FILE__ << ':' << __LINE__ << '(' << __FUNCTION__ \
                      << ')' << std::endl;                                  \
            assert(false);                                                  \
        }                                                                   \
    }

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

        gl_context = SDL_GL_CreateContext(sdl_window);
        if (gl_context == nullptr) {
            std::string msg("Failed to create opengl context: ");
            msg += SDL_GetError();
            cerr << msg << endl;
            return false;
        }

        int gl_major_ver = 0;
        int result = SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &gl_major_ver);
        SDL_assert(result == 0);
        int gl_minor_ver = 0;
        result = SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &gl_minor_ver);
        SDL_assert(result == 0);

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

        GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
        ONE_DC_GL_CHECK();
        const char* vertex_shader_src = R"(
attribute vec2 a_position;
void main()
{
    gl_Position = vec4(a_position, 0.0, 1.0);
}
)";
        const char* source = vertex_shader_src;
        glShaderSource(vert_shader, 1, &source, nullptr);
        ONE_DC_GL_CHECK();

        glCompileShader(vert_shader);
        ONE_DC_GL_CHECK();

        GLint compiled_status = 0;
        glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &compiled_status);
        ONE_DC_GL_CHECK();
        if (compiled_status == 0) {
            GLint info_len = 0;
            glGetShaderiv(vert_shader, GL_INFO_LOG_LENGTH, &info_len);
            ONE_DC_GL_CHECK();
            std::vector<char> info_chars(info_len);
            glGetShaderInfoLog(vert_shader, info_len, NULL, info_chars.data());
            ONE_DC_GL_CHECK();
            glDeleteShader(vert_shader);
            ONE_DC_GL_CHECK();

            std::string shader_type_name = "vertex";
            cerr << "Error compiling shader(vertex)\n"
                 << vertex_shader_src << "\n"
                 << info_chars.data();
            return false;
        }

        // create fragment shader

        GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        ONE_DC_GL_CHECK();
        const char* fragment_shader_src = R"(
void main()
{
    gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);
}
)";
        source = fragment_shader_src;
        glShaderSource(fragment_shader, 1, &source, nullptr);
        ONE_DC_GL_CHECK();

        glCompileShader(fragment_shader);
        ONE_DC_GL_CHECK();

        compiled_status = 0;
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compiled_status);
        ONE_DC_GL_CHECK();
        if (compiled_status == 0) {
            GLint info_len = 0;
            glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &info_len);
            ONE_DC_GL_CHECK();
            std::vector<char> info_chars(info_len);
            glGetShaderInfoLog(fragment_shader, info_len, NULL,
                info_chars.data());
            ONE_DC_GL_CHECK();
            glDeleteShader(fragment_shader);
            ONE_DC_GL_CHECK();

            cerr << "Error compiling shader(fragment)\n"
                 << vertex_shader_src << "\n"
                 << info_chars.data();
            return false;
        }

        // now create program and attach vertex and fragment shaders

        GLuint program_id_ = glCreateProgram();
        ONE_DC_GL_CHECK();
        if (0 == program_id_) {
            cerr << "failed to create gl program";
            return false;
        }

        glAttachShader(program_id_, vert_shader);
        ONE_DC_GL_CHECK();
        glAttachShader(program_id_, fragment_shader);
        ONE_DC_GL_CHECK();

        // bind attribute location
        glBindAttribLocation(program_id_, 0, "a_position");
        ONE_DC_GL_CHECK();
        // link program after binding attribute locations
        glLinkProgram(program_id_);
        ONE_DC_GL_CHECK();
        // Check the link status
        GLint linked_status = 0;
        glGetProgramiv(program_id_, GL_LINK_STATUS, &linked_status);
        ONE_DC_GL_CHECK();
        if (linked_status == 0) {
            GLint infoLen = 0;
            glGetProgramiv(program_id_, GL_INFO_LOG_LENGTH, &infoLen);
            ONE_DC_GL_CHECK();
            std::vector<char> infoLog(infoLen);
            glGetProgramInfoLog(program_id_, infoLen, NULL, infoLog.data());
            ONE_DC_GL_CHECK();
            cerr << "Error linking program:\n"
                 << infoLog.data();
            glDeleteProgram(program_id_);
            ONE_DC_GL_CHECK();
            return false;
        }

        // turn on rendering with just created shader program
        glUseProgram(program_id_);
        ONE_DC_GL_CHECK();

        return true;
    }

    void window::clear() const
    {
        glClearColor(0.f, 1.0, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
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
}
}
