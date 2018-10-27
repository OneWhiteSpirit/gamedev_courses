#pragma once

#include "sprite_batch.hpp"

#include <vector>
#include <string>

const int TILE_WIDTH = 12;

class level
{
public:
    level(const std::string& file_name);
    void draw();

    glm::vec2 get_start_player_pos() const;

    std::vector<std::string> get_level_data() const;

private:
    std::vector<std::string> _level_data;
    sprite_batch _sprite_batch;

    glm::vec2 _start_player_pos;
    //std::vector<glm::ivec2> _
};
