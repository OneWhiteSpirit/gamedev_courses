#pragma once
#include "texture_cache.hpp"

class resource_manager {
public:
    static gl_texture get_texture(const char* text_path);

    //static GLuint bound_texture;

private:
    static texture_cache _texture_cache;
};
