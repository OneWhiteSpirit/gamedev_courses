//#include <iostream>
#include <cstdlib>

#include "basic_render.hpp"

int main()
{
    const color black = { 0, 0, 0 };
    const color white = { 255, 255, 255 };
    const color green = { 0, 255, 0 };
    const color violet = { 238, 130, 238};

    basic_render render(image_width, image_height);
    //basic_render render1(image_width, image_height);

    render.clear(white);
    //render1.clear(white);

    point rand_point1(0,0);
    point rand_point2(100,100);
    point rand_point3(180,50);
    point rand_point4(20,50);
    point rand_point5(10,150);

    //render.draw_line(rand_point1, rand_point2, violet);
    //render.draw_line(rand_point1, rand_point3, green);
    render.draw_triangle(rand_point2, rand_point3, rand_point4, green);

    render.draw_triangle(rand_point1, rand_point4, rand_point5, violet);

    points temp = render.get_triangel_points(rand_point1, rand_point4, rand_point5);

    render.fill_figure(temp, green);

    render.save_to_image("test_image.ppm");

    return EXIT_SUCCESS;
}
