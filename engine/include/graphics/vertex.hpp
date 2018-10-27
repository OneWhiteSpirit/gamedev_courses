#pragma once

typedef unsigned char GLubyte;

struct position {
    float x;
    float y;
};

struct color {
    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;

    void set_color(GLubyte r_, GLubyte g_, GLubyte b_, GLubyte a_)
    {
        r = r_;
        g = g_;
        b = b_;
        a = a_;
    }
};

struct uv_coordinate {
    float u;
    float v;
};

struct vertex {
    position pos;
    color col;
    uv_coordinate uv;

    void set_position(float x, float y)
    {
        pos.x = x;
        pos.y = y;
    }

    void set_color(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
    {
        col.r = r;
        col.g = g;
        col.b = b;
        col.a = a;
    }

    void set_uv(float u, float v)
    {
        uv.u = u;
        uv.v = v;
    }
};
