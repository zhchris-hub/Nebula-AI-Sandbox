#pragma once

#include <string>
#include <GLFW/glfw3.h>
#include "Core/Event.h"

namespace nebula {

struct WindowProps {
    std::string Title;
    int Width;
    int Height;

    WindowProps(const std::string& title = "Nebula AI Sandbox",
                int width = 1280, int height = 720)
        : Title(title), Width(width), Height(height) {}
};

class Window {
public:
    Window(const WindowProps& props = WindowProps());
    ~Window();

    void OnUpdate();

    int GetWidth() const { return m_Data.Width; }
    int GetHeight() const { return m_Data.Height; }
    bool ShouldClose() const { return glfwWindowShouldClose(m_Window); }

    void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }
    GLFWwindow* GetNativeWindow() const { return m_Window; }

private:
    void Init(const WindowProps& props);
    void Shutdown();

    GLFWwindow* m_Window;

    struct WindowData {
        std::string Title;
        int Width, Height;
        EventCallbackFn EventCallback;
    };
    WindowData m_Data;
};

} // namespace nebula
