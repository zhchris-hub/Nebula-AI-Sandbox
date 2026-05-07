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

    auto [intent, params] = ParseIntent(input);
    std::string response = ExecuteIntent(intent, params);
    m_History.push_back({input, response});

    return response;
}

std::string AIPanel::ExecuteIntent(const std::string& intent, const nlohmann::json& params) {
    if (intent == "__help") {
        nlohmann::json result = {
            {"success", true},
            {"message", "Available tools"},
            {"data", params["tools"]}
        };
        return result.dump(2);
    }

    ToolResult result = m_ToolRouter->ExecuteTool(intent, params);
    return result.ToJson().dump(2);
}

std::pair<std::string, nlohmann::json> AIPanel::ParseIntent(const std::string& input) {
    std::string lower = input;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    // Help / tool list
    if (lower.find("help") != std::string::npos || lower.find("tools") != std::string::npos) {
        nlohmann::json tools = m_ToolRouter->GetToolList();
        return {"__help", {{"tools", tools}}};
    }

    // Scene: create entity
    if ((lower.find("create") != std::string::npos || lower.find("add") != std::string::npos || lower.find("spawn") != std::string::npos) &&
        (lower.find("entity") != std::string::npos || lower.find("object") != std::string::npos || lower.find("box") != std::string::npos || lower.find("triangle") != std::string::npos)) {
        std::string name = "New Entity";
        if (lower.find("box") != std::string::npos) name = "Box";
        else if (lower.find("triangle") != std::string::npos) name = "Triangle";

        nlohmann::json params = {{"action", "create_entity"}, {"entity_name", name}};

        // Parse position if mentioned
        if (lower.find("at") != std::string::npos || lower.find("position") != std::string::npos) {
            params["position"] = {{"x", 0.0f}, {"y", 0.0f}};
        }

        return {"scene", params};
    }

    // Scene: delete entity
    if ((lower.find("delete") != std::string::npos || lower.find("remove") != std::string::npos || lower.find("destroy") != std::string::npos) &&
        (lower.find("entity") != std::string::npos || lower.find("object") != std::string::npos)) {
        return {"scene", {{"action", "destroy_entity"}}};
    }

    // Scene: get info
    if (lower.find("scene") != std::string::npos || lower.find("info") != std::string::npos ||
        lower.find("status") != std::string::npos || lower.find("list") != std::string::npos) {
        return {"scene", {{"action", "get_scene_info"}}};
    }

    // Physics: set gravity
    if (lower.find("gravity") != std::string::npos) {
        float gx = 0.0f, gy = -9.8f;
        if (lower.find("zero") != std::string::npos || lower.find("no") != std::string::npos || lower.find("disable") != std::string::npos) {
            gx = 0.0f; gy = 0.0f;
        } else if (lower.find("up") != std::string::npos) {
            gx = 0.0f; gy = 9.8f;
        }
        return {"runtime", {{"action", "set_gravity"}, {"gravity", {{"x", gx}, {"y", gy}}}}};
    }

    // Physics: info
    if (lower.find("physics") != std::string::npos) {
        return {"runtime", {{"action", "get_physics_info"}}};
    }

    // Scripting: run code
    if (lower.find("lua") != std::string::npos || lower.find("script") != std::string::npos) {
        if (lower.find("run") != std::string::npos || lower.find("execute") != std::string::npos) {
            return {"scripting", {{"action", "run_code"}, {"code", "print('Hello from Lua!')"}}};
        }
    }

    // Default: scene info
    return {"scene", {{"action", "get_scene_info"}}};
}

void AIPanel::ClearHistory() {
    m_History.clear();
}

} // namespace nebula
