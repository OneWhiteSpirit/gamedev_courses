#pragma once

#include "types.hpp"
#include <string>
#include <vector>

struct bullet {
    bullet() {}
    bullet(double x, double y, character_type owner)
        : x(x)
        , y(y)
        , owner(owner)
        , is_destroyed(false)
    {
    }

    int id;
    float x, y;
    float xv, yv;
    character_type owner;
    int owner_id;
    bool is_destroyed;
};

class bullets {
public:
    bullets()
        : default_speed_(100)
        , bullets_num_(0)
    {
    }
    ~bullets() {}
    void update_pos(float dt);
    void draw();
    void create_bullet(float x, float y, direction dir, character_type owner, int owner_id, float bullet_speed);

    void destroy_bullet(int id);
    void destroy_allbullets();

    int bullets_num() { return bullets_.size(); }
    std::vector<bullet> all_bullets() { return bullets_; }

private:
    bool collide_with_wall(const std::vector<std::string>& levelData, float x, float y);

    std::vector<bullet> bullets_;
    float default_speed_;
    int bullets_num_;
};
