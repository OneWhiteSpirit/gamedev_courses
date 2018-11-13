#pragma once

#include "game_object.hpp"
#include "types.hpp"

class enemy : public game_object {
public:
    enemy();
    ~enemy();

    void init(float speed, glm::vec2 pos);

    void update(const std::vector<std::string>& level_data, float delta_time);

    void set_direction(direction dir) { _direction = dir; }

    void draw();

private:
    int _frames;
    direction _direction;
};
