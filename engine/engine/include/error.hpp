#pragma once

#include <iostream>
#include <cassert>

using std::cerr;
using std::endl;

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

class error {
public:
    error();
};
