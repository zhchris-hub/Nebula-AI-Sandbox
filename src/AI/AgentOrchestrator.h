#pragma once

#include "AI/AIPanel.h"
#include "Tools/ToolRouter.h"
#include <string>
#include <vector>
#include <functional>

namespace nebula {

// Agent task status
enum class AgentTaskStatus {
    Pending,
    Running,
    Completed,
    Failed
};

// Agent task
struct AgentTask {
    std::string Id;
    std::string Description;
    AgentTaskStatus Status = AgentTaskStatus::Pending;
    std::string Result;
    std::vector<std::string> Steps;
};

class AgentOrchestrator {
public:
    AgentOrchestrator();
    ~AgentOrchestrator();

    // Initialize with AI panel and tool router
    void Init(AIPanel* aiPanel, ToolRouter* toolRouter);

    // Execute a high-level task (decomposed into steps)
    AgentTask ExecuteTask(const std::string& taskDescription);

    // Get task history
    const std::vector<AgentTask>& GetTaskHistory() const { return m_TaskHistory; }

    // Check if initialized
    bool IsInitialized() const { return m_AIPanel != nullptr && m_ToolRouter != nullptr; }

private:
    AIPanel* m_AIPanel = nullptr;
    ToolRouter* m_ToolRouter = nullptr;
    std::vector<AgentTask> m_TaskHistory;

    // Decompose task into steps
    std::vector<std::string> DecomposeTask(const std::string& task);

    // Execute a single step
    std::string ExecuteStep(const std::string& step);
};

} // namespace nebula
