#include "enemy.hpp"

#include <ctime>
#include <random>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

#include "game.hpp"
#include "resource_manager.hpp"

enemy::enemy()
    : _frames(0)
    , _direction(direction_left)
{
    _health = 35.0f;
}

enemy::~enemy()
{
}

void enemy::init(float speed, glm::vec2 pos)
{
    static std::mt19937 randomEngine(time(nullptr));
    static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

    _color.set_color(255, 255, 255, 255);

    _speed = speed;
    _position = pos;
    _dir = glm::vec2(randDir(randomEngine), randDir(randomEngine));

    _dir = glm::normalize(_dir);

    _texture_id = resource_manager::get_texture("resources/floor_water.png").id;
}

void enemy::update(const std::vector<std::string>& levelData, float delta_time)
{
    static std::mt19937 randomEngine(time(nullptr));
    static std::uniform_real_distribution<float> randomRotate(-40.0f, 40.0f);

    _position += _speed * delta_time;

    if (_frames == 20) {
        _dir = glm::rotate(_dir, randomRotate(randomEngine));
        _frames = 0;
    } else {
        _frames++;
    }

    if (collision_with_level(levelData)) {
        _dir = glm::rotate(_dir, randomRotate(randomEngine));
        _frames = 0;
    }

    switch (dir) {

        if (bullet_speed > 0) {
            set_direction(direction_up);
        }

    case direction_down:
        tempBullet.x = x + (24 / static_cast<float>(OBJECT_WIDTH));
        tempBullet.y = y - (16 / static_cast<float>(OBJECT_WIDTH));
        tempBullet.xv = 0.0;
        tempBullet.yv = -bullet_speed;
        if()
        break;
    case direction_left:
        tempBullet.x = x - (16 / static_cast<float>(OBJECT_WIDTH));
        tempBullet.y = y + (24 / static_cast<float>(OBJECT_WIDTH));
        tempBullet.xv = -bullet_speed;
        tempBullet.yv = 0.0;
        break;
    case direction_right:
        tempBullet.x = x + (64 / static_cast<float>(OBJECT_WIDTH));
        tempBullet.y = y + (24 / static_cast<float>(OBJECT_WIDTH));
        tempBullet.xv = bullet_speed;
        tempBullet.yv = 0.0;
        break;
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
