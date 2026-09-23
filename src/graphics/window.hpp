#pragma once

#include <functional>

#include <glm/vec2.hpp>
#include <GLFW/glfw3.h>

class Window {
public:
    Window(std::string_view title, cref<glm::uvec2> size);
    ~Window();

    bool is_open() const;
    void set_user_pointer(void* pointer) const;
    void set_key_callback(GLFWkeyfun callback) const;

    cref<glm::uvec2> get_size() const { return size; }
private:
    GLFWwindow* context;
    glm::uvec2 size;
};
