#include <cstdlib>

#include "basic_render.hpp"

int main()
{
    const color black = { 0, 0, 0 };
    const color white = { 255, 255, 255 };
    const color green = { 0, 255, 0 };
    const color violet = { 238, 130, 238 };

    basic_render render(image_width, image_height);
    basic_render render1(image_width, image_height);

    render.clear(white);
    render1.clear(white);

    point rand_point1(700, 370);
    point rand_point2(240, 500);
    point rand_point3(700, 100);

    render.fill_triangel(triangle(rand_point1, rand_point2, rand_point3), green);

    render.save_to_image("triangle.ppm");

    for (size_t i = 0; i < 100; i++) {
        point point_start(rand() % image_width, rand() % image_height);
        point point_end(rand() % image_width, rand() % image_height);
        render1.draw_line(line(point_start, point_end), violet);
    }

    render1.save_to_image("lines.ppm");

    return EXIT_SUCCESS;
}
