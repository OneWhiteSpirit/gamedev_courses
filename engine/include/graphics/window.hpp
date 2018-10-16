#pragma once

#include "render.hpp"

#include <iostream>

struct SDL_Window;

namespace one_dc {

namespace graphics {
    class window {
    public:
        window(const char* title_, std::size_t width_, std::size_t height_);
        ~window();

        void update() const;
        void swap_buffer() const;
        bool closed() const;

        SDL_Window* get_sdl_window() const;

    private:
        const char* title;
        std::size_t width;
        std::size_t height;
        bool is_closed;
        SDL_Window* sdl_window;
        void* gl_context;

        bool init();
    };
}
}
