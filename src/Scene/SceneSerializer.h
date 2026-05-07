#pragma once

#include <string>
#include <nlohmann/json.hpp>

namespace nebula {

class Scene;

class SceneSerializer {
public:
    static nlohmann::json Serialize(Scene& scene);
    static bool Deserialize(Scene& scene, const nlohmann::json& json);
    static bool SaveToFile(Scene& scene, const std::string& filepath);
    static bool LoadFromFile(Scene& scene, const std::string& filepath);
};

} // namespace nebula
