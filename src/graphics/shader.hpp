#pragma once

#include <glm/mat4x2.hpp>

class Shader {
public:
    Shader(std::string_view vertexPath, std::string_view fragmentPath);
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    void bind(bool state);

    void set_uniform(std::string_view name, int value);
    void set_uniform(std::string_view name, const glm::mat4& value);
private:
    std::uint32_t program;

    std::uint32_t create_shader(std::uint32_t type, const std::string& data);
};
