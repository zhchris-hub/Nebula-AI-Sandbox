#pragma once

#include <imgui.h>
#include <string>
#include <vector>
#include <functional>

namespace nebula {

class Scene;
class AIPanel;

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
        // Main menu bar
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Exit")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View")) {
                ImGui::MenuItem("Hierarchy", nullptr, &m_ShowHierarchy);
                ImGui::MenuItem("Inspector", nullptr, &m_ShowInspector);
                ImGui::MenuItem("Console", nullptr, &m_ShowConsole);
                ImGui::MenuItem("AI Chat", nullptr, &m_ShowAIChat);
                ImGui::MenuItem("Demo Window", nullptr, &m_ShowDemo);
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        // Status bar
        ImGui::Begin("Status");
        ImGui::Text("Nebula AI Sandbox | FPS: %.1f", ImGui::GetIO().Framerate);
        if (m_Scene) {
            ImGui::SameLine();
            ImGui::Text("| Scene: %s | Entities: %zu", m_Scene->GetName().c_str(), m_Scene->GetEntityCount());
        }
        ImGui::End();

        // Panels
        if (m_ShowHierarchy) DrawHierarchyPanel();
        if (m_ShowInspector) DrawInspectorPanel();
        if (m_ShowConsole) DrawConsolePanel();
        if (m_ShowAIChat) DrawAIChatPanel();

        if (m_ShowDemo) {
            ImGui::ShowDemoWindow(&m_ShowDemo);
        }
    }

    void SetScene(Scene* scene) { m_Scene = scene; }
    void SetAIPanel(AIPanel* aiPanel) { m_AIPanel = aiPanel; }

    void AddLog(const std::string& msg, const ImVec4& color = {1.0f, 1.0f, 1.0f, 1.0f}) {
        m_LogEntries.push_back({msg, color});
        if (m_LogEntries.size() > 1000) {
            m_LogEntries.erase(m_LogEntries.begin());
        }
    }

    void SetSelectedEntity(uint32_t id) { m_SelectedEntity = id; }
    uint32_t GetSelectedEntity() const { return m_SelectedEntity; }

private:
    void DrawHierarchyPanel() {
        ImGui::Begin("Hierarchy", &m_ShowHierarchy);
        if (m_Scene) {
            ImGui::Text("Scene: %s", m_Scene->GetName().c_str());
            ImGui::Separator();

            const auto& entities = m_Scene->GetEntities();
            for (size_t i = 0; i < entities.size(); i++) {
                auto& entity = entities[i];
                bool selected = (m_SelectedEntity == entity.GetID());

                if (m_Scene->HasTransformComponent(entity)) {
                    auto& t = m_Scene->GetTransformComponent(entity);
                    ImGui::TextColored({0.7f, 0.7f, 1.0f, 1.0f}, "[%zu]", i);
                    ImGui::SameLine();
                }

                char label[64];
                snprintf(label, sizeof(label), "Entity_%u", entity.GetID());
                if (ImGui::Selectable(label, selected)) {
                    m_SelectedEntity = entity.GetID();
                }
            }
        }
        ImGui::End();
    }

    void DrawInspectorPanel() {
        ImGui::Begin("Inspector", &m_ShowInspector);

        if (m_SelectedEntity != 0 && m_Scene) {
            ImGui::Text("Entity ID: %u", m_SelectedEntity);
            ImGui::Separator();

            // Find entity
            for (auto& entity : m_Scene->GetEntities()) {
                if (entity.GetID() == m_SelectedEntity) {
                    if (m_Scene->HasTransformComponent(entity)) {
                        auto& t = m_Scene->GetTransformComponent(entity);
                        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
                            ImGui::DragFloat2("Position", &t.Position.x, 1.0f);
                            ImGui::DragFloat2("Scale", &t.Scale.x, 1.0f);
                            ImGui::SliderAngle("Rotation", &t.Rotation);
                        }
                    }

                    if (m_Scene->HasSpriteComponent(entity)) {
                        auto& s = m_Scene->GetSpriteComponent(entity);
                        if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen)) {
                            ImGui::ColorEdit4("Color", &s.Color.x);
                        }
                    }

                    if (m_Scene->HasRigidBodyComponent(entity)) {
                        auto& rb = m_Scene->GetRigidBodyComponent(entity);
                        if (ImGui::CollapsingHeader("RigidBody")) {
                            ImGui::Text("Static: %s", rb.IsStatic ? "Yes" : "No");
                            ImGui::Text("Density: %.2f", rb.Density);
                            ImGui::Text("Friction: %.2f", rb.Friction);
                            ImGui::Text("Restitution: %.2f", rb.Restitution);
                        }
                    }
                    break;
                }
            }
        } else {
            ImGui::Text("No entity selected");
            ImGui::Text("Click an entity in the Hierarchy");
        }

        ImGui::End();
    }

    void DrawConsolePanel() {
        ImGui::Begin("Console", &m_ShowConsole);

        if (ImGui::Button("Clear")) {
            m_LogEntries.clear();
        }
        ImGui::SameLine();
        ImGui::Text("Entries: %zu", m_LogEntries.size());
        ImGui::Separator();

        for (size_t i = 0; i < m_LogEntries.size(); ++i) {
            const LogEntry& entry = m_LogEntries[i];
            ImGui::TextColored(entry.Color, "%s", entry.Message.c_str());
        }

        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::End();
    }

    void DrawAIChatPanel() {
        ImGui::Begin("AI Chat", &m_ShowAIChat);

        // Chat history
        ImGui::BeginChild("ChatHistory", ImVec2(0, -60), true);
        for (const auto& [user, response] : m_ChatHistory) {
            ImGui::TextColored({0.5f, 1.0f, 0.5f, 1.0f}, "You: %s", user.c_str());
            ImGui::TextColored({0.8f, 0.8f, 1.0f, 1.0f}, "AI: %s", response.c_str());
            ImGui::Separator();
        }
        ImGui::EndChild();

        // Input
        ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;
        bool enterPressed = ImGui::InputText("##Input", m_InputBuffer, sizeof(m_InputBuffer), flags);
        ImGui::SameLine();
        if (ImGui::Button("Send") || enterPressed) {
            if (strlen(m_InputBuffer) > 0 && m_AIPanel) {
                std::string input = m_InputBuffer;
                std::string response = m_AIPanel->ProcessInput(input);
                m_ChatHistory.push_back({input, response});
                m_InputBuffer[0] = '\0';
            }
        }

        ImGui::End();
    }

    Scene* m_Scene = nullptr;
    AIPanel* m_AIPanel = nullptr;
    bool m_ShowHierarchy = true;
    bool m_ShowInspector = true;
    bool m_ShowConsole = true;
    bool m_ShowAIChat = true;
    bool m_ShowDemo = false;
    uint32_t m_SelectedEntity = 0;
    std::vector<LogEntry> m_LogEntries;
    std::vector<std::pair<std::string, std::string>> m_ChatHistory;
    char m_InputBuffer[512] = "";
};

} // namespace nebula
