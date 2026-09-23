#include <vector>
#include <format>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "shader.hpp"
#include "../io/resources.hpp"
#include "../utilities/debug.hpp"

Shader::Shader(std::string_view vertexPath, std::string_view fragmentPath) {
    debug.info("Creating shader");
    program = glCreateProgram();

    debug.info("Compling vertex shader: {}", vertexPath);
    auto vertexResource = resources::get_resource(vertexPath);
    auto vertexShader = create_shader(GL_VERTEX_SHADER, vertexResource.data);
    glAttachShader(program, vertexShader);

    debug.info("Compling fragment shader: {}", fragmentPath);
    auto fragmentResource = resources::get_resource(fragmentPath);
    auto fragmentShader = create_shader(GL_FRAGMENT_SHADER, fragmentResource.data);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);

    int status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint logLen;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
        std::vector<char> errorLog(logLen);
        int written;
        glGetProgramInfoLog(program, logLen, &written, &errorLog[0]);

        debug.error("Failed to link program: {}", errorLog.data());
        throw std::runtime_error(std::format("Failed to link program: {}", errorLog.data()));
    }

    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(program);
}

u32 Shader::create_shader(u32 type, const std::string& data) {
    auto data_ptr = data.c_str();

    auto id = glCreateShader(type);
    glShaderSource(id, 1, &data_ptr, nullptr);
    glCompileShader(id);

    i32 is_compiled = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &is_compiled);
    if (is_compiled == GL_FALSE) {
        i32 max_length = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &max_length);
        std::vector<char> error_log(max_length);
        glGetShaderInfoLog(id, max_length, &max_length, &error_log[0]);
        glDeleteShader(id);

        debug.error("Failed to compile shader: {}", error_log.data());
        throw std::runtime_error(std::format("Failed to compile shader: {}", error_log.data()));
    }

    return id;
}

void Shader::bind(bool state) {
//    if (bound == state)
//        return;
//
//    glUseProgram(bound ? program : 0);
//    bound = state;
    glUseProgram(state ? program : 0);
}

void Shader::set_uniform(std::string_view name, i32 value) {
    debug.info("Setting uniform: {} = {}", name, value);
    auto location = glGetUniformLocation(program, name.data());
    glUniform1i(location, value);
}

void Shader::set_uniform(std::string_view name, cref<glm::mat4> value) {
    debug.info("Setting uniform: {} = {}", name, glm::to_string(value));
    auto location = glGetUniformLocation(program, name.data());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
