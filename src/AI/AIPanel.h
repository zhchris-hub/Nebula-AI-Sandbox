#pragma once

#include "Tools/ToolRouter.h"
#include <string>
#include <vector>

namespace nebula {

class AIPanel {
public:
    AIPanel();
    ~AIPanel();

    // Initialize with tool router
    void Init(ToolRouter* toolRouter);

    // Process user input (natural language)
    std::string ProcessInput(const std::string& input);

    // Get conversation history
    const std::vector<std::pair<std::string, std::string>>& GetHistory() const { return m_History; }

    // Clear history
    void ClearHistory();

    // Check if initialized
    bool IsInitialized() const { return m_ToolRouter != nullptr; }

private:
    ToolRouter* m_ToolRouter = nullptr;
    std::vector<std::pair<std::string, std::string>> m_History; // (user, assistant) pairs

    // Parse user intent and execute tools
    std::string ExecuteIntent(const std::string& intent, const nlohmann::json& params);

    // Simple keyword-based intent parsing (placeholder for LLM integration)
    std::pair<std::string, nlohmann::json> ParseIntent(const std::string& input);
};

} // namespace nebula
