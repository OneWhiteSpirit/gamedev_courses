#include "basic_render.hpp"

point::point(size_t x_, size_t y_) : x(x_), y(y_) {}

line::line(point start_, point end_) : start(start_), end(end_) {}

triangle::triangle(point first_, point second_, point third_) : first(first_), second(second_), third(third_) {}

basic_render::basic_render(size_t width_, size_t height_) :
  width(width_)
  , height(height_) {

}

void basic_render::set_pixel_color(point current_point, color col)
{
    color& temp_color = buffer.at(current_point.y * image_width + current_point.x);

    temp_color = col;
}

void basic_render::save_to_image(const std::string &file_name)
{
    std::ofstream out_file;
    out_file.exceptions(std::ios_base::failbit);

    out_file.open(file_name, std::ios_base::binary);

    out_file << "P6\n" << image_width << ' ' << image_height << ' ' << 255 << '\n';

    out_file.write(reinterpret_cast<const char*>(&buffer), sizeof(color) * image_render_size);
    out_file.close();
}

void basic_render::clear(color col)
{
    std::fill(begin(buffer), end(buffer), col);
}
