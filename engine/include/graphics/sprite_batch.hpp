#pragma once

#include "vertex.hpp"
#include <glm/glm.hpp>
#include <vector>

typedef unsigned int GLuint;

enum class glyph_sort_type {
    NONE,
    FRONT_TO_BACK,
    BACK_TO_FRONT,
    TEXTURE
};

struct glyph {
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

    void begin(glyph_sort_type sort_type = glyph_sort_type::TEXTURE);
    void end();

    void draw(const glm::vec4& dest_rect, const glm::vec4& uv_rect, GLuint texture, float depth, const color& color);

    void render_batch();

private:
    GLuint _vbo;
    GLuint _vao;
    glyph_sort_type _sort_type;
    std::vector<glyph*> _glyphs;
    std::vector<renderbatch> _render_batchs;

    void create_vertex_array();
    void create_render_batchs();
    void sort_glypths();

    static bool compare_front_to_back(glyph* a, glyph* b);
    static bool compare_back_to_front(glyph* a, glyph* b);
    static bool compare_texture(glyph* a, glyph* b);
};
