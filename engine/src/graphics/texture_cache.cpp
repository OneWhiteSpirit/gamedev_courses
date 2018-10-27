#include "texture_cache.hpp"
#include "image_loader.hpp"

gl_texture texture_cache::get_texture(const char* texture_path)
{
    auto it_text = _texture_map.find(texture_path);

    if (it_text == _texture_map.end()) {
        gl_texture new_text = image_loader::load_png(texture_path);

        _texture_map.insert(std::make_pair(texture_path, new_text));

        return new_text;
    }

    return it_text->second;
}
