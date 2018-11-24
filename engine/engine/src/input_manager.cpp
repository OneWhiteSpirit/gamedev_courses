#include "input_manager.hpp"

#include <algorithm>




input_manager::input_manager()
{
}

input_manager::~input_manager()
{
}

void input_manager::press_key(unsigned int key_id)
{
    _key_map[key_id] = true;
}

void input_manager::release_key(unsigned int key_id)
{
    _key_map[key_id] = false;
}

bool input_manager::is_key_pressed(unsigned int key_id)
{
    auto it = _key_map.find(key_id);
    if (it != _key_map.end()) {
        return it->second;
    } else {
        return false;
    }
}

void input_manager::set_mouse_coordinates(float x, float y)
{
    _mouse_coordinates.x = x;
    _mouse_coordinates.y = y;
}

glm::vec2 input_manager::get_mouse_coordinates() const
{
    return _mouse_coordinates;
}

bool input_manager::check_input(const SDL_Event &e, const bind *&result)
{
    using namespace std;

    const auto it = find_if(begin(keys), end(keys), [&](const bind& b) {
        return b.key == e.key.keysym.sym;
    });

    if (it != end(keys))
    {
        result = &(*it);
        return true;
    }
    return false;
}


bool input_manager::is_key_down(const enum keys key)
{
    const auto it = std::find_if(
        begin(keys), end(keys), [&](const bind& b) { return b.om_key == key; });

    if (it != end(keys))
    {
        const std::uint8_t* state         = SDL_GetKeyboardState(nullptr);
        int                 sdl_scan_code = SDL_GetScancodeFromKey(it->key);
        return state[sdl_scan_code];
    }
    return false;
}
