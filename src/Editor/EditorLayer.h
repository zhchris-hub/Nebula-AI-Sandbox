#pragma once

#include <imgui.h>
#include <string>
#include <vector>

namespace nebula {

class Scene;

struct LogEntry {
    std::string Message;
    ImVec4 Color;
};

class EditorLayer {
public:
    EditorLayer() = default;
    ~EditorLayer() = default;

    void OnAttach() {}
    void OnDetach() {}
    void OnUpdate(float deltaTime) { (void)deltaTime; }

    void OnImGuiRender() {
        // 简单的 ImGui 窗口
        ImGui::Begin("Nebula AI Sandbox Editor");
        ImGui::Text("Welcome to Nebula AI Sandbox!");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::End();

        // 绘制面板
        DrawHierarchyPanel();
        DrawInspectorPanel();
        DrawConsolePanel();
        DrawViewportPanel();
    }

    void SetScene(Scene* scene) { m_Scene = scene; }

private:
    void DrawHierarchyPanel() {
        ImGui::Begin("Hierarchy");

        if (m_Scene) {
            // TODO: 显示场景中的实体列表
            ImGui::Text("Scene: %s", m_Scene->GetName().c_str());
            ImGui::Text("Entities: %zu", m_Scene->GetEntityCount());
        }

        ImGui::End();
    }

    void DrawInspectorPanel() {
        ImGui::Begin("Inspector");

        // TODO: 显示选中实体的组件信息

        ImGui::End();
    }

    void DrawConsolePanel() {
        ImGui::Begin("Console");

        // 显示日志
        for (size_t i = 0; i < m_LogEntries.size(); ++i) {
            const LogEntry& entry = m_LogEntries[i];
            ImGui::TextColored(entry.Color, "%s", entry.Message.c_str());
        }

        // 自动滚动到底部
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::End();
    }

    void DrawViewportPanel() {
        ImGui::Begin("Viewport");

        // TODO: 显示渲染视口

        ImGui::End();
    }

    Scene* m_Scene = nullptr;
    bool m_ShowDemo = false;

    // 控制台日志
    std::vector<LogEntry> m_LogEntries;
};

} // namespace nebula
