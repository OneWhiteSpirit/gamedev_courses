#pragma once

#include <unordered_map>
#include <glm/vec2.hpp>

class input_manager
{
public:
    input_manager();
    ~input_manager();

    void press_key(unsigned int key_id);
    void release_key(unsigned int key_id);
    bool is_key_pressed(unsigned int key_id);
    void set_mouse_coordinates(float x, float y);
    glm::vec2 get_mouse_coordinates() const;

private:
    std::unordered_map<unsigned int, bool> _key_map;
    glm::vec2 _mouse_coordinates;
};
