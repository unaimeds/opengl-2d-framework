#pragma once

#include <glm/vec2.hpp>
#include <GLFW/glfw3.h>

class Window {
public:
    Window(std::string_view title, const glm::uvec2& size);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool is_open() const;
    void set_user_pointer(void* pointer) const;
    void set_key_callback(GLFWkeyfun callback) const;

    const glm::uvec2& get_size() const { return size; }
private:
    GLFWwindow* context;
    glm::uvec2 size;
};
