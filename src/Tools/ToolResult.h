#pragma once

#include <nlohmann/json.hpp>
#include <string>

namespace nebula {

struct ToolResult {
    bool Success = false;
    std::string Message;
    nlohmann::json Data = nullptr;

    nlohmann::json ToJson() const {
        return {
            {"success", Success},
            {"message", Message},
            {"data", Data}
        };
    }

    static ToolResult Ok(const std::string& msg, const nlohmann::json& data = nullptr) {
        return {true, msg, data};
    }

    static ToolResult Fail(const std::string& msg) {
        return {false, msg, nullptr};
    }
};

} // namespace nebula
