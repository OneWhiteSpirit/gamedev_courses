#pragma once
#include "player.hpp"
#include <SDL2/SDL_keycode.h>

void player::init(float speed, glm::vec2 pos, input_manager* input_manager)
{
    _speed = speed;
    _position = pos;
    _color.set_color(0, 0, 128, 255);
    _input_manager = input_manager;
}

void player::update(const std::vector<std::string>& level_data)
{
    if (_input_manager->is_key_pressed(SDLK_w)) {
        _position.y += _speed;
    } else if (_input_manager->is_key_pressed(SDLK_s)) {
        _position.y -= _speed;
    } else if (_input_manager->is_key_pressed(SDLK_a)) {
        _position.x -= _speed;
    } else if (_input_manager->is_key_pressed(SDLK_d)) {
        _position.x += _speed;
    }

    collision_with_level(level_data);
}
