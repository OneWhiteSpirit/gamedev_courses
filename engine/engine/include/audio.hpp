#pragma once

#include <SDL2/SDL_mixer.h>

enum sound_type {
    SOUND_ONMOVE = 0,
    SOUND_FIRE = 1,
    SOUND_EXPLOSION = 2,
    SOUND_BONUS = 3,
    SOUND_GAMESTART = 4,
    SOUND_GAMEOVER = 5,
    SOUND_DIE = 6
};

struct sound {
    sound()
        : is_played(false)
    {
    }
    bool is_played;
    Mix_Chunk* s;
};

class audio {
public:
    ~audio();
    void init();

    void play_chunk(sound_type type);
    void stop_chunk(sound_type type);
    void stop_all();

private:
    sound on_move_;
    sound fire_;
    sound explosion_;
    sound bonus_;
    sound game_start_;
    sound game_over_;
};
