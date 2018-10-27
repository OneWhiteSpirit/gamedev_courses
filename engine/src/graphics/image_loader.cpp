#include "image_loader.hpp"
#include "error.hpp"
#include "io_manager.hpp"
#include "picopng.hpp"
#include <stdexcept>
#include <GL/glew.h>

gl_texture image_loader::load_png(const char* file_path)
{
    std::vector<unsigned char> in_image;
    std::vector<unsigned char> out_image;

    unsigned long image_width;
    unsigned long image_height;

    if (!io_manager::read_file_to_buffer(file_path, in_image)) {
        throw std::runtime_error("Failed to load PNG to buffer");
    }

    int err = decodePNG(out_image, image_width, image_height, &(in_image[0]), in_image.size());
    if (err != 0) {
        throw std::runtime_error("decodePNG failed");
    }

    gl_texture texture = {};

    glGenTextures(1, &(texture.id));
    ONE_DC_GL_CHECK();

    glBindTexture(GL_TEXTURE_2D, texture.id);
    ONE_DC_GL_CHECK();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<int>(image_width), static_cast<int>(image_height), 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out_image[0]));
    ONE_DC_GL_CHECK();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    ONE_DC_GL_CHECK();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    ONE_DC_GL_CHECK();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    ONE_DC_GL_CHECK();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    ONE_DC_GL_CHECK();

    glGenerateMipmap(GL_TEXTURE_2D);
    ONE_DC_GL_CHECK();

    glBindTexture(GL_TEXTURE_2D, 0);
    ONE_DC_GL_CHECK();

    texture.width = static_cast<int>(image_width);
    texture.height = static_cast<int>(image_height);

    return texture;
}
