#pragma once

#include "audio.hpp"
#include "bullets.hpp"
#include "game_object.hpp"
#include "input_manager.hpp"

class player : public game_object {
public:
    ~player();

    void init(float speed, glm::vec2 pos, input_manager* input_manager);

    void update(const std::vector<std::string>& level_data, float delta_time);

    void draw();

    void shoot();

    void set_direction(direction dir) { _direction = dir; }

    float get_player_health() { return _health; }

    bullets* get_bullets() const;

private:
    bool can_shoot;
    int _bullet_speed;

    input_manager* _input_manager;
    bullets* bullets_;
};
