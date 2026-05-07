#include "Core/Application.h"
#include "Core/Log.h"
#include "Platform/Input.h"
#include "Tools/SceneTool.h"
#include "Tools/RuntimeTool.h"
#include "AI/DemoScene.h"
#include "Editor/EditorLayer.h"
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace nebula {

Application* Application::s_Instance = nullptr;

Application::Application()
    : m_Camera(-640.0f, 640.0f, -360.0f, 360.0f),
      m_Scene("Main Scene"),
      m_Editor(std::make_unique<EditorLayer>())
{
    s_Instance = this;
    Log::Init();
    NEBULA_INFO("Nebula AI Sandbox v{0}.{1}", 0, 1);

    m_Window = std::make_unique<Window>();
    m_Window->SetEventCallback([this](Event& e) { OnEvent(e); });

    Input::Init(m_Window->GetNativeWindow());
    m_Renderer.Init();

    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_Window->GetNativeWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Register tools
    m_ToolRouter.RegisterTool(std::make_shared<SceneTool>(&m_Scene));
    m_ToolRouter.RegisterTool(std::make_shared<RuntimeTool>(&m_Scene));

    // Initialize AI panel and agent
    m_AIPanel.Init(&m_ToolRouter);
    m_Agent.Init(&m_AIPanel, &m_ToolRouter);

    // Setup editor
    m_Editor->SetScene(&m_Scene);
    m_Editor->OnAttach();

    // Setup demo scene
    DemoScene demo;
    demo.Setup(m_Scene);

    NEBULA_INFO("Engine initialized with {0} tools.", m_ToolRouter.GetToolCount());
}

Application::~Application() {
    m_Editor->OnDetach();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

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

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Render editor UI
        m_Editor->OnImGuiRender();

        // Render scene
        m_Renderer.SetClearColor({0.1f, 0.1f, 0.12f, 1.0f});
        m_Renderer.Clear();
        m_Renderer.BeginScene(m_Camera);
        m_Scene.OnRender(m_Renderer);
        m_Renderer.EndScene();

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
