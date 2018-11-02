#include "shader.hpp"
#include "error.hpp"
#include <GL/glew.h>
#include <fstream>
#include <vector>

shader::shader()
    : _num_attributes(0)
    , _program_id(0)
    , _vertex_shader_id(0)
    , _fragment_shader_id(0)
{
}

void shader::compile(const char* vertex_shader_file_path, const char* fragment_shader_file_path)
{
    //get shader prog object
    _program_id = glCreateProgram();
    ONE_DC_GL_CHECK();
    if (0 == _program_id) {
        cerr << "failed to create gl program";
    }

    // Create the shaders
    _vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    ONE_DC_GL_CHECK()
    _fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    ONE_DC_GL_CHECK()

    // Read the Vertex Shader code from the file
    std::ifstream vertex_file(vertex_shader_file_path, std::ios_base::in);
    if (vertex_file.fail()) {
        throw std::runtime_error("can't load vertex shader");
    }

    std::string vertex_file_content;
    if (vertex_file.is_open()) {
        std::string line = "";
        while (std::getline(vertex_file, line))
            vertex_file_content += line + "\n";
        vertex_file.close();
    }

    // Read the Fragment Shader code from the file
    std::ifstream fragment_file(fragment_shader_file_path, std::ios_base::in);
    if (fragment_file.fail()) {
        throw std::runtime_error("can't load fragment shader");
    }

    std::string fragment_file_content;
    if (fragment_file.is_open()) {
        std::string line = "";
        while (getline(fragment_file, line))
            fragment_file_content += line + "\n";
        fragment_file.close();
    }

    GLint compiled_status = GL_FALSE;

    // Compile Vertex Shader    
    char const* vertex_source_pointer = vertex_file_content.c_str();
    glShaderSource(_vertex_shader_id, 1, &vertex_source_pointer, nullptr);
    glCompileShader(_vertex_shader_id);

    // Check Vertex Shader
    glGetShaderiv(_vertex_shader_id, GL_COMPILE_STATUS, &compiled_status);
    ONE_DC_GL_CHECK();
    if (compiled_status == 0) {
        GLint info_len = 0;
        glGetShaderiv(_vertex_shader_id, GL_INFO_LOG_LENGTH, &info_len);
        ONE_DC_GL_CHECK();
        std::vector<char> info_chars(static_cast<size_t>(info_len));
        glGetShaderInfoLog(_vertex_shader_id, info_len, nullptr, info_chars.data());
        ONE_DC_GL_CHECK();
        glDeleteShader(_vertex_shader_id);
        ONE_DC_GL_CHECK();

        cerr << "Error compiling shader(vertex)\n"
             << info_chars.data();
        return;
    }

    // Compile Fragment Shader    
    char const* fragment_source_pointer = fragment_file_content.c_str();
    glShaderSource(_fragment_shader_id, 1, &fragment_source_pointer, nullptr);
    glCompileShader(_fragment_shader_id);

    // Check Fragment Shader
    glGetShaderiv(_fragment_shader_id, GL_COMPILE_STATUS, &compiled_status);
    ONE_DC_GL_CHECK();
    if (compiled_status == 0) {
        GLint info_len = 0;
        glGetShaderiv(_fragment_shader_id, GL_INFO_LOG_LENGTH, &info_len);
        ONE_DC_GL_CHECK();
        std::vector<char> info_chars(static_cast<size_t>(info_len));
        glGetShaderInfoLog(_fragment_shader_id, info_len, nullptr, info_chars.data());
        ONE_DC_GL_CHECK();
        glDeleteShader(_fragment_shader_id);
        ONE_DC_GL_CHECK();

        cerr << "Error compiling shader(vertex)\n"
             << info_chars.data();
        return;
    }
}

void shader::add_attribute(const char* attr_name)
{
    glBindAttribLocation(_program_id, _num_attributes++, attr_name);
}

void shader::link()
{
    glAttachShader(_program_id, _vertex_shader_id);
    ONE_DC_GL_CHECK();

    glAttachShader(_program_id, _fragment_shader_id);
    ONE_DC_GL_CHECK();

    //    // bind attribute location
    //    glBindAttribLocation(_program_id, 0, "vertex_position");
    //    ONE_DC_GL_CHECK();

    glLinkProgram(_program_id);
    ONE_DC_GL_CHECK();

    GLint link_status = GL_FALSE;

    // Check the program
    glGetProgramiv(_program_id, GL_LINK_STATUS, &link_status);
    ONE_DC_GL_CHECK();

    if (link_status == GL_FALSE) {
        GLint info_length = 0;
        glGetProgramiv(_program_id, GL_INFO_LOG_LENGTH, &info_length);
        ONE_DC_GL_CHECK();

        std::vector<char> info_log(static_cast<size_t>(info_length));
        glGetProgramInfoLog(_program_id, info_length, &info_length, &info_log[0]);
        ONE_DC_GL_CHECK();

        glDeleteProgram(_program_id);
        ONE_DC_GL_CHECK();

        glDeleteShader(_vertex_shader_id);
        ONE_DC_GL_CHECK();

        glDeleteShader(_fragment_shader_id);
        ONE_DC_GL_CHECK();

        return;
    }

    glDetachShader(_program_id, _vertex_shader_id);
    ONE_DC_GL_CHECK();

    glDetachShader(_program_id, _fragment_shader_id);
    ONE_DC_GL_CHECK();

    glDeleteShader(_vertex_shader_id);
    ONE_DC_GL_CHECK();

    glDeleteShader(_fragment_shader_id);
    ONE_DC_GL_CHECK();
}

GLint shader::getUniformLocation(const char *uniform_name)
{
    GLint uniform_location = glGetUniformLocation(_program_id, uniform_name);
    ONE_DC_GL_CHECK();

    return uniform_location;
}

void shader::use()
{
    glUseProgram(_program_id);
    ONE_DC_GL_CHECK();
    for (GLuint i = 0; i < _num_attributes; i++) {
        glEnableVertexAttribArray(i);
    }
}

void shader::unuse()
{
    glUseProgram(0);
    for (GLuint i = 0; i < _num_attributes; i++) {
        glDisableVertexAttribArray(i);
    }
}
