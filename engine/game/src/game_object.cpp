#include "game_object.hpp"
#include "level.hpp"

game_object::game_object()
{
}

game_object::~game_object()
{
}

bool game_object::collision_with_level(const std::vector<std::string>& level_data)
{
    std::vector<glm::vec2> collision_to_tile_position;

    check_tile_pos(level_data, collision_to_tile_position, _position.x, _position.y);

    check_tile_pos(level_data, collision_to_tile_position, _position.x + OBJECT_WIDTH, _position.y);

    check_tile_pos(level_data, collision_to_tile_position, _position.x, _position.y + OBJECT_WIDTH);

    check_tile_pos(level_data, collision_to_tile_position, _position.x + OBJECT_WIDTH, _position.y + OBJECT_WIDTH);

    // no blocks collided
    if (collision_to_tile_position.size() == 0) {
        return false;
    }

    for (size_t i = 0; i < collision_to_tile_position.size(); i++) {
        collide_with_tile(collision_to_tile_position[i]);
    }

    return true;
}

void game_object::bullets_collision_with_level(const std::vector<std::string>& level_data, float x, float y)
{
    std::vector<glm::vec2> collision_to_tile_position;

    check_tile_pos(level_data, collision_to_tile_position, x, y);

    check_tile_pos(level_data, collision_to_tile_position, x + OBJECT_WIDTH / 4.0f, y);

    check_tile_pos(level_data, collision_to_tile_position, x, y + OBJECT_WIDTH / 4.0f);

    check_tile_pos(level_data, collision_to_tile_position, x + OBJECT_WIDTH / 4.0f, y + OBJECT_WIDTH / 4.0f);

    for (size_t i = 0; i < collision_to_tile_position.size(); i++) {
        collide_with_tile(collision_to_tile_position[i]);
    }
}

void game_object::check_tile_pos(const std::vector<std::string>& level_data, std::vector<glm::vec2>& collision_tile_pos, float x, float y)
{
    glm::vec2 corner_pos = glm::vec2(floorf(x / static_cast<float>(TILE_WIDTH)), floorf(y / static_cast<float>(TILE_WIDTH)));

    if (level_data[static_cast<unsigned long>(corner_pos.y)][static_cast<unsigned long>(corner_pos.x)] != '.') {
        collision_tile_pos.push_back(corner_pos * static_cast<float>(TILE_WIDTH) + glm::vec2(static_cast<float>(TILE_WIDTH) / 2.0f));
    }
}

bool game_object::collide_with_game_object(game_object* agent)
{
    glm::vec2 centerPosA = _position + glm::vec2(OBJECT_RADIUS);
    glm::vec2 centerPosB = agent->get_position() + glm::vec2(OBJECT_RADIUS);

    glm::vec2 distVec = centerPosA - centerPosB;

    float distance = glm::length(distVec);
    float depth = OBJECT_WIDTH - distance;

    if (depth > 0) {
        glm::vec2 collisionDepthVec = (glm::normalize(distVec) * depth) / 2.0f;

        _position += collisionDepthVec;
        agent->_position -= collisionDepthVec;
        return true;
    }
    return false;
}

bool game_object::apply_damage(float damage)
{
    _health -= damage;
    return (_health <= 0);
}

void game_object::collide_with_tile(glm::vec2 tile_pos)
{
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
    } else {
        if (dist_vec.y < 0) {
            _position.y -= y_depth;
        } else {
            _position.y += y_depth;
        }
    }
}
