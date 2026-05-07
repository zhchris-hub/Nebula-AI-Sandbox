#include "Core/Application.h"
#include "Core/Log.h"
#include "Platform/Input.h"
#include "Tools/SceneTool.h"
#include "Tools/RuntimeTool.h"
#include <GLFW/glfw3.h>

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

    // Register tools
    m_ToolRouter.RegisterTool(std::make_shared<SceneTool>(&m_Scene));
    m_ToolRouter.RegisterTool(std::make_shared<RuntimeTool>(&m_Scene));

    // Initialize AI panel and agent
    m_AIPanel.Init(&m_ToolRouter);
    m_Agent.Init(&m_AIPanel, &m_ToolRouter);

    // Test entities with physics
    auto redTriangle = m_Scene.CreateEntity("Red Triangle");
    m_Scene.AddTransformComponent(redTriangle, glm::vec2{-200.0f, 200.0f}, glm::vec2{200.0f, 200.0f}, 0.0f);
    m_Scene.AddSpriteComponent(redTriangle, glm::vec4{1.0f, 0.3f, 0.3f, 1.0f});
    m_Scene.AddShapeComponent(redTriangle, ShapeType::Triangle);
    m_Scene.AddRigidBodyComponent(redTriangle, false, 1.0f, 0.3f, 0.5f);

    auto greenQuad = m_Scene.CreateEntity("Green Quad");
    m_Scene.AddTransformComponent(greenQuad, glm::vec2{200.0f, -100.0f}, glm::vec2{150.0f, 150.0f}, 0.0f);
    m_Scene.AddSpriteComponent(greenQuad, glm::vec4{0.3f, 1.0f, 0.3f, 1.0f});
    m_Scene.AddShapeComponent(greenQuad, ShapeType::Quad);
    m_Scene.AddRigidBodyComponent(greenQuad, false, 1.0f, 0.3f, 0.3f);

    // Ground (static)
    auto ground = m_Scene.CreateEntity("Ground");
    m_Scene.AddTransformComponent(ground, glm::vec2{0.0f, -300.0f}, glm::vec2{1200.0f, 20.0f}, 0.0f);
    m_Scene.AddSpriteComponent(ground, glm::vec4{0.5f, 0.5f, 0.5f, 1.0f});
    m_Scene.AddShapeComponent(ground, ShapeType::Quad);
    m_Scene.AddRigidBodyComponent(ground, true);

    NEBULA_INFO("Engine initialized with {0} tools.", m_ToolRouter.GetToolCount());
}

Application::~Application() {
    m_Renderer.Shutdown();
}

void Application::Run() {
    NEBULA_INFO("Engine running...");
    float lastTime = static_cast<float>(glfwGetTime());

    while (m_Running) {
        float currentTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Update physics
        m_Scene.OnUpdate(deltaTime);

        m_Renderer.SetClearColor({0.1f, 0.1f, 0.12f, 1.0f});
        m_Renderer.Clear();

        m_Renderer.BeginScene(m_Camera);

        // Render all entities
        m_Scene.OnRender(m_Renderer);

        m_Renderer.EndScene();

        m_Window->OnUpdate();
    }
    NEBULA_INFO("Engine shutting down.");
}

void Application::OnEvent(Event& e) {
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
