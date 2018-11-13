#pragma once

#include "sprite_batch.hpp"
#include <glm/vec2.hpp>
#include <string>

class enemy;

const int OBJECT_WIDTH = 8;

class game_object {
public:
    game_object();
    virtual ~game_object();

    virtual void update(const std::vector<std::string>& level_data, float delta_time) = 0;

    bool collision_with_level(const std::vector<std::string>& level_data);
    void bullets_collision_with_level(const std::vector<std::string>& level_data, float x, float y);

    virtual void draw() = 0;

    glm::vec2 get_position() const { return _position; }

protected:
    glm::vec2 _position;
    glm::vec2 _dir;
    float _speed;
    color _color;
    float _health;
    GLuint _texture_id;

    void check_tile_pos(const std::vector<std::string>& level_data, std::vector<glm::vec2>& collision_tile_pos, float x, float y);

    void collide_with_tile(glm::vec2 tile_pos);
};
