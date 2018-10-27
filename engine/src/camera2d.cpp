#include "camera2d.hpp"

camera2d::camera2d()
    : _position(0.0f, 0.0f)
    , _camera_matrix(1.0f)
    , _ortho_matrix(1.0f)
    , _scale(1.0f)
    , _screen_width(640)
    , _screen_height(480)
{
}

camera2d::~camera2d()
{
}

void camera2d::init(int screen_width, int screen_height)
{
    _screen_width = screen_width;
    _screen_height = screen_height;
    _ortho_matrix = glm::ortho(0.0f, static_cast<float>(_screen_width), 0.0f, static_cast<float>(_screen_height));
}

void camera2d::update()
{
    if (_need_matrix_update) {
        //camera translation
        glm::vec3 translate(-_position.x + _screen_width / 2, -_position.y + _screen_height / 2, 0.0f);
        _camera_matrix = glm::translate(_ortho_matrix, translate);

        //camera scale
        glm::vec3 scale(_scale, _scale, 0.0f);
        _camera_matrix = glm::scale(glm::mat4(1.0f), scale) * _camera_matrix;
        //        translate = glm::vec3(_screen_width / 2, _screen_height / 2, 0.0f);
        //        _camera_matrix = glm::translate(_camera_matrix, translate);
        _need_matrix_update = false;
    }
}

void camera2d::set_position(const glm::vec2& new_pos)
{
    _position = new_pos;
    _need_matrix_update = true;
}

void camera2d::set_scale(float new_scale)
{
    _scale = new_scale;
    _need_matrix_update = true;
}

float camera2d::get_scale() const
{
    return _scale;
}

glm::vec2 camera2d::get_position() const
{
    return _position;
}

glm::mat4 camera2d::get_camera_matrix() const
{
    return _camera_matrix;
}
