#pragma once

#include <memory>
#include "Platform/Window.h"
#include "Core/Event.h"
#include "Render/Renderer.h"
#include "Render/Camera.h"
#include "Scene/Scene.h"
#include "Tools/ToolRouter.h"
#include "AI/AIPanel.h"

namespace nebula {

class Application {
public:
    Application();
    virtual ~Application();

    void Run();

    void OnEvent(Event& e);
    Window& GetWindow() { return *m_Window; }
    Renderer& GetRenderer() { return m_Renderer; }
    Camera& GetCamera() { return m_Camera; }
    Scene& GetScene() { return m_Scene; }
    ToolRouter& GetToolRouter() { return m_ToolRouter; }
    AIPanel& GetAIPanel() { return m_AIPanel; }

    static Application& Get() { return *s_Instance; }

private:
    bool OnWindowClose(WindowCloseEvent& e);
    bool OnWindowResize(WindowResizeEvent& e);

    std::unique_ptr<Window> m_Window;
    Renderer m_Renderer;
    Camera m_Camera;
    Scene m_Scene;
    ToolRouter m_ToolRouter;
    AIPanel m_AIPanel;
    bool m_Running = true;

    static Application* s_Instance;
};

// To be defined by the client
Application* CreateApplication();

} // namespace nebula
