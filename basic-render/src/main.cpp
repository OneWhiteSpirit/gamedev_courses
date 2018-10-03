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

    render.clear(white);

    for(size_t i = 0; i < 1000; i++)
    {
        point rand_point = { rand() % image_width, rand() % image_height };
        render.set_pixel_color(rand_point, violet);
    }

    render.save_to_image("test_image.ppm");

    return EXIT_SUCCESS;
}
