#pragma once

#include <cstddef>
#include <istream>

struct point {
    point() = default;
    point(float x_, float y_);
    float x;
    float y;

    //point operator-(const point& point1);
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

std::istream& operator>>(std::istream& is, point&);
std::istream& operator>>(std::istream& is, triangle&);

class render {
public:
    render();
    void render_triangle(const triangle& t);

private:
};
