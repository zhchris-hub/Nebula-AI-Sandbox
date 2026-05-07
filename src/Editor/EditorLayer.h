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
        ImGui::Begin("Nebula AI Sandbox Editor", nullptr, ImGuiWindowFlags_MenuBar);

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Exit")) {
                    // TODO: trigger app close
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View")) {
                ImGui::MenuItem("Show Demo", nullptr, &m_ShowDemo);
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::Text("Welcome to Nebula AI Sandbox!");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Separator();

        if (m_Scene) {
            ImGui::Text("Scene: %s", m_Scene->GetName().c_str());
            ImGui::Text("Entities: %zu", m_Scene->GetEntityCount());
        }

        ImGui::End();

        DrawHierarchyPanel();
        DrawInspectorPanel();
        DrawConsolePanel();
        DrawViewportPanel();

        if (m_ShowDemo) {
            ImGui::ShowDemoWindow(&m_ShowDemo);
        }
    }

    void SetScene(Scene* scene) { m_Scene = scene; }

    void AddLog(const std::string& msg, const ImVec4& color = {1.0f, 1.0f, 1.0f, 1.0f}) {
        m_LogEntries.push_back({msg, color});
        if (m_LogEntries.size() > 1000) {
            m_LogEntries.erase(m_LogEntries.begin());
        }
    }

private:
    void DrawHierarchyPanel() {
        ImGui::Begin("Hierarchy");
        if (m_Scene) {
            ImGui::Text("Scene: %s", m_Scene->GetName().c_str());
            ImGui::Text("Entities: %zu", m_Scene->GetEntityCount());
        }
        ImGui::End();
    }

    void DrawInspectorPanel() {
        ImGui::Begin("Inspector");
        ImGui::Text("Select an entity to inspect");
        ImGui::End();
    }

    void DrawConsolePanel() {
        ImGui::Begin("Console");
        for (size_t i = 0; i < m_LogEntries.size(); ++i) {
            const LogEntry& entry = m_LogEntries[i];
            ImGui::TextColored(entry.Color, "%s", entry.Message.c_str());
        }
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0f);
        }
        ImGui::End();
    }

    void DrawViewportPanel() {
        ImGui::Begin("Viewport");
        ImGui::Text("Viewport rendering area");
        ImGui::End();
    }

    Scene* m_Scene = nullptr;
    bool m_ShowDemo = false;
    std::vector<LogEntry> m_LogEntries;
};

} // namespace nebula
