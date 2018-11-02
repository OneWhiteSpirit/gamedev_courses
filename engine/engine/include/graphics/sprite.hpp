#pragma once

#include "gl_texture.hpp"
typedef unsigned int GLuint;

class sprite {
public:
    sprite();
    ~sprite();

    void init(float x, float y, float width, float height, const char* texture_path);

    void draw();

private:
    GLuint _vbo_id;
    float _x;
    float _y;
    float _width;
    float _height;

    gl_texture _texture;
};
