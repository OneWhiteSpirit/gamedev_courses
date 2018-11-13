#include "game.hpp"
#include "error.hpp"
#include "resource_manager.hpp"
#include "window.hpp"
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <random>

game::game()
    : _fps(0)
    , _max_fps(60.0f)
    , _frame_time(0)
    , _screen_width(640)
    , _screen_height(360)
    , time_step(1.0f / 60.0f)
    , accumulator(0.0f)
    , max_time(1.0f)
{
    _window = std::unique_ptr<one_dc::graphics::window>(new one_dc::graphics::window);
    _texture_program = std::unique_ptr<shader>(new shader);
    _game_state = std::unique_ptr<game_state>(new game_state);
    _input_manager = std::unique_ptr<input_manager>(new input_manager);
    _sprite_batch = std::unique_ptr<sprite_batch>(new sprite_batch);
    _player = std::unique_ptr<player>(new player);
    _camera = std::unique_ptr<camera2d>(new camera2d);
    _bullets = std::unique_ptr<bullets>(new bullets);
    _audio = std::unique_ptr<audio>(new audio);
}

game::~game()
{
}

void game::start()
{
    init_engine();

    init_level();

    while (*_game_state.get() != game_state::EXIT) {

        float start_tick = SDL_GetTicks();

        handle_input();

        _camera->set_position(_player->get_position());
        _player->update(_levels[_current_level]->get_level_data(), time_step);
        _camera->update();

        draw_game();

        calculateFPS();

        float frame_tick = SDL_GetTicks() - start_tick;
        if (1000.0f / _max_fps > frame_tick) {
            SDL_Delay(static_cast<Uint32>(1000.0f / _max_fps - frame_tick));
        }
    }
}

void game::init_engine()
{
    if (!_window->init("some game title", _screen_width, _screen_height)) {
        SDL_Quit();
    }

    init_shaders();

    _sprite_batch->init();

    _audio->init();
    _audio->stop_all();

    _camera->init(static_cast<int>(_screen_width / 2), static_cast<int>(_screen_height / 2));
}

void game::init_shaders()
{
    _texture_program->compile("shaders/vertex_shader.vert", "shaders/fragment_shader.frag");
    _texture_program->add_attribute("vertex_position");
    _texture_program->add_attribute("vertex_color");
    _texture_program->add_attribute("vertex_uv");
    _texture_program->link();
}

void game::init_level()
{
    _levels.push_back(std::unique_ptr<level>(new level("level1.txt")));
    _current_level = 0;

    _player->init(1.0f, _levels[_current_level]->get_start_player_pos(), _input_manager.get());

    const float HUMAN_SPEED = 5.9804f;
    std::mt19937 randomEngine;
    randomEngine.seed(time(nullptr));
    std::uniform_int_distribution<int> ranX(2, _levels[_current_level]->get_width() - 2);
    std::uniform_int_distribution<int> ranY(2, _levels[_current_level]->get_height() - 2);

    for (int i = 0; i < 10; i++) {
        _enemys.push_back(std::unique_ptr<enemy>(new enemy()));
        _enemys.back()->init(HUMAN_SPEED,
            glm::vec2(ranX(randomEngine) * TILE_WIDTH, ranY(randomEngine) * TILE_WIDTH));
    }
}

void game::draw_game()
{
    glClearDepth(1.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _texture_program->use();

    glActiveTexture(GL_TEXTURE0);
    ONE_DC_GL_CHECK();

    GLint texture_loc = _texture_program->getUniformLocation("sampler");
    ONE_DC_GL_CHECK();
    glUniform1i(texture_loc, 0);
    ONE_DC_GL_CHECK();

    GLint time_location = _texture_program->getUniformLocation("time");
    ONE_DC_GL_CHECK();
    glUniform1f(time_location, one_dc::graphics::window::get_time_from_init());
    ONE_DC_GL_CHECK();

    glm::mat4 projection_matrix = _camera->get_camera_matrix();
    GLint p_uniform = _texture_program->getUniformLocation("projection");
    glUniformMatrix4fv(p_uniform, 1, GL_FALSE, &projection_matrix[0][0]);
    ONE_DC_GL_CHECK();

    _levels[_current_level]->draw();

    _sprite_batch->begin();
    _player->draw();

    // Frame Processing
    ticksNew = SDL_GetTicks();
    _frame_time = static_cast<float>((ticksNew - ticksOld)) / 1000.0f;
    ticksOld = ticksNew;
    if (_frame_time < 0)
        _frame_time = 0;

    accumulator += _frame_time;
    if (accumulator < 0)
        accumulator = 0;
    else if (accumulator > max_time)
        accumulator = max_time;

    while (accumulator > time_step) {
        get_bullets()->update_pos(time_step);
        for (size_t i = 0; i < _enemys.size(); i++) {
            _enemys[i]->update(_levels[_current_level]->get_level_data(), time_step);
        }
        accumulator -= time_step;
    }

    for (size_t i = 0; i < _enemys.size(); i++) {
        _enemys[i]->draw();
    }
    get_bullets()->draw();

    _sprite_batch->end();
    _sprite_batch->render_batch();

    _texture_program->unuse();

    _window->update();
}

void game::handle_input()
{
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            *_game_state.get() = game_state::EXIT;
            break;
        case SDL_KEYDOWN:
            _input_manager->press_key(static_cast<unsigned int>(event.key.keysym.sym));
            break;
        case SDL_KEYUP:
            _input_manager->release_key(static_cast<unsigned int>(event.key.keysym.sym));
            break;
        case SDL_MOUSEBUTTONDOWN:
            _input_manager->press_key(event.button.button);
            break;
        case SDL_MOUSEBUTTONUP:
            _input_manager->release_key(event.button.button);
            break;
        case SDL_MOUSEMOTION:
            _input_manager->set_mouse_coordinates(event.motion.x, event.motion.y);
            break;
        }
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
