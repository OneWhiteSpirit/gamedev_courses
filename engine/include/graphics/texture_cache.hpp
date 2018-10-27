#pragma once

#include "gl_texture.hpp"
#include <map>

class texture_cache
{
public:
    texture_cache() {}

    gl_texture get_texture(const char* texture_path);
private:
    std::map<const char*, gl_texture> _texture_map;

};
