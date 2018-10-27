#include "game.hpp"
#include "error.hpp"
#include "resource_manager.hpp"
#include "window.hpp"
#include <GL/glew.h>
#include <SDL2/SDL.h>

game::game()
    : _fps(0)
    , _max_fps(60.0f)
    , _frame_time(0)
    , _screen_width(640)
    , _screen_height(360)
{
}

game::~game()
{
    for (size_t i = 0; i < _levels.size(); i++) {
        delete _levels[i];
    }
}

void game::start()
{
    init_engine();

    init_level();

    while (_game_state != game_state::EXIT) {

        float start_tick = SDL_GetTicks();

        handle_input();

        _camera.set_position(_player->get_position());
        _player->update(_levels[_current_level]->get_level_data());
        _camera.update();

        draw_game();

        calculateFPS();

        //std::cout << _fps << std::endl;

        float frame_tick = SDL_GetTicks() - start_tick;
        if (1000.0f / _max_fps > frame_tick) {
            SDL_Delay(static_cast<Uint32>(1000.0f / _max_fps - frame_tick));
        }
    }
}

void game::init_engine()
{
    if (!_window.init("some game title", _screen_width, _screen_height)) {
        SDL_Quit();
    }

    init_shaders();

    _sprite_batch.init();

    _camera.init(_screen_width / 2, _screen_height / 2);
}

void game::init_shaders()
{
    _texture_program.compile("shaders/vertex_shader.vert", "shaders/fragment_shader.frag");
    _texture_program.add_attribute("vertex_position");
    _texture_program.add_attribute("vertex_color");
    _texture_program.add_attribute("vertex_uv");
    _texture_program.link();
}

void game::init_level()
{
    _levels.push_back(new level("level1.txt"));
    _current_level = 0;

    _player = new player();
    _player->init(1.0f, _levels[_current_level]->get_start_player_pos(), &_input_manager);
}

void game::draw_game()
{
    glClearDepth(1.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _texture_program.use();

    glActiveTexture(GL_TEXTURE0);
    ONE_DC_GL_CHECK();

    GLint texture_loc = _texture_program.getUniformLocation("sampler");
    ONE_DC_GL_CHECK();
    glUniform1i(texture_loc, 0);
    ONE_DC_GL_CHECK();

    GLint time_location = _texture_program.getUniformLocation("time");
    ONE_DC_GL_CHECK();
    glUniform1f(time_location, one_dc::graphics::window::get_time_from_init());
    ONE_DC_GL_CHECK();

    glm::mat4 projection_matrix = _camera.get_camera_matrix();
    GLint p_uniform = _texture_program.getUniformLocation("projection");
    glUniformMatrix4fv(p_uniform, 1, GL_FALSE, &projection_matrix[0][0]);
    ONE_DC_GL_CHECK();

    _levels[_current_level]->draw();
    //    _sprite_batch.begin();
    //    glm::vec4 pos(-0.5f, -0.5f, 0.5f, 0.5f);
    //    glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
    //    static gl_texture texture = resource_manager::get_texture("dinosaur.png");
    //    color col;
    //    col.set_color(200, 200, 200, 200);
    //    _sprite_batch.draw(pos, uv, texture.id, 0.0f, col);
    //    _sprite_batch.draw(pos + glm::vec4(0.5, 0, 0, 0), uv, texture.id, 0.0f, col);
    //    _sprite_batch.end();

    //    _sprite_batch.render_batch();

    _sprite_batch.begin();
    _player->draw(_sprite_batch);
    _sprite_batch.end();

    _sprite_batch.render_batch();

    _texture_program.unuse();

    _window.update();
}

void game::handle_input()
{
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            _game_state = game_state::EXIT;
            break;
        case SDL_KEYDOWN:
            _input_manager.press_key(static_cast<unsigned int>(event.key.keysym.sym));
            break;
        case SDL_KEYUP:
            _input_manager.release_key(static_cast<unsigned int>(event.key.keysym.sym));
            break;
        case SDL_MOUSEBUTTONDOWN:
            _input_manager.press_key(event.button.button);
            break;
        case SDL_MOUSEBUTTONUP:
            _input_manager.release_key(event.button.button);
            break;
        case SDL_MOUSEMOTION:
            _input_manager.set_mouse_coordinates(event.motion.x, event.motion.y);
            break;
        }
    }

    if (_input_manager.is_key_pressed(SDLK_w)) {
    } else if (_input_manager.is_key_pressed(SDL_BUTTON_LEFT)) {
        glm::vec2 mouse_coord = _input_manager.get_mouse_coordinates();
        std::cout << mouse_coord.x << ' ' << mouse_coord.y << std::endl;
    }
}

void game::calculateFPS()
{

    static const int NUM_SAMPLES = 10;
    static float frame_times[NUM_SAMPLES];
    static int current_frame;

    static float previous_tick = SDL_GetTicks();

    float current_tick;
    current_tick = SDL_GetTicks();

    _frame_time = current_tick - previous_tick;
    frame_times[current_frame % NUM_SAMPLES] = _frame_time;

    previous_tick = current_tick;

    int count;
    current_frame++;
    if (current_frame < NUM_SAMPLES) {
        count = current_frame;
    } else
        count = NUM_SAMPLES;

    float frame_time_average = 0;
    for (int i = 0; i < count; i++) {
        frame_time_average += frame_times[i];
    }

    frame_time_average /= count;

    if (frame_time_average > 0) {
        _fps = 1000.0f / frame_time_average;
    } else {
        _fps = 60;
    }
}
