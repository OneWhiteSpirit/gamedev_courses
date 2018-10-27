#pragma once

#include <glm/vec2.hpp>
#include <glm/gtc/matrix_transform.hpp>

class camera2d
{
public:
    camera2d();
    ~camera2d();

    void init(int screen_width, int screen_height);

    void update();

    void set_position(const glm::vec2& new_pos);

    void set_scale(float new_scale);

    float get_scale() const;

    glm::vec2 get_position() const;

    glm::mat4 get_camera_matrix() const;

private:
    int _screen_width;
    int _screen_height;
    bool _need_matrix_update;
    float _scale;
    glm::vec2 _position;
    glm::mat4 _camera_matrix;
    glm::mat4 _ortho_matrix;
};
