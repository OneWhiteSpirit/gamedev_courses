#pragma once

#include "audio.hpp"
#include "game_object.hpp"
#include "input_manager.hpp"

class player : public game_object {
public:
    ~player();

    void init(float speed, glm::vec2 pos, input_manager* input_manager);

    void update(const std::vector<std::string>& level_data);

private:
    input_manager* _input_manager;
    audio* audio_;

    void init_player_audio();
};
