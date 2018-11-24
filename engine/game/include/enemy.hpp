#pragma once

#include "game_object.hpp"

class enemy : public game_object {
public:
    enemy();
    ~enemy();

    void init(float speed, glm::vec2 pos);

    void update(const std::vector<std::string>& level_data, float delta_time);

    void draw();

private:
    unsigned int shoot_counter;
    int _frames;

    void set_direction(direction dir) { _direction = dir; }
    void shoot();
    glm::vec2 get_nearest_player(glm::vec2 player_pos);
};
