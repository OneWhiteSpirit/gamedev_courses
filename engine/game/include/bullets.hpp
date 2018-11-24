#pragma once

#include "game_object.hpp"
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
        : default_speed_(8)
        , bullets_num_(0)
        , damage_(10)
    {
    }
    ~bullets() {}
    void update_pos(float dt);
    void draw();
    void create_bullet(float x, float y, direction dir, character_type owner, int owner_id, float bullet_speed);
    character_type get_bullets_owner()
    {
        if (!bullets_.empty())
            return bullets_.front().owner;
        else
            return none;
    }

    void destroy_bullet(int id);
    void destroy_allbullets();

    float get_damage() const;
    bool collide_with_game_object(size_t index, game_object* current_game_object);

    int bullets_num() { return bullets_.size(); }
    std::vector<bullet> all_bullets() { return bullets_; }
    void set_bullets(std::vector<bullet> bullets);

private:
    bool collide_with_wall(const std::vector<std::string>& levelData, float x, float y);

    float damage_;
    std::vector<bullet> bullets_;
    float default_speed_;
    int bullets_num_;
};
