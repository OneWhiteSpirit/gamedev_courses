#include "render.hpp"
#include "math_2d.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define R 24
#define C 24

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

    point::point(float x_, float y_, float tx_, float ty_)
        : x(x_)
        , y(y_)
        , tx(tx_ = 0)
        , ty(ty_ = 0)
    {
    }

    circle::circle(const float radius_)
        : radius(radius_)
    {
        float double_pi = 2 * 3.14f;
        for (float i = 0; i < double_pi; i += double_pi / NUMBER_OF_VERTICES) {
            circle_points.push_back(std::cos(i) * radius); //X coordinate
            circle_points.push_back(std::sin(i) * radius); //Y coordinate
            circle_points.push_back(0.0); //Z coordinate
        }
    }

    std::istream& operator>>(std::istream& is, point& p)
    {
        is >> p.x;
        is >> p.y;

        is >> p.tx;
        is >> p.ty;

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
            &t.triangle_points[0].x);
        ONE_DC_GL_CHECK();
        glEnableVertexAttribArray(0);
        ONE_DC_GL_CHECK();

        // texture coordinates
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(point),
            &t.triangle_points[0].tx);
        ONE_DC_GL_CHECK();
        glEnableVertexAttribArray(1);
        ONE_DC_GL_CHECK();

        glDrawArrays(GL_TRIANGLES, 0, 3);
        ONE_DC_GL_CHECK();
    }

    void render::render_circle(const circle& c)
    {
        // Use our shader
        glUseProgram(program_id);
        ONE_DC_GL_CHECK();

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3,
            c.circle_points.data());
        ONE_DC_GL_CHECK();
        glEnableVertexAttribArray(0);
        ONE_DC_GL_CHECK();
        glDrawArrays(GL_TRIANGLE_FAN, 0, NUMBER_OF_VERTICES);
        ONE_DC_GL_CHECK();
    }

    void render::clear() const
    {
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ONE_DC_GL_CHECK();
    }

    void render::clear_render_data() const
    {
        //        glDeleteBuffers(1, &vertex_buffer);
        //        ONE_DC_GL_CHECK();

        //        glDeleteVertexArrays(1, &vertex_array_id);
        //        ONE_DC_GL_CHECK();

        glDeleteProgram(program_id);
        ONE_DC_GL_CHECK();

        SDL_GL_DeleteContext(gl_context);
        ONE_DC_GL_CHECK();
    }

    GLuint render::do_shaders(const char* VertexSourcePointer, const char* FragmentSourcePointer)
    {

        // Create the shaders
        GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        ONE_DC_GL_CHECK();

        GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        ONE_DC_GL_CHECK();

        GLint Result = GL_FALSE;
        int InfoLogLength;

        // Compile Vertex Shader
        glShaderSource(VertexShaderID, 1, &VertexSourcePointer, nullptr);
        ONE_DC_GL_CHECK();

        glCompileShader(VertexShaderID);
        ONE_DC_GL_CHECK();

        // Check Vertex Shader
        glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
        ONE_DC_GL_CHECK();
        //        if (Result == 0) {
        //            GLint info_len = 0;
        //            glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &info_len);
        //            ONE_DC_GL_CHECK();
        //            std::vector<char> info_chars(static_cast<size_t>(info_len));
        //            glGetShaderInfoLog(VertexShaderID, info_len, nullptr, info_chars.data());
        //            ONE_DC_GL_CHECK();
        //            glDeleteShader(VertexShaderID);
        //            ONE_DC_GL_CHECK();

        //            std::string shader_type_name = "vertex";
        //            cerr << "Error compiling shader(vertex)\n"
        //                 << VertexSourcePointer << "\n"
        //                 << info_chars.data();
        //        }

        glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        ONE_DC_GL_CHECK();

        if (InfoLogLength > 0) {
            std::vector<char> VertexShaderErrorMessage(static_cast<size_t>(InfoLogLength + 1));
            glGetShaderInfoLog(VertexShaderID, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
            printf("%s\n", &VertexShaderErrorMessage[0]);
        }

        // Compile Fragment Shader
        glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, nullptr);
        ONE_DC_GL_CHECK();

        glCompileShader(FragmentShaderID);
        ONE_DC_GL_CHECK();

        // Check Fragment Shader
        glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
        ONE_DC_GL_CHECK();

        glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        ONE_DC_GL_CHECK();

        if (InfoLogLength > 0) {
            std::vector<char> FragmentShaderErrorMessage(static_cast<size_t>(InfoLogLength + 1));
            glGetShaderInfoLog(FragmentShaderID, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
            printf("%s\n", &FragmentShaderErrorMessage[0]);
        }

        // Link the program
        printf("Linking program\n");
        GLuint program_id = glCreateProgram();
        ONE_DC_GL_CHECK();
        if (0 == program_id) {
            cerr << "failed to create gl program";
        }

        glAttachShader(program_id, VertexShaderID);
        ONE_DC_GL_CHECK();

        glAttachShader(program_id, FragmentShaderID);
        ONE_DC_GL_CHECK();

        // bind attribute location
        glBindAttribLocation(program_id, 0, "vertex_position");
        ONE_DC_GL_CHECK();

        glLinkProgram(program_id);
        ONE_DC_GL_CHECK();

        // Check the program
        glGetProgramiv(program_id, GL_LINK_STATUS, &Result);
        ONE_DC_GL_CHECK();

        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &InfoLogLength);
        ONE_DC_GL_CHECK();

        if (InfoLogLength > 0) {
            std::vector<char> ProgramErrorMessage(static_cast<size_t>(InfoLogLength + 1));
            glGetProgramInfoLog(program_id, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
            printf("%s\n", &ProgramErrorMessage[0]);
        }

        glDetachShader(program_id, VertexShaderID);
        ONE_DC_GL_CHECK();

        glDetachShader(program_id, FragmentShaderID);
        ONE_DC_GL_CHECK();

        glDeleteShader(VertexShaderID);
        ONE_DC_GL_CHECK();

        glDeleteShader(FragmentShaderID);
        ONE_DC_GL_CHECK();

        return program_id;
    }

    bool render::load_texture(const char* path)
    {
        stbi_set_flip_vertically_on_load(true);

        int w = 0;
        int h = 0;
        int nrChannels = 0;
        unsigned char* data = stbi_load(path, &w, &h, &nrChannels,
            0);

        // if there's an error, display it
        if (!data) {
            std::cerr << "error: " << *data << std::endl;
            return false;
        }

        stbi_image_free(data);

        GLuint tex_handl = 0;
        glGenTextures(1, &tex_handl);
        ONE_DC_GL_CHECK();
        glBindTexture(GL_TEXTURE_2D, tex_handl);
        ONE_DC_GL_CHECK();

        GLint mipmap_level = 0;
        GLint border = 0;
        glTexImage2D(GL_TEXTURE_2D, mipmap_level, GL_RGBA, static_cast<int>(w), static_cast<int>(h), border,
            GL_RGBA, GL_UNSIGNED_BYTE, data);
        ONE_DC_GL_CHECK();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        ONE_DC_GL_CHECK();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        ONE_DC_GL_CHECK();

        return true;
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
        assert(result == 0);
        int gl_minor_ver = 0;
        result = SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &gl_minor_ver);
        assert(result == 0);

        if (gl_major_ver <= 2 && gl_minor_ver < 1) {
            cerr << "current context opengl version: " << gl_major_ver << '.'
                 << gl_minor_ver << '\n'
                 << "need opengl version at least: 2.1\n"
                 << std::flush;
        }

        if (glewInit() != GLEW_OK) {
            const char* err_message = SDL_GetError();
            cerr << "Failed to initialize GLEW: " << err_message << endl;
            return;
        }

        const char* vertex_shader_src = R"(
                #version 120

                attribute vec2 vertex_position;
                attribute vec2 a_tex_coord;
                varying vec2 v_tex_coord;

                void main() {
                    v_tex_coord = a_tex_coord;
                    gl_Position = vec4(vertex_position, 0.0, 1.0);
                }
                )";

        const char* fragment_shader_src = R"(
                #version 120

                varying vec2 v_tex_coord;
                uniform sampler2D s_texture;
                void main() {
                    // Output color = red
                    gl_FragColor = texture2D(s_texture, v_tex_coord);
                }
                )";

        // Create and compile our GLSL program from the shaders
        program_id = do_shaders(vertex_shader_src, fragment_shader_src);

        // Use our shader
        glUseProgram(program_id);

        int location = glGetUniformLocation(program_id, "s_texture");
        ONE_DC_GL_CHECK();
        assert(-1 != location);
        //int texture_unit = 0;
        glActiveTexture(GL_TEXTURE0);
        ONE_DC_GL_CHECK();

        if (!load_texture("dinosaur.png")) {
            cerr << "failed load texture" << endl;
            return;
        }

        glUniform1i(location, 0);
        ONE_DC_GL_CHECK();

        glEnable(GL_BLEND);
        ONE_DC_GL_CHECK();
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        ONE_DC_GL_CHECK();

        glRasterPos2i(0, 1);
    }
}
}
