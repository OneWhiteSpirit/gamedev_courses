#include "player.hpp"
#include <SDL2/SDL_keycode.h>

player::~player()
{
    audio_->stop_all();
    delete audio_;
}

void player::init(float speed, glm::vec2 pos, input_manager* input_manager)
{
    _speed = speed;
    _position = pos;
    _color.set_color(0, 0, 128, 255);
    _input_manager = input_manager;

    init_player_audio();
}

void player::update(const std::vector<std::string>& level_data)
{
    if (_input_manager->is_key_pressed(SDLK_w)) {
        _position.y += _speed;
        audio_->play_chunk(SOUND_ONMOVE);
    } else if (_input_manager->is_key_pressed(SDLK_s)) {
        _position.y -= _speed;
        audio_->play_chunk(SOUND_ONMOVE);
    } else if (_input_manager->is_key_pressed(SDLK_a)) {
        _position.x -= _speed;
        audio_->play_chunk(SOUND_ONMOVE);
    } else if (_input_manager->is_key_pressed(SDLK_d)) {
        _position.x += _speed;
        audio_->play_chunk(SOUND_ONMOVE);
    }
    else {
        audio_->stop_chunk(SOUND_ONMOVE);
    }

    collision_with_level(level_data);
}

void player::init_player_audio()
{
    audio_ = new audio;

    audio_->init();
}
