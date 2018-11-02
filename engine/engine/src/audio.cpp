#include "audio.hpp"
#include <iostream>
#include <stdexcept>

using std::cerr;
using std::endl;

audio::~audio()
{
    Mix_CloseAudio();
}

void audio::init()
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cerr << "- Unable to initialize SDL_Mixer! SDL_mixer Error:" << Mix_GetError() << endl;
        return;
    }

    Mix_Volume(0, MIX_MAX_VOLUME / 2);
    Mix_Volume(1, MIX_MAX_VOLUME / 2);
    Mix_Volume(2, MIX_MAX_VOLUME / 4);
    Mix_Volume(3, MIX_MAX_VOLUME / 2);
    Mix_Volume(4, MIX_MAX_VOLUME / 2);
    Mix_Volume(5, MIX_MAX_VOLUME / 2);
    Mix_Volume(6, MIX_MAX_VOLUME / 2);

    on_move_.s = Mix_LoadWAV_RW(SDL_RWFromFile("sounds/background.ogg", "rb"), 1);
    fire_.s = Mix_LoadWAV_RW(SDL_RWFromFile("sounds/fire.ogg", "rb"), 1);
    explosion_.s = Mix_LoadWAV_RW(SDL_RWFromFile("sounds/explosion.ogg", "rb"), 1);
    bonus_.s = Mix_LoadWAV_RW(SDL_RWFromFile("sounds/bonus.ogg", "rb"), 1);
    game_start_.s = Mix_LoadWAV_RW(SDL_RWFromFile("sounds/gamestart.ogg", "rb"), 1);
    game_over_.s = Mix_LoadWAV_RW(SDL_RWFromFile("sounds/gameover.ogg", "rb"), 1);

    if (on_move_.s == nullptr || fire_.s == nullptr || explosion_.s == nullptr || bonus_.s == nullptr || game_start_.s == nullptr || game_over_.s == nullptr) {
        throw std::runtime_error("- Cant't load audio\n");
    }
}

void audio::play_chunk(sound_type type)
{
    switch (type) {
    case SOUND_ONMOVE:
        if (on_move_.is_played == false) {
            Mix_PlayChannel(0, on_move_.s, -1);
            on_move_.is_played = true;
        }
        break;
    case SOUND_FIRE:
        Mix_HaltChannel(1);
        Mix_PlayChannel(1, fire_.s, 0);
        break;
    case SOUND_EXPLOSION:
        Mix_HaltChannel(2);
        Mix_PlayChannel(2, explosion_.s, 0);
        break;
    case SOUND_BONUS:
        Mix_HaltChannel(3);
        Mix_PlayChannel(3, bonus_.s, 0);
        break;
    case SOUND_GAMESTART:
        Mix_HaltChannel(4);
        Mix_PlayChannel(4, game_start_.s, 0);
        break;
    case SOUND_GAMEOVER:
        Mix_HaltChannel(5);
        Mix_PlayChannel(5, game_over_.s, 0);
        break;
    case SOUND_DIE:
        Mix_HaltChannel(6);
        Mix_PlayChannel(6, explosion_.s, 0);
    }
}

void audio::stop_chunk(sound_type type)
{
    switch (type) {
    case SOUND_ONMOVE:
        Mix_HaltChannel(0);
        on_move_.is_played = false;
        break;
    }
}

void audio::stop_all()
{
    for (int i = 0; i < 7; ++i)
        Mix_HaltChannel(i);
}
