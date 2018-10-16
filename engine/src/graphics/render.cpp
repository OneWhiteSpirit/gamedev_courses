#include "render.hpp"

#include <cassert>
#include <iostream>
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

    std::istream& operator>>(std::istream& is, point& p)
    {
        is >> p.x;
        is >> p.y;
        return is;
    }

    std::istream& operator>>(std::istream& is, triangle& t)
    {
        is >> t.triangle_points[0];
        is >> t.triangle_points[1];
        is >> t.triangle_points[2];
        return is;
    }

    void render::render_triangle(const triangle& t)
    {
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(point),
            &t.triangle_points[0]);

        glEnableVertexAttribArray(0);

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void render::clear() const
    {
        glClearColor(0.f, 1.0, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    render::render(SDL_Window* sdl_window)
    {
        using std::cerr;
        using std::endl;

        gl_context = SDL_GL_CreateContext(sdl_window);
        if (gl_context == nullptr) {
            std::string msg("Failed to create opengl context: ");
            msg += SDL_GetError();
            cerr << msg << endl;
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
        }

        if (glewInit() != GLEW_OK) {
            const char* err_message = SDL_GetError();
            cerr << "Failed to initialize GLEW: " << err_message << endl;
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
            glGetShaderInfoLog(vert_shader, info_len, nullptr, info_chars.data());
            ONE_DC_GL_CHECK();
            glDeleteShader(vert_shader);
            ONE_DC_GL_CHECK();

            std::string shader_type_name = "vertex";
            cerr << "Error compiling shader(vertex)\n"
                 << vertex_shader_src << "\n"
                 << info_chars.data();
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
            glGetShaderInfoLog(fragment_shader, info_len, nullptr,
                info_chars.data());
            ONE_DC_GL_CHECK();
            glDeleteShader(fragment_shader);
            ONE_DC_GL_CHECK();

            cerr << "Error compiling shader(fragment)\n"
                 << vertex_shader_src << "\n"
                 << info_chars.data();
        }

        // now create program and attach vertex and fragment shaders

        GLuint program_id_ = glCreateProgram();
        ONE_DC_GL_CHECK();
        if (0 == program_id_) {
            cerr << "failed to create gl program";
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
            glGetProgramInfoLog(program_id_, infoLen, nullptr, infoLog.data());
            ONE_DC_GL_CHECK();
            cerr << "Error linking program:\n"
                 << infoLog.data();
            glDeleteProgram(program_id_);
            ONE_DC_GL_CHECK();
        }

        // turn on rendering with just created shader program
        glUseProgram(program_id_);
        ONE_DC_GL_CHECK();
    }
}
}
