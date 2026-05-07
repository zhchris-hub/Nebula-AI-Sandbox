#include "Platform/Window.h"
#include "Core/Log.h"

namespace nebula {

Window::Window(const WindowProps& props) {
    Init(props);
}

Window::~Window() {
    Shutdown();
}

void Window::Init(const WindowProps& props) {
    m_Data.Title = props.Title;
    m_Data.Width = props.Width;
    m_Data.Height = props.Height;

    NEBULA_INFO("Creating window: {0} ({1}, {2})", m_Data.Title, m_Data.Width, m_Data.Height);

    if (!glfwInit()) {
        NEBULA_CRITICAL("Failed to initialize GLFW!");
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
    if (!m_Window) {
        NEBULA_CRITICAL("Failed to create GLFW window!");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_Window);
    glfwSetWindowUserPointer(m_Window, &m_Data);
    glfwSwapInterval(1); // VSync

    // ── GLFW callbacks ────────────────────────────────────────────────
    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
        auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        WindowCloseEvent event;
        data.EventCallback(event);
    });

    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
        auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        data.Width = width;
        data.Height = height;
        WindowResizeEvent event(width, height);
        data.EventCallback(event);
    });

    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        switch (action) {
            case GLFW_PRESS: {
                KeyPressedEvent event(key, false);
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE: {
                KeyReleasedEvent event(key);
                data.EventCallback(event);
                break;
            }
            case GLFW_REPEAT: {
                KeyPressedEvent event(key, true);
                data.EventCallback(event);
                break;
            }
        }
    });

    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
        auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        if (action == GLFW_PRESS) {
            MouseButtonPressedEvent event(button);
            data.EventCallback(event);
        } else {
            MouseButtonReleasedEvent event(button);
            data.EventCallback(event);
        }
    });

    glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
        auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
        data.EventCallback(event);
    });

    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
        auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
        data.EventCallback(event);
    });
}

void Window::Shutdown() {
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void Window::OnUpdate() {
    glfwPollEvents();
    glfwSwapBuffers(m_Window);
}

} // namespace nebula
