#pragma once

#include "sprite_batch.hpp"

#include <map>
#include <string>
#include <vector>

const int TILE_WIDTH = 10;

class level {
public:
    level(const std::string& file_name);
    void draw();

    size_t get_width() const { return _level_data[0].size(); }
    size_t get_height() const { return _level_data.size(); }
    glm::vec2 get_start_player_pos() const;

    std::vector<std::string> get_level_data() const;

    std::map<size_t, size_t> getLevel_walls() const;

private:
    std::vector<std::string> _level_data;
    std::map<size_t, size_t> _level_walls;
    sprite_batch _sprite_batch;

    glm::vec2 _start_player_pos;
};
