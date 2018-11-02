#include "input_manager.hpp"

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
