#pragma once

#include "glm/glm.hpp"
#include "vertex.hpp"
#include <vector>

typedef unsigned int GLuint;

struct sprite_data {
    GLuint texture;
    float depth;

    vertex top_left;
    vertex bottom_left;
    vertex top_right;
    vertex bottom_right;
};

class renderbatch {
public:
    renderbatch(GLuint offset_, GLuint num_vertices_, GLuint texture_)
        : offset(offset_)
        , num_vertices(num_vertices_)
        , texture(texture_)
    {
    }
    GLuint offset;
    GLuint num_vertices;
    GLuint texture;
};

class sprite_batch {
public:
    sprite_batch();
    ~sprite_batch();

    void init();

    void begin();
    void end();

    void draw(const glm::vec4& dest_rect, const glm::vec4& uv_rect, GLuint texture, const color& color);

    void render_batch();

private:
    GLuint _vbo;
    GLuint _vao;
    std::vector<sprite_data*> _sprite_datas;
    std::vector<renderbatch> _render_batchs;

    void create_vertex_array();
    void create_render_batchs();
};
