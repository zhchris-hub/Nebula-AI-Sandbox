#include "AI/AIPanel.h"
#include "Core/Log.h"
#include <algorithm>
#include <sstream>

namespace nebula {

AIPanel::AIPanel() {
    NEBULA_INFO("AIPanel created");
}

AIPanel::~AIPanel() {
    NEBULA_INFO("AIPanel destroyed");
}

void AIPanel::Init(ToolRouter* toolRouter) {
    m_ToolRouter = toolRouter;
    NEBULA_INFO("AIPanel initialized with {0} tools", toolRouter->GetToolCount());
}

std::string AIPanel::ProcessInput(const std::string& input) {
    if (!m_ToolRouter) {
        return "Error: AI Panel not initialized";
    }

    NEBULA_INFO("Processing input: {0}", input);

    // Parse intent from natural language
    auto [intent, params] = ParseIntent(input);

    // Execute the intent
    std::string response = ExecuteIntent(intent, params);

    // Store in history
    m_History.push_back({input, response});

    return response;
}

std::string AIPanel::ExecuteIntent(const std::string& intent, const nlohmann::json& params) {
    ToolResult result = m_ToolRouter->ExecuteTool(intent, params);
    return result.ToJson().dump(2);
}

std::pair<std::string, nlohmann::json> AIPanel::ParseIntent(const std::string& input) {
    // Simple keyword-based parsing (placeholder for LLM integration)
    std::string lower = input;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    // Scene operations
    if (lower.find("create") != std::string::npos || lower.find("add") != std::string::npos) {
        if (lower.find("entity") != std::string::npos || lower.find("object") != std::string::npos) {
            nlohmann::json params = {{"action", "create_entity"}, {"entity_name", "New Entity"}};
            return {"scene", params};
        }
    }

    if (lower.find("delete") != std::string::npos || lower.find("remove") != std::string::npos) {
        if (lower.find("entity") != std::string::npos || lower.find("object") != std::string::npos) {
            // Try to extract entity ID
            nlohmann::json params = {{"action", "destroy_entity"}};
            return {"scene", params};
        }
    }

    if (lower.find("scene") != std::string::npos && lower.find("info") != std::string::npos) {
        return {"scene", {{"action", "get_scene_info"}}};
    }

    if (lower.find("list") != std::string::npos && (lower.find("entity") != std::string::npos || lower.find("entities") != std::string::npos)) {
        return {"scene", {{"action", "get_scene_info"}}};
    }

    // Physics operations
    if (lower.find("gravity") != std::string::npos) {
        nlohmann::json params = {{"action", "set_gravity"}, {"gravity", {{"x", 0.0f}, {"y", -9.8f}}}};
        return {"runtime", params};
    }

    if (lower.find("physics") != std::string::npos && lower.find("info") != std::string::npos) {
        return {"runtime", {{"action", "get_physics_info"}}};
    }

    // Default: return scene info
    return {"scene", {{"action", "get_scene_info"}}};
}

void AIPanel::ClearHistory() {
    m_History.clear();
}

} // namespace nebula
