#include "enemy.hpp"
#include "game.hpp"

#include <random>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/rotate_vector.hpp"

#include "resource_manager.hpp"

enemy::enemy()
    : _frames(0)
{
    _health = 40.0f;
    _direction = direction_left;
}

enemy::~enemy()
{
}

void enemy::init(float speed, glm::vec2 pos)
{    
    shoot_counter = 0;
    static std::mt19937 randomEngine;
    randomEngine.seed((std::random_device()()));
    static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

    _color.set_color(255, 255, 255, 255);

    _speed = speed;
    _position = pos;
    _dir = glm::vec2(randDir(randomEngine), randDir(randomEngine));

    _dir = glm::normalize(_dir);
}

void enemy::update(const std::vector<std::string>& levelData, float delta_time)
{
    static std::mt19937 randomEngine;
    randomEngine.seed((std::random_device()()));
    static std::uniform_real_distribution<float> randomRotate(-40.0f, 40.0f);

    _position += _dir * _speed * delta_time;

    if (_frames == 20) {
        _dir = glm::rotate(_dir, randomRotate(randomEngine));
        _frames = 0;
    } else {
        _frames++;
    }

    glm::vec2 min_player_dist = get_nearest_player(game::instance().get_player()->get_position());
    if (min_player_dist != glm::vec2(-10000.0f, -10000.0f)) {
        _dir = glm::normalize(min_player_dist - _position);
        _position += _dir * _speed * delta_time;

        if (shoot_counter >= 20) {
            shoot();
            shoot_counter = 0;
        }

        shoot_counter++;
    }

    if (collision_with_level(levelData)) {
        _dir = glm::rotate(_dir, randomRotate(randomEngine));
        _frames = 0;
    }

    if (_dir.x > 0)
        set_direction(direction_right);
    else if (_dir.x < 0)
        set_direction(direction_left);
    else if (_dir.y > 0) {
        set_direction(direction_up);
    } else if (_dir.y < 0) {
        set_direction(direction_down);
    }
}

void enemy::draw()
{
    const glm::vec4 uv_rect(0.0f, 0.0f, 1.0f, 1.0f);

    glm::vec4 dest_rect;
    dest_rect.x = _position.x;
    dest_rect.y = _position.y;
    dest_rect.z = OBJECT_WIDTH;
    dest_rect.w = OBJECT_WIDTH;

    switch (_direction) {
    case direction_up:
        game::instance().get_sprite_batch()->draw(dest_rect, uv_rect, resource_manager::get_texture("resources/tank_sprites/red/up1.png").id, _color);
        break;
    case direction_right:
        game::instance().get_sprite_batch()->draw(dest_rect, uv_rect, resource_manager::get_texture("resources/tank_sprites/red/right1.png").id, _color);
        break;
    case direction_left:
        game::instance().get_sprite_batch()->draw(dest_rect, uv_rect, resource_manager::get_texture("resources/tank_sprites/red/left1.png").id, _color);
        break;
    case direction_down:
        game::instance().get_sprite_batch()->draw(dest_rect, uv_rect, resource_manager::get_texture("resources/tank_sprites/red/down1.png").id, _color);
        break;
    }
}

void enemy::shoot()
{
    game::instance().get_bullets()->create_bullet(_position.x, _position.y, _direction, ENEMY, 2, 10);    
}

glm::vec2 enemy::get_nearest_player(glm::vec2 player_pos)
{
    glm::vec2 min_player_dist = glm::vec2(-10000.0f, -10000.0f);
    float smallestDistance = 5 * TILE_WIDTH;
    glm::vec2 distVec = player_pos - _position;
    float distance = glm::length(distVec);

    if (distance <= smallestDistance) {
        smallestDistance = distance;
        min_player_dist = player_pos;
    }

    return min_player_dist;
}
