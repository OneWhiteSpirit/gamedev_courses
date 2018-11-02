#include "game_object.hpp"
#include "level.hpp"
#include "resource_manager.hpp"

game_object::game_object()
{
}

game_object::~game_object()
{
}

void game_object::collision_with_level(const std::vector<std::string>& level_data)
{
    std::vector<glm::vec2> collision_to_tile_position;

    check_tile_pos(level_data, collision_to_tile_position, _position.x, _position.y);

    check_tile_pos(level_data, collision_to_tile_position, _position.x + OBJECT_WIDTH, _position.y);

    check_tile_pos(level_data, collision_to_tile_position, _position.x, _position.y + OBJECT_WIDTH);

    check_tile_pos(level_data, collision_to_tile_position, _position.x + OBJECT_WIDTH, _position.y + OBJECT_WIDTH);

    for (size_t i = 0; i < collision_to_tile_position.size(); i++) {
        collide_with_tile(collision_to_tile_position[i]);
    }
}

void game_object::draw(sprite_batch& sprite_batch)
{
    static GLuint texture_id0 = resource_manager::get_texture("resources/floor.png").id;
    static GLuint texture_id1 = resource_manager::get_texture("resources/floor_water.png").id;

    const glm::vec4 uv_rect(0.0f, 0.0f, 1.0f, 1.0f);

    glm::vec4 dest_rect;
    dest_rect.x = _position.x;
    dest_rect.y = _position.y;
    dest_rect.z = OBJECT_WIDTH;
    dest_rect.w = OBJECT_WIDTH;

    sprite_batch.draw(dest_rect, uv_rect, texture_id0, 0.0f, _color);
    sprite_batch.draw(dest_rect, uv_rect, texture_id1, 0.0f, _color);
}

void game_object::check_tile_pos(const std::vector<std::string>& level_data, std::vector<glm::vec2>& collision_tile_pos, float x, float y)
{
    glm::vec2 corner_pos = glm::vec2(floorf(x / static_cast<float>(TILE_WIDTH)), floorf(y / static_cast<float>(TILE_WIDTH)));

    if (level_data[corner_pos.y][corner_pos.x] != '.') {
        collision_tile_pos.push_back(corner_pos * static_cast<float>(TILE_WIDTH) + glm::vec2(static_cast<float>(TILE_WIDTH) / 2.0f));
    }
}

void game_object::collide_with_tile(glm::vec2 tile_pos)
{
    const float OBJECT_RADIUS = static_cast<float>(OBJECT_WIDTH) / 2.0f;
    const float TILE_RADIUS = static_cast<float>(TILE_WIDTH) / 2.0f;
    const float MIN_DISTANCE = OBJECT_RADIUS + TILE_RADIUS;

    glm::vec2 center_plaer_pos = _position + glm::vec2(OBJECT_RADIUS);
    glm::vec2 dist_vec = center_plaer_pos - tile_pos;

    float x_depth = MIN_DISTANCE - abs(dist_vec.x);
    float y_depth = MIN_DISTANCE - abs(dist_vec.y);

    //if this is true, we are colliding
    if (std::max(x_depth, 0.0f) < std::max(y_depth, 0.0f)) {
        if (dist_vec.x < 0) {
            _position.x -= x_depth;
        } else {
            _position.x += x_depth;
        }
    }
    else
    {
        if(dist_vec.y < 0) {
            _position.y -= y_depth;
        }else {
            _position.y += y_depth;
        }
    }
}
