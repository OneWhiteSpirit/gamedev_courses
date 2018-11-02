#pragma once
#include "render.hpp"

typedef __SIZE_TYPE__ size_t;
struct SDL_Window;

namespace one_dc {

namespace graphics {

    class window {
    public:
        window();
        ~window();
        bool init(const char* title, size_t width, size_t height);
        void update();
        bool closed();

        SDL_Window* get_sdl_window();
        static float get_time_from_init();

    private:
        const char* _title;
        size_t _width;
        size_t _height;
        bool is_closed;
        SDL_Window* _sdl_window;
        render* _render;
        void* gl_context;
    };
}
}
