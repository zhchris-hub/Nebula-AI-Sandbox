#include "Platform/Input.h"

namespace nebula {

GLFWwindow* Input::s_Window = nullptr;

void Input::Init(GLFWwindow* window) {
    s_Window = window;
}

bool Input::IsKeyPressed(int keycode) {
    auto state = glfwGetKey(s_Window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(int button) {
    auto state = glfwGetMouseButton(s_Window, button);
    return state == GLFW_PRESS;
}

glm::vec2 Input::GetMousePosition() {
    double x, y;
    glfwGetCursorPos(s_Window, &x, &y);
    return { static_cast<float>(x), static_cast<float>(y) };
}

float Input::GetMouseX() {
    return GetMousePosition().x;
}

float Input::GetMouseY() {
    return GetMousePosition().y;
}

} // namespace nebula
