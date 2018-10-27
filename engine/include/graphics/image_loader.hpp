#pragma once

#include "gl_texture.hpp"

class image_loader {
public:
    image_loader() {}
    static gl_texture load_png(const char* file_path);
};
