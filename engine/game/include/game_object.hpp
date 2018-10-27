#pragma once

#include "sprite_batch.hpp"
#include <glm/vec2.hpp>
#include <string>

const int OBJECT_WIDTH = 8;

class game_object {
public:
    game_object();
    virtual ~game_object();

    virtual void update(const std::vector<std::string>& level_data) = 0;

    void collision_with_level(const std::vector<std::string>& level_data);

    void draw(sprite_batch& sprite_batch);

    glm::vec2 get_position() const { return _position; }

protected:
    glm::vec2 _position;
    float _speed;
    color _color;

    void check_tile_pos(const std::vector<std::string>& level_data, std::vector<glm::vec2>& collision_tile_pos, float x, float y);

    void collide_with_tile(glm::vec2 tile_pos);
};
