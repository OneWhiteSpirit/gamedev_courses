#pragma once

#include "shader.hpp"
#include "window.hpp"
#include "sprite_batch.hpp"
#include "input_manager.hpp"
#include "level.hpp"
#include "player.hpp"
#include "camera2d.hpp"
#include "audio.hpp"

#include <vector>

enum class game_state
{
    RUN,
    EXIT
};

class game {
public:
    game();
    ~game();

    void start();

private:
    float _fps;
    float _max_fps;
    float _frame_time;
    size_t _screen_width;
    size_t _screen_height;
    one_dc::graphics::window _window;
    shader _texture_program;
    game_state _game_state;
    input_manager _input_manager;    
    sprite_batch _sprite_batch;
    std::vector<level*> _levels;
    size_t _current_level;
    player* _player;
    camera2d _camera;
    audio* audio_;

    void init_engine();
    void init_shaders();
    void init_level();
    void init_audio();

    void draw_game();
    void handle_input();
    void calculateFPS();

};
