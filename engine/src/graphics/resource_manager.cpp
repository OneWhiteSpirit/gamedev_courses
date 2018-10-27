#include "resource_manager.hpp"

texture_cache resource_manager::_texture_cache;

gl_texture resource_manager::get_texture(const char *text_path)
{
    return _texture_cache.get_texture(text_path);
}
