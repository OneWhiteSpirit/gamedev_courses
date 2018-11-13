#pragma once

#include "bullets.hpp"
#include "camera2d.hpp"
#include "enemy.hpp"
#include "input_manager.hpp"
#include "level.hpp"
#include "player.hpp"
#include "shader.hpp"
#include "sprite_batch.hpp"
#include "window.hpp"

#include <memory>
#include <vector>

enum class game_state {
    RUN,
    EXIT
};

class game {
public:
    static game& instance()
    {
        static game game;
        return game;
    }

    void start();

    player* get_player() const { return _player.get(); }
    camera2d* get_camera2d() const { return _camera.get(); }
    sprite_batch* get_sprite_batch() const { return _sprite_batch.get(); }
    bullets* get_bullets() const { return _bullets.get(); }
    audio* get_audio() const { return _audio.get(); }
    std::vector<std::string> get_current_level_data() const { return _levels[_current_level]->get_level_data(); }
    //std::vector<std::unique_ptr<level>> getLevels() const { return _levels; }

    //    std::vector<std::unique_ptr<level>> get_levels() const
    //    {
    //        return _levels;
    //    }
    float get_delta_time() const { return time_step; }



    std::unique_ptr<enemy> getEnemys() const;

private:
    game();
    ~game();
    float _fps;
    float _max_fps;
    float _frame_time;
    size_t _screen_width;
    size_t _screen_height;
    size_t _current_level;
    std::unique_ptr<one_dc::graphics::window> _window;
    std::unique_ptr<shader> _texture_program;
    std::unique_ptr<game_state> _game_state;
    std::unique_ptr<input_manager> _input_manager;
    std::unique_ptr<sprite_batch> _sprite_batch;
    std::vector<std::unique_ptr<level>> _levels;
    std::unique_ptr<player> _player;
    std::unique_ptr<camera2d> _camera;
    std::unique_ptr<bullets> _bullets;
    std::unique_ptr<audio> _audio;
    std::vector<std::unique_ptr<enemy>> _enemys;

    float time_step;
    float accumulator;
    float max_time;
    Uint32 ticksOld, ticksNew;

    void init_engine();
    void init_shaders();
    void init_level();
    void init_audio();

    void draw_game();
    void handle_input();
    void calculateFPS();
};

inline bool CheckCollision(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
    if (x1 > x3) {
        std::swap(x1, x3);
        std::swap(y1, y3);
        std::swap(x2, x4);
        std::swap(y2, y4);
    }
    if (y1 > y2)
        std::swap(y1, y2);
    if (y3 > y4)
        std::swap(y3, y4);
    if (x1 > x2)
        std::swap(x1, x2);
    if (x3 > x4)
        std::swap(x3, x4);

    if (y1 > y3) {
        std::swap(y1, y3);
        std::swap(y2, y4);
    }
    if (y1 == y3 && y2 > y4)
        std::swap(y2, y4);

    if (x3 < x2 && y3 < y2) {
        return true;
    }
    return false;
}
