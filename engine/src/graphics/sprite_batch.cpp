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

void sprite_batch::begin(glyph_sort_type sort_type)
{
    _sort_type = sort_type;
    _render_batchs.clear();
    for (size_t i = 0; i < _glyphs.size(); i++) {
        delete _glyphs[i];
    }
    _glyphs.clear();
}

void sprite_batch::end()
{
    //sort_glypths();
    create_render_batchs();
}

void sprite_batch::draw(const glm::vec4& dest_rect, const glm::vec4& uv_rect, GLuint texture, float depth, const color& color)
{
    glyph* new_glyphs = new glyph;
    new_glyphs->texture = texture;
    new_glyphs->depth = depth;

    new_glyphs->top_left.col = color;
    new_glyphs->top_left.set_position(dest_rect.x, dest_rect.y + dest_rect.w);
    new_glyphs->top_left.set_uv(uv_rect.x, uv_rect.y + uv_rect.w);

    new_glyphs->bottom_left.col = color;
    new_glyphs->bottom_left.set_position(dest_rect.x, dest_rect.y);
    new_glyphs->bottom_left.set_uv(uv_rect.x, uv_rect.y);

    new_glyphs->bottom_right.col = color;
    new_glyphs->bottom_right.set_position(dest_rect.x + dest_rect.z, dest_rect.y);
    new_glyphs->bottom_right.set_uv(uv_rect.x + uv_rect.z, uv_rect.y);

    new_glyphs->top_right.col = color;
    new_glyphs->top_right.set_position(dest_rect.x + dest_rect.z, dest_rect.y + dest_rect.w);
    new_glyphs->top_right.set_uv(uv_rect.x + uv_rect.z, uv_rect.y + uv_rect.w);

    _glyphs.push_back(new_glyphs);
}

void sprite_batch::render_batch()
{
    glBindVertexArray(_vao);

    for (size_t i = 0; i < _render_batchs.size(); i++) {
        glBindTexture(GL_TEXTURE_2D, _render_batchs[i].texture);

        glDrawArrays(GL_TRIANGLES, _render_batchs[i].offset, _render_batchs[i].num_vertices);
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
    vertices.resize(_glyphs.size() * 6);

    if (_glyphs.empty()) {
        return;
    }

    unsigned long offset = 0;
    unsigned long cv = 0;
    _render_batchs.emplace_back(offset, 6, _glyphs[0]->texture);
    vertices[cv++] = _glyphs[0]->top_left;
    vertices[cv++] = _glyphs[0]->bottom_left;
    vertices[cv++] = _glyphs[0]->bottom_right;
    vertices[cv++] = _glyphs[0]->bottom_right;
    vertices[cv++] = _glyphs[0]->top_right;
    vertices[cv++] = _glyphs[0]->top_left;
    offset += 6;

    for (unsigned long cg = 1; cg < _glyphs.size(); cg++) {
        if (_glyphs[cg]->texture != _glyphs[cg - 1]->texture) {
            _render_batchs.emplace_back(offset, 6, _glyphs[cg]->texture);
        } else {
            _render_batchs.back().num_vertices += 6;
        }

        vertices[cv++] = _glyphs[cg]->top_left;
        vertices[cv++] = _glyphs[cg]->bottom_left;
        vertices[cv++] = _glyphs[cg]->bottom_right;
        vertices[cv++] = _glyphs[cg]->bottom_right;
        vertices[cv++] = _glyphs[cg]->top_right;
        vertices[cv++] = _glyphs[cg]->top_left;
        offset += 6;
    }

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), nullptr, GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(vertex), vertices.data());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void sprite_batch::sort_glypths()
{
    switch (_sort_type) {
    case glyph_sort_type::BACK_TO_FRONT:
        std::stable_sort(_glyphs.begin(), _glyphs.end(), compare_back_to_front);
        break;
    case glyph_sort_type::FRONT_TO_BACK:
        std::stable_sort(_glyphs.begin(), _glyphs.end(), compare_front_to_back);
        break;
    case glyph_sort_type::TEXTURE:
        std::stable_sort(_glyphs.begin(), _glyphs.end(), compare_texture);
        break;
    }
}

bool sprite_batch::compare_front_to_back(glyph* a, glyph* b)
{
    return (a->depth < b->depth);
}

bool sprite_batch::compare_back_to_front(glyph* a, glyph* b)
{
    return (a->depth > b->depth);
}

bool sprite_batch::compare_texture(glyph* a, glyph* b)
{
    return (a->texture < b->texture);
}
