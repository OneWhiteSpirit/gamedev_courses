#include "player.hpp"
#include "enemy.hpp"
#include "game.hpp"
#include "resource_manager.hpp"
#include <SDL2/SDL_keycode.h>

player::~player()
{
    delete bullets_;
}

void player::init(float speed, glm::vec2 pos, input_manager* input_manager)
{
    _speed = speed;
    _position = pos;
    _health = 50.0f;
    _color.set_color(255, 255, 255, 255);
    _input_manager = input_manager;
    set_direction(direction_up);
    can_shoot = true;
    _bullet_speed = 10;
    bullets_ = new bullets;
}

void player::update(const std::vector<std::string>& level_data, float delta_time)
{
    if (_input_manager->is_key_down(keys::up)) {
        set_direction(direction_up);
        _position.y += _speed;
        game::instance().get_audio()->play_chunk(SOUND_ONMOVE);
    } else if (_input_manager->is_key_down(keys::down)) {
        set_direction(direction_down);
        _position.y -= _speed;
        game::instance().get_audio()->play_chunk(SOUND_ONMOVE);
    } else if (_input_manager->is_key_down(keys::left)) {
        set_direction(direction_left);
        _position.x -= _speed;
        game::instance().get_audio()->play_chunk(SOUND_ONMOVE);
    } else if (_input_manager->is_key_down(keys::right)) {
        set_direction(direction_right);
        _position.x += _speed;
        game::instance().get_audio()->play_chunk(SOUND_ONMOVE);
    } else if (_input_manager->is_key_down(keys::space)) {
        if (can_shoot) {
            game::instance().get_audio()->play_chunk(SOUND_FIRE);
            game::instance().get_player()->shoot();
        }
    } else {
        game::instance().get_audio()->stop_chunk(SOUND_ONMOVE);
        can_shoot = true;
    }

    bullets_->update_pos(delta_time);

    collision_with_level(level_data);
}

void player::draw()
{
    const glm::vec4 uv_rect(0.0f, 0.0f, 1.0f, 1.0f);

    glm::vec4 dest_rect;
    dest_rect.x = _position.x;
    dest_rect.y = _position.y;
    dest_rect.z = OBJECT_WIDTH;
    dest_rect.w = OBJECT_WIDTH;

    switch (_direction) {
    case direction_up:
        game::instance().get_sprite_batch()->draw(dest_rect, uv_rect, resource_manager::get_texture("resources/tank_sprites/up1.png").id, _color);
        break;
    case direction_right:
        game::instance().get_sprite_batch()->draw(dest_rect, uv_rect, resource_manager::get_texture("resources/tank_sprites/right1.png").id, _color);
        break;
    case direction_left:
        game::instance().get_sprite_batch()->draw(dest_rect, uv_rect, resource_manager::get_texture("resources/tank_sprites/left1.png").id, _color);
        break;
    case direction_down:
        game::instance().get_sprite_batch()->draw(dest_rect, uv_rect, resource_manager::get_texture("resources/tank_sprites/down1.png").id, _color);
        break;
    }

    bullets_->draw();
}

void player::shoot()
{
    bullets_->create_bullet(_position.x, _position.y, _direction, PLAYER, 1, _bullet_speed);
    can_shoot = false;    
}

bullets* player::get_bullets() const
{
    return bullets_;
}
