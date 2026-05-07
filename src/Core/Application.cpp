#include "Core/Application.h"
#include "Core/Log.h"
#include "Platform/Input.h"

namespace nebula {

Application* Application::s_Instance = nullptr;

Application::Application()
    : m_Camera(-640.0f, 640.0f, -360.0f, 360.0f)
{
    s_Instance = this;
    Log::Init();
    NEBULA_INFO("Nebula AI Sandbox v{0}.{1}", 0, 1);

    m_Window = std::make_unique<Window>();
    m_Window->SetEventCallback([this](Event& e) { OnEvent(e); });

    Input::Init(m_Window->GetNativeWindow());
    m_Renderer.Init();

    NEBULA_INFO("Engine initialized.");
}

Application::~Application() {
    m_Renderer.Shutdown();
}

void Application::Run() {
    NEBULA_INFO("Engine running...");
    while (m_Running) {
        m_Renderer.SetClearColor({0.1f, 0.1f, 0.12f, 1.0f});
        m_Renderer.Clear();

        m_Renderer.BeginScene(m_Camera);

        // Test rendering: a red triangle and a green quad
        m_Renderer.DrawTriangle({-200.0f, 0.0f}, {200.0f, 200.0f}, {1.0f, 0.3f, 0.3f, 1.0f});
        m_Renderer.DrawQuad({200.0f, 0.0f}, {150.0f, 150.0f}, {0.3f, 1.0f, 0.3f, 1.0f});

        m_Renderer.EndScene();

        m_Window->OnUpdate();
    }
    NEBULA_INFO("Engine shutting down.");
}

void Application::OnEvent(Event& e) {
    // Dispatch to handlers based on event type
    switch (e.GetEventType()) {
        case EventType::WindowClose:
            OnWindowClose(static_cast<WindowCloseEvent&>(e));
            break;
        case EventType::WindowResize:
            OnWindowResize(static_cast<WindowResizeEvent&>(e));
            break;
        default:
            NEBULA_TRACE("Event: {0}", e.ToString());
            break;
    }
}

bool Application::OnWindowClose(WindowCloseEvent& e) {
    m_Running = false;
    return true;
}

bool Application::OnWindowResize(WindowResizeEvent& e) {
    NEBULA_TRACE("Window resized to {0}x{1}", e.GetWidth(), e.GetHeight());
    return false;
}

} // namespace nebula
