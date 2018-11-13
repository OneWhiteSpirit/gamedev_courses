#include "sprite_batch.hpp"
#include "error.hpp"
#include <GL/glew.h>
#include <algorithm>

sprite_batch::sprite_batch()
    : _vbo(0)
    , _vao(0)
{
}

sprite_batch::~sprite_batch()
{
}

void sprite_batch::init()
{
    create_vertex_array();
}

void sprite_batch::begin()
{
    _render_batchs.clear();
    for (size_t i = 0; i < _sprite_datas.size(); i++) {
        delete _sprite_datas[i];
    }
    _sprite_datas.clear();
}

void sprite_batch::end()
{
    create_render_batchs();
}

void sprite_batch::draw(const glm::vec4& dest_rect, const glm::vec4& uv_rect, GLuint texture, const color& color)
{
    sprite_data* new_sprite_datas = new sprite_data;
    new_sprite_datas->texture = texture;
    new_sprite_datas->depth = 0.0f;

    new_sprite_datas->top_left.col = color;
    new_sprite_datas->top_left.set_position(dest_rect.x, dest_rect.y + dest_rect.w);
    new_sprite_datas->top_left.set_uv(uv_rect.x, uv_rect.y + uv_rect.w);

    new_sprite_datas->bottom_left.col = color;
    new_sprite_datas->bottom_left.set_position(dest_rect.x, dest_rect.y);
    new_sprite_datas->bottom_left.set_uv(uv_rect.x, uv_rect.y);

    new_sprite_datas->bottom_right.col = color;
    new_sprite_datas->bottom_right.set_position(dest_rect.x + dest_rect.z, dest_rect.y);
    new_sprite_datas->bottom_right.set_uv(uv_rect.x + uv_rect.z, uv_rect.y);

    new_sprite_datas->top_right.col = color;
    new_sprite_datas->top_right.set_position(dest_rect.x + dest_rect.z, dest_rect.y + dest_rect.w);
    new_sprite_datas->top_right.set_uv(uv_rect.x + uv_rect.z, uv_rect.y + uv_rect.w);

    _sprite_datas.push_back(new_sprite_datas);
}

void sprite_batch::render_batch()
{
    glBindVertexArray(_vao);

    for (size_t i = 0; i < _render_batchs.size(); i++) {
        glBindTexture(GL_TEXTURE_2D, _render_batchs[i].texture);

        glDrawArrays(GL_TRIANGLES, static_cast<GLint>(_render_batchs[i].offset), static_cast<GLsizei>(_render_batchs[i].num_vertices));
    }

    glBindVertexArray(0);
}

void sprite_batch::create_vertex_array()
{
    if (_vao == 0) {
        glGenVertexArrays(1, &_vao);
    }
    glBindVertexArray(_vao);

    if (_vbo == 0) {
        glGenBuffers(1, &_vbo);
    }

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glEnableVertexAttribArray(0);
    ONE_DC_GL_CHECK();

    glEnableVertexAttribArray(1);
    ONE_DC_GL_CHECK();

    glEnableVertexAttribArray(2);
    ONE_DC_GL_CHECK();

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, pos)));
    ONE_DC_GL_CHECK();

    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, col)));
    ONE_DC_GL_CHECK();

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, uv)));
    ONE_DC_GL_CHECK();

    glBindVertexArray(0);
}

void sprite_batch::create_render_batchs()
{
    std::vector<vertex> vertices;
    vertices.resize(_sprite_datas.size() * 6);

    if (_sprite_datas.empty()) {
        return;
    }

    unsigned long offset = 0;
    unsigned long cv = 0;
    _render_batchs.emplace_back(offset, 6, _sprite_datas[0]->texture);
    vertices[cv++] = _sprite_datas[0]->top_left;
    vertices[cv++] = _sprite_datas[0]->bottom_left;
    vertices[cv++] = _sprite_datas[0]->bottom_right;
    vertices[cv++] = _sprite_datas[0]->bottom_right;
    vertices[cv++] = _sprite_datas[0]->top_right;
    vertices[cv++] = _sprite_datas[0]->top_left;
    offset += 6;

    for (unsigned long cg = 1; cg < _sprite_datas.size(); cg++) {
        if (_sprite_datas[cg]->texture != _sprite_datas[cg - 1]->texture) {
            _render_batchs.emplace_back(offset, 6, _sprite_datas[cg]->texture);
        } else {
            _render_batchs.back().num_vertices += 6;
        }

        vertices[cv++] = _sprite_datas[cg]->top_left;
        vertices[cv++] = _sprite_datas[cg]->bottom_left;
        vertices[cv++] = _sprite_datas[cg]->bottom_right;
        vertices[cv++] = _sprite_datas[cg]->bottom_right;
        vertices[cv++] = _sprite_datas[cg]->top_right;
        vertices[cv++] = _sprite_datas[cg]->top_left;
        offset += 6;
    }

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(vertex)), nullptr, GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(vertices.size() * sizeof(vertex)), vertices.data());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
