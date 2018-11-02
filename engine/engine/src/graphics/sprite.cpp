#include "sprite.hpp"
#include "error.hpp"
#include "vertex.hpp"
#include "resource_manager.hpp"
#include <GL/glew.h>
#include <cstdlib>

sprite::sprite()
    : _vbo_id(0)
{
}

sprite::~sprite()
{
    if (_vbo_id != 0) {
        glDeleteBuffers(1, &_vbo_id);
        ONE_DC_GL_CHECK();
    }
}

void sprite::init(float x, float y, float width, float height, const char* texture_path)
{
    _x = x;
    _y = y;
    _width = width;
    _height = height;

    _texture = resource_manager::get_texture(texture_path);

    if (_vbo_id == 0) {
        glGenBuffers(1, &_vbo_id);
    }

    vertex vertex_d[6];
    for (int i = 0; i < 6; i++) {
        vertex_d[i].set_color(255, 255, 0, 255);
    }

    vertex_d[0].set_position(x + width, y + height);
    vertex_d[0].set_uv(1.0f, 1.0f);

    vertex_d[1].set_position(x, y + height);
    vertex_d[1].set_uv(0.0f, 1.0f);

    vertex_d[2].set_position(x, y);
    vertex_d[2].set_uv(0.0f, 0.0f);

    vertex_d[3].set_position(x, y);
    vertex_d[3].set_uv(0.0f, 0.0f);

    vertex_d[4].set_position(x + width, y);
    vertex_d[4].set_uv(1.0f, 0.0f);

    vertex_d[5].set_position(x + width, y + height);
    vertex_d[5].set_uv(1.0f, 1.0f);

    vertex_d[0].set_color(0, 255, 0, 255);

    vertex_d[5].set_color(0, 255, 0, 255);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo_id);
    ONE_DC_GL_CHECK();

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_d), vertex_d, GL_STATIC_DRAW);
    ONE_DC_GL_CHECK();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    ONE_DC_GL_CHECK();
}

void sprite::draw()
{
    glBindTexture(GL_TEXTURE_2D, _texture.id);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo_id);
    ONE_DC_GL_CHECK();

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

    glDrawArrays(GL_TRIANGLES, 0, 6);
    ONE_DC_GL_CHECK();

    glDisableVertexAttribArray(0);
    ONE_DC_GL_CHECK();

    glDisableVertexAttribArray(1);
    ONE_DC_GL_CHECK();

    glDisableVertexAttribArray(2);
    ONE_DC_GL_CHECK();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    ONE_DC_GL_CHECK();
}
