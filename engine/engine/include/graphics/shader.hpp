#pragma once

typedef unsigned int GLuint;
typedef int GLint;

class shader
{
public:
    shader();

    void compile(const char* vertex_shader_file_path, const char* fragment_shader_file_path);
    void add_attribute(const char* attr_name);
    void link();
    GLint getUniformLocation(const char* uniform_name);

    void use();
    void unuse();
private:
    GLuint _num_attributes;
    GLuint _program_id;
    GLuint _vertex_shader_id;
    GLuint _fragment_shader_id;
};
