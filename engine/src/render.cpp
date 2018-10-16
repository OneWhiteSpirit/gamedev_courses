#include "render.hpp"

#include <GL/glew.h>
//#include <SDL2/SDL.h>
//#include <SDL2/SDL_opengl.h>
#include <iostream>

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

render::render()
{
    using std::cerr;
    using std::endl;
}

void render::render_triangle(const triangle& t)
{
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(point),
        &t.triangle_points[0]);

    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}
