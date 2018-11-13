#pragma once

#include "audio.hpp"
#include "game_object.hpp"
#include "input_manager.hpp"
#include "types.hpp"

class player : public game_object {
public:
    ~player();

    void init(float speed, glm::vec2 pos, input_manager* input_manager);

    void update(const std::vector<std::string>& level_data, float delta_time);

    void draw();

    void shoot();

    void set_direction(direction dir) { _direction = dir; }

private:    
    bool can_shoot;
    int _bullet_speed;
    int bullets_in_screen;
    int max_bullets_in_screen;
    direction _direction;
    input_manager* _input_manager;
};
