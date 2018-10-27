#pragma once

#include <iostream>
#include <vector>

#define NUMBER_OF_VERTICES 64

struct SDL_Window;
typedef unsigned int GLuint;
typedef float GLfloat;

namespace one_dc {

using std::cerr;
using std::endl;

namespace graphics {

    struct point {
        point() = default;
        point(float x_, float y_, float tx_, float ty_);
        float x;
        float y;

        float tx; // texture coordinate
        float ty;
    };

    struct triangle {
        inline triangle()
        {
            triangle_points[0] = point();
            triangle_points[1] = point();
            triangle_points[2] = point();
        }
        point triangle_points[3];
    };

    struct circle {

        circle(const float radius_);

        float radius;
        std::vector<float> circle_points;
    };

    std::istream& operator>>(std::istream& is, point&);
    std::istream& operator>>(std::istream& is, triangle&);

    class render {
    public:
        render(SDL_Window* window);
        void render_object() const;
        void render_triangle(const triangle& t);
        void render_circle(const circle& c);
        //Clear screen color
        void clear() const;
        //Clear render objects
        void clear_render_data() const;

    private:
        void* gl_context;
        GLuint program_id;
        //GLuint vertex_buffer;
        //GLuint vertex_array_id;

        GLuint do_shaders(const char* vertex_shader_string, const char* fragment_shader_string);
        bool load_texture(const char* file_path);
    };
}
}
