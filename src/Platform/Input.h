#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace nebula {

class Input {
public:
    static void Init(GLFWwindow* window);

    static bool IsKeyPressed(int keycode);
    static bool IsMouseButtonPressed(int button);
    static glm::vec2 GetMousePosition();
    static float GetMouseX();
    static float GetMouseY();

private:
    static GLFWwindow* s_Window;
};

} // namespace nebula
