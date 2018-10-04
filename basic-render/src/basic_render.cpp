#include "basic_render.hpp"

point::point(size_t x_, size_t y_)
    : x(x_)
    , y(y_)
{
}

line::line(point start_, point end_)
    : start(start_)
    , end(end_)
{
}

triangle::triangle(point first_, point second_, point third_)
    : first(first_)
    , second(second_)
    , third(third_)
{
}

basic_render::basic_render(size_t width_, size_t height_)
    : width(width_)
    , height(height_)
{
}

void basic_render::draw_line(point start, point end, color col)
{
    int A, B, sign;
    A = end.y - start.y;
    B = start.x - end.x;
    if (abs(A) > abs(B))
        sign = 1;
    else
        sign = -1;
    int signa, signb;
    if (A < 0)
        signa = -1;
    else
        signa = 1;
    if (B < 0)
        signb = -1;
    else
        signb = 1;
    int f = 0;
    int x = start.x, y = start.y;
    if (sign == -1) {
        do {
            f += A * signa;
            if (f > 0) {
                f -= B * signb;
                y += signa;
            }
            x -= signb;

            set_pixel_color(point(x,y), col);
        } while (x != end.x || y != end.y);
    } else {
        do {
            f += B * signb;
            if (f > 0) {
                f -= A * signa;
                x -= signb;
            }
            y += signa;

            set_pixel_color(point(x,y), col);
        } while (x != end.x || y != end.y);
    }
}

void basic_render::draw_triangle(point first, point second, point third, color col)
{
    draw_line(first, second, col);
    draw_line(second, third, col);
    draw_line(third, first, col);
}

void basic_render::set_pixel_color(point current_point, color col)
{
    buffer.at(current_point.y * image_width + current_point.x) = col;
}

points basic_render::get_triangel_points(point first, point second, point third)
{
    points current_triangel;
    std::vector<std::pair<point,point>> points_pair;
    points_pair.reserve(3);
    points_pair.push_back(std::make_pair(first, second));
    points_pair.push_back(std::make_pair(second, third));
    points_pair.push_back(std::make_pair(third, first));

    for (auto elem : points_pair) {
        int x0 = elem.first.x;
        int y0 = elem.first.y;
        int x1 = elem.second.x;
        int y1 = elem.second.y;

        auto get_line = [&current_triangel, &x0, &y0, &x1, &y1]() {
            int A, B, sign;
            A = y1 - y0;
            B = x0 - x1;
            if (abs(A) > abs(B))
                sign = 1;
            else
                sign = -1;
            int signa, signb;
            if (A < 0)
                signa = -1;
            else
                signa = 1;
            if (B < 0)
                signb = -1;
            else
                signb = 1;
            int f = 0;

            int x = x0, y = y0;
            if (sign == -1) {
                do {
                    f += A * signa;
                    if (f > 0) {
                        f -= B * signb;
                        y += signa;
                    }
                    x -= signb;
                    current_triangel.push_back(point(x, y));
                } while (x != x1 || y != y1);
            } else {
                do {
                    f += B * signb;
                    if (f > 0) {
                        f -= A * signa;
                        x -= signb;
                    }
                    y += signa;
                    current_triangel.push_back(point(x, y));
                } while (x != x1 || y != y1);
            }
        };
        get_line();
    }

    current_triangel.shrink_to_fit();

    points_pair.size();

    std::sort (current_triangel.begin(), current_triangel.end(), [](const point& i, const point& j){
        return (i.y < j.y);
    });

    return current_triangel;
}

void basic_render::fill_figure(points &figure_points, color col)
{
    for(auto start = figure_points.begin(); start != figure_points.end(); start++)
    {
        if(start->y == (start + 1)->y)
            draw_line(*start, *(start + 1) , col);
    }
}

void basic_render::save_to_image(const std::string& file_name)
{
    std::ofstream out_file;
    out_file.exceptions(std::ios_base::failbit);

    out_file.open(file_name, std::ios_base::binary);

    out_file << "P6\n"
             << image_width << ' ' << image_height << ' ' << 255 << '\n';

    out_file.write(reinterpret_cast<const char*>(&buffer), sizeof(color) * image_render_size);
    out_file.close();
}

void basic_render::clear(color col)
{
    std::fill(begin(buffer), end(buffer), col);
}
