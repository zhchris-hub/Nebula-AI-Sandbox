#include "AI/AgentOrchestrator.h"
#include "Core/Log.h"
#include <sstream>
#include <algorithm>

namespace nebula {

static int s_TaskCounter = 0;

AgentOrchestrator::AgentOrchestrator() {
    NEBULA_INFO("AgentOrchestrator created");
}

AgentOrchestrator::~AgentOrchestrator() {
    NEBULA_INFO("AgentOrchestrator destroyed");
}

void AgentOrchestrator::Init(AIPanel* aiPanel, ToolRouter* toolRouter) {
    m_AIPanel = aiPanel;
    m_ToolRouter = toolRouter;
    NEBULA_INFO("AgentOrchestrator initialized");
}

AgentTask AgentOrchestrator::ExecuteTask(const std::string& taskDescription) {
    AgentTask task;
    task.Id = "task_" + std::to_string(++s_TaskCounter);
    task.Description = taskDescription;
    task.Status = AgentTaskStatus::Running;

    NEBULA_INFO("Executing task: {0} - {1}", task.Id, taskDescription);

    // Decompose task into steps
    task.Steps = DecomposeTask(taskDescription);

    // Execute each step
    std::stringstream result;
    for (size_t i = 0; i < task.Steps.size(); i++) {
        const auto& step = task.Steps[i];
        NEBULA_INFO("Step {0}/{1}: {2}", i + 1, task.Steps.size(), step);

        std::string stepResult = ExecuteStep(step);
        result << "Step " << (i + 1) << ": " << stepResult << "\n";

        // Check for failure
        if (stepResult.find("\"success\": false") != std::string::npos) {
            task.Status = AgentTaskStatus::Failed;
            task.Result = "Failed at step " + std::to_string(i + 1) + ": " + stepResult;
            m_TaskHistory.push_back(task);
            return task;
        }
    }

    task.Status = AgentTaskStatus::Completed;
    task.Result = result.str();
    m_TaskHistory.push_back(task);

    NEBULA_INFO("Task completed: {0}", task.Id);
    return task;
}

std::vector<std::string> AgentOrchestrator::DecomposeTask(const std::string& task) {
    // Simple decomposition (placeholder for LLM-based decomposition)
    std::vector<std::string> steps;
    std::string lower = task;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    if (lower.find("create") != std::string::npos && lower.find("scene") != std::string::npos) {
        steps.push_back("Get current scene info");
        steps.push_back("Create new entities as requested");
    } else if (lower.find("physics") != std::string::npos || lower.find("gravity") != std::string::npos) {
        steps.push_back("Get physics info");
        steps.push_back("Apply physics changes");
    } else if (lower.find("info") != std::string::npos || lower.find("status") != std::string::npos) {
        steps.push_back("Get scene info");
    } else {
        steps.push_back(task);
    }

    return steps;
}

std::string AgentOrchestrator::ExecuteStep(const std::string& step) {
    if (!m_AIPanel) {
        return "{\"success\": false, \"message\": \"AI Panel not initialized\"}";
    }

    return m_AIPanel->ProcessInput(step);
}

} // namespace nebula
