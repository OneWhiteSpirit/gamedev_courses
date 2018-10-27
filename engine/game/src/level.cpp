#include "level.hpp"
#include "resource_manager.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

level::level(const std::string& file_name)
{
    std::ifstream file;
    file.open(file_name);

    if (file.fail()) {
        throw std::runtime_error("failed to open level file");
    }

    std::string tmp;

    while (std::getline(file, tmp)) {
        _level_data.push_back(tmp);
    }

    _sprite_batch.init();
    _sprite_batch.begin();

    glm::vec4 uvRect(0.0f, 0.0f, 0.2f, 0.2f);
    color col;
    col.set_color(255, 255, 255, 255);

    for (size_t i = 0; i < _level_data.size(); i++) {
        for (size_t j = 0; j < _level_data[i].size(); j++) {
            char tile = _level_data[i][j];

            glm::vec4 destRect(j * TILE_WIDTH, i * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

            switch (tile) {
            case 'R':
                _sprite_batch.draw(destRect, uvRect, resource_manager::get_texture("resources/floor.png").id, 0.0f, col);
                break;
            case 'L':
                _sprite_batch.draw(destRect, uvRect, resource_manager::get_texture("resources/floor_water.png").id, 0.0f, col);
                break;
            case '.':
                _sprite_batch.draw(destRect, uvRect, resource_manager::get_texture("resources/acid.png").id, 0.0f, col);
                break;
            case '@':
                _level_data[i][j] = '.';
                _start_player_pos.x = static_cast<int>(j * TILE_WIDTH);
                _start_player_pos.y = static_cast<int>(i * TILE_WIDTH);
                break;
            default:
                std::cout << "unexpected symbol " << std::endl;
                break;
            }
        }
    }
    _sprite_batch.end();
}

void level::draw()
{
    _sprite_batch.render_batch();
}

glm::vec2 level::get_start_player_pos() const
{
    return _start_player_pos;
}

std::vector<std::string> level::get_level_data() const
{
    return _level_data;
}
