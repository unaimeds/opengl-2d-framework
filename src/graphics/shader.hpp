#pragma once

#include <glm/mat4x2.hpp>

class Shader {
public:
    Shader(std::string_view vertexPath, std::string_view fragmentPath);
    ~Shader();

    void bind(bool state);

    void set_uniform(std::string_view name, i32 value);
    void set_uniform(std::string_view name, cref<glm::mat4> value);
private:
    u32 program;

    u32 create_shader(u32 type, const std::string& data);
};
