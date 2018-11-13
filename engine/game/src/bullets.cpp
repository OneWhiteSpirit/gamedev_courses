#include "bullets.hpp"
#include "game.hpp"
#include "resource_manager.hpp"

void bullets::update_pos(float dt)
{
    for (size_t i = 0; i < bullets_.size();) {
        bullets_[i].x = bullets_[i].x + bullets_[i].xv * dt;
        bullets_[i].y = bullets_[i].y + bullets_[i].yv * dt;

        if (collide_with_wall(game::instance().get_current_level_data(), bullets_[i].x, bullets_[i].y)) {
            bullets_[i] = bullets_.back();
            bullets_.pop_back();
        }
        {
            i++;
        }
    }
}

bool bullets::collide_with_wall(const std::vector<std::string>& levelData, float x, float y)
{
    glm::vec2 gridPosition;
    gridPosition.x = floorf(x / TILE_WIDTH);
    gridPosition.y = floorf(y / TILE_WIDTH);

    // check if out of bounds
    if (gridPosition.x < 0 || gridPosition.x >= levelData[0].length() || gridPosition.y < 0 || gridPosition.y >= levelData.size()) {
        return true;
    }

    return levelData[static_cast<unsigned long>(gridPosition.y)][static_cast<unsigned long>(gridPosition.x)] != '.';
}

void bullets::draw()
{
    glm::vec4 uv_rect(0.0f, 0.0f, 1.0f, 1.0f);
    color color;
    color.set_color(255, 255, 255, 255);
    for (std::vector<bullet>::iterator iter = bullets_.begin(); iter != bullets_.end(); ++iter) {

        glm::vec4 dist_rect(iter->x, iter->y, OBJECT_WIDTH / 4.0f, OBJECT_WIDTH / 4.0f);
        if (iter->xv < 0) { // left
            GLuint tex_id = resource_manager::get_texture("resources/tank_shot/left.png").id;
            game::instance().get_sprite_batch()->draw(dist_rect, uv_rect, tex_id, color);
        } else if (iter->xv > 0) { // right
            GLuint tex_id = resource_manager::get_texture("resources/tank_shot/right.png").id;
            game::instance().get_sprite_batch()->draw(dist_rect, uv_rect, tex_id, color);
        } else if (iter->yv < 0) { // down
            GLuint tex_id = resource_manager::get_texture("resources/tank_shot/down.png").id;
            game::instance().get_sprite_batch()->draw(dist_rect, uv_rect, tex_id, color);
        } else if (iter->yv > 0) { // up
            GLuint tex_id = resource_manager::get_texture("resources/tank_shot/up.png").id;
            game::instance().get_sprite_batch()->draw(dist_rect, uv_rect, tex_id, color);
        }
    }
}

void bullets::create_bullet(float x, float y, direction dir, character_type owner, int owner_id, float bullet_speed)
{
    bullet tempBullet;
    tempBullet.owner = owner;
    tempBullet.owner_id = owner_id;
    tempBullet.is_destroyed = false;
    tempBullet.id = bullets_num_ + 1;
    ++bullets_num_;

    if (bullet_speed == 0.0f)
        bullet_speed = default_speed_;

    switch (dir) {
    case direction_up:
        tempBullet.x = x + (24 / static_cast<float>(OBJECT_WIDTH));
        tempBullet.y = y + (64 / static_cast<float>(OBJECT_WIDTH));
        tempBullet.xv = 0.0;
        tempBullet.yv = bullet_speed;
        break;
    case direction_down:
        tempBullet.x = x + (24 / static_cast<float>(OBJECT_WIDTH));
        tempBullet.y = y - (16 / static_cast<float>(OBJECT_WIDTH));
        tempBullet.xv = 0.0;
        tempBullet.yv = -bullet_speed;
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

    bullets_.insert(bullets_.end(), tempBullet);
}
