#pragma once

#include "glm/vec2.hpp"
#include <unordered_map>

#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_events.h>


enum class event
{
    /// input events
    left_pressed,
    left_released,
    right_pressed,
    right_released,
    up_pressed,
    up_released,
    down_pressed,
    down_released,
    escape_pressed,
    escape_released,
    start_pressed,
    start_released,
    l_ctrl_pressed,
    l_ctrl_released,
    space_pressed,
    space_released,
    /// virtual console events
    turn_off
};

enum class keys
{
    left,
    right,
    up,
    down,
    escape,
    start,
    l_ctrl,
    space
};

struct bind
{
    bind(std::string s, SDL_Keycode k, event pressed, event released,
         keys om_k)
        : name(s)
        , key(k)
        , event_pressed(pressed)
        , event_released(released)
        , om_key(om_k)
    {
    }

    std::string name;
    SDL_Keycode      key;

    event event_pressed;
    event event_released;

    keys om_key;
};

static std::array<std::string, 17> event_names = {
    /// input events
    { "left_pressed", "left_released", "right_pressed", "right_released",
      "up_pressed", "up_released", "down_pressed", "down_released",
      "escape_pressed", "escape_released", "start_pressed", "start_released",
      "l_ctrl_pressed", "l_ctrl_released", "space_pressed",
      "space_released",
      /// virtual console events
      "turn_off" }
};

const std::array<bind, 8> keys{
    { bind{ "up", SDLK_w, event::up_pressed, event::up_released, keys::up },
      bind{ "left", SDLK_a, event::left_pressed, event::left_released,
            keys::left },
      bind{ "down", SDLK_s, event::down_pressed, event::down_released,
            keys::down },
      bind{ "right", SDLK_d, event::right_pressed, event::right_released,
            keys::right },
      bind{ "l_ctrl", SDLK_LCTRL, event::l_ctrl_pressed,
            event::l_ctrl_released, keys::l_ctrl },
      bind{ "space", SDLK_SPACE, event::space_pressed,
            event::space_released, keys::space },
      bind{ "escape", SDLK_ESCAPE, event::escape_pressed,
            event::escape_released, keys::escape },
      bind{ "start", SDLK_RETURN, event::start_pressed, event::start_released,
            keys::start } }
};

class input_manager {
public:
    input_manager();
    ~input_manager();

    void press_key(unsigned int key_id);
    void release_key(unsigned int key_id);
    bool is_key_pressed(unsigned int key_id);
    void set_mouse_coordinates(float x, float y);
    glm::vec2 get_mouse_coordinates() const;
    static bool check_input(const SDL_Event& e, const bind*& result);
    bool is_key_down(const enum keys key);

private:
    std::unordered_map<unsigned int, bool> _key_map;
    glm::vec2 _mouse_coordinates;
};
