#include "basic_render.hpp"

point::point(size_t x_, size_t y_)
    : x(x_)
    , y(y_)
{
}

point point::operator-(const point& point1)
{
    return point(x - point1.x, y - point1.y);
}

line::line(point start_, point end_)
    : start(start_)
    , end(end_)
{
}

size_t line::delta_x()
{
    return end.x - start.x;
}

size_t line::delta_y()
{
    return end.y - start.y;
}

size_t line::get_start_x()
{
    return start.x;
}

size_t line::get_start_y()
{
    return start.y;
}

size_t line::get_end_x()
{
    return end.x;
}

size_t line::get_end_y()
{
    return end.y;
}

triangle::triangle(point first_, point second_, point third_)
    : first(first_)
    , second(second_)
    , third(third_)
{
}

points triangle::get_triangel_points()
{
    points current_triangel;
    std::vector<std::pair<point, point>> points_pair;
    points_pair.reserve(3);
    points_pair.push_back(std::make_pair(first, second));
    points_pair.push_back(std::make_pair(second, third));
    points_pair.push_back(std::make_pair(third, first));

    for (auto elem : points_pair) {
        int x0 = elem.first.x;
        int y0 = elem.first.y;
        int x1 = elem.second.x;
        int y1 = elem.second.y;

        auto get_line = [&current_triangel](int x0, int y0, int x1, int y1) {
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
        get_line(x0, y0, x1, y1);
    }

    current_triangel.shrink_to_fit();

    points_pair.size();

    std::sort(current_triangel.begin(), current_triangel.end(), [](const point& i, const point& j) {
        return (i.y < j.y);
    });

    return current_triangel;
}

basic_render::basic_render(size_t width_, size_t height_)
    : width(width_)
    , height(height_)
{
}

void basic_render::draw_line(line line, color col)
{
    int A, B, sign;
    A = line.delta_y();
    B = -line.delta_x();
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
    int x = line.get_start_x(), y = line.get_start_y();
    if (sign == -1) {
        do {
            f += A * signa;
            if (f > 0) {
                f -= B * signb;
                y += signa;
            }
            x -= signb;

            set_pixel_color(point(x, y), col);
        } while (x != line.get_end_x() || y != line.get_end_y());
    } else {
        do {
            f += B * signb;
            if (f > 0) {
                f -= A * signa;
                x -= signb;
            }
            y += signa;

            set_pixel_color(point(x, y), col);
        } while (x != line.get_end_x() || y != line.get_end_y());
    }
}

void basic_render::draw_triangle(triangle& triangle, color col)
{
    points triangle_points = triangle.get_triangel_points();

    for (auto elem : triangle_points) {
        set_pixel_color(elem, col);
    }
}

void basic_render::set_pixel_color(point current_point, color col)
{
    buffer.at(current_point.y * image_width + current_point.x) = col;
}

void basic_render::fill_triangel(triangle triangle, color col)
{
    points triangle_points = triangle.get_triangel_points();
    for (auto start = triangle_points.begin(); start != triangle_points.end(); start++) {
        if (start->y == (start + 1)->y)
            draw_line(line(*start, *(start + 1)), col);
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
