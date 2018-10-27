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
        inline point(const float x_, const float y_)
        {
            x = x_;
            y = y_;
        }
        float x;
        float y;
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
        render();
        ~render();
        bool init(SDL_Window* window);
        void render_object();
        void render_triangle(const triangle& t);
        void render_circle(const circle& c);
        //Clear screen color
        void clear() const;
        //Clear render objects
        void clear_render_data() const;

    private:
        void* gl_context;
        };
}
}
