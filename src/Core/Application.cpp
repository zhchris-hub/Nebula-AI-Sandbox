#include "Core/Application.h"
#include "Core/Log.h"
#include "Platform/Input.h"

namespace nebula {

Application* Application::s_Instance = nullptr;

Application::Application()
    : m_Camera(-640.0f, 640.0f, -360.0f, 360.0f),
      m_Scene("Main Scene")
{
    s_Instance = this;
    Log::Init();
    NEBULA_INFO("Nebula AI Sandbox v{0}.{1}", 0, 1);

    m_Window = std::make_unique<Window>();
    m_Window->SetEventCallback([this](Event& e) { OnEvent(e); });

    Input::Init(m_Window->GetNativeWindow());
    m_Renderer.Init();

    // 创建测试实体
    auto redTriangle = m_Scene.CreateEntity("Red Triangle");
    m_Scene.AddTransformComponent(redTriangle, glm::vec2{-200.0f, 0.0f}, glm::vec2{200.0f, 200.0f}, 0.0f);
    m_Scene.AddSpriteComponent(redTriangle, glm::vec4{1.0f, 0.3f, 0.3f, 1.0f});
    m_Scene.AddShapeComponent(redTriangle, ShapeType::Triangle);

    auto greenQuad = m_Scene.CreateEntity("Green Quad");
    m_Scene.AddTransformComponent(greenQuad, glm::vec2{200.0f, 0.0f}, glm::vec2{150.0f, 150.0f}, 0.0f);
    m_Scene.AddSpriteComponent(greenQuad, glm::vec4{0.3f, 1.0f, 0.3f, 1.0f});
    m_Scene.AddShapeComponent(greenQuad, ShapeType::Quad);

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

        // 使用Scene渲染所有实体
        m_Scene.OnRender(m_Renderer);

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
