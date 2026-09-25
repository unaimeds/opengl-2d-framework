#include <stdexcept>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "window.hpp"
#include "../utilities/debug.hpp"

void opengl_message_callback(std::uint32_t source, std::uint32_t type, std::uint32_t id, std::uint32_t severity, int length, const char* message, const void* user_param) {
    debug.info("[OpenGL] {}", message); // TODO: use level based on severity
}

Window::Window(std::string_view title, const glm::uvec2& size) : size(size) {
    debug.info("Creating window: {}x{} {}", size.x, size.y, title);

    if (!glfwInit()) {
        debug.error("Failed to initialize GLFW");
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    context = glfwCreateWindow(size.x, size.y, title.data(), nullptr, nullptr);
    if (!context) {
        debug.error("Failed to create GLFW window");
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(context);
    glfwSwapInterval(1);

    if (const auto version = gladLoadGL(glfwGetProcAddress); version == 0)
        throw std::runtime_error("Failed to initialize OpenGL context");

    glDebugMessageCallback(opengl_message_callback, nullptr);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

Window::~Window() {
    glfwTerminate();
}

bool Window::is_open() const {
    glfwSwapBuffers(context);
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT);
    return !glfwWindowShouldClose(context);
}

void Window::set_user_pointer(void* pointer) const {
    glfwSetWindowUserPointer(context, pointer);
}

void Window::set_key_callback(GLFWkeyfun callback) const {
    glfwSetKeyCallback(context, callback);
}
