#include "input.hpp"
#include "../utilities/debug.hpp"

Input::Input(cref<Window> window) {
    debug.info("Initializing input");
    window.set_user_pointer(this);
    window.set_key_callback([](GLFWwindow* window, i32 key, i32 scan_code, i32 action, i32 mods) {
        auto instance = (Input*)glfwGetWindowUserPointer(window);
        instance->key_callback(key, action);
    });
}

bool Input::is_key_pressed(Key key) {
    return key_states[(i32)key];
}

void Input::key_callback(i32 key, i32 action) {
    key_states[key] = action != GLFW_RELEASE;
}
