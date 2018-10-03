#pragma once

#include <array>
#include <fstream>
#include <algorithm>

constexpr size_t image_width = 200;
constexpr size_t image_height = 200;

const size_t image_render_size = image_width * image_height;

#pragma pack(push, 1)
struct color
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};
#pragma pack(pop)

struct point
{
    point(size_t x_, size_t y_);
    size_t x = 0;
    size_t y = 0;
};

struct line
{
    line(point start_, point end_);
    point start = {0, 0};
    point end = {0, 0};
};

struct triangle
{
    triangle(point first_, point second_, point third_);
    point first = {0, 0};
    point second = {0, 0};
    point third = {0, 0};
};

class basic_render
{
public:
    basic_render(size_t width_, size_t height_);
    void draw_line(point start, point end, color);
    void set_pixel_color(point current_point, color);
    void save_to_image(const std::string& file_name);
    void clear(color);

private:
    std::array<color, image_render_size> buffer;
    size_t width;
    size_t height;
};
