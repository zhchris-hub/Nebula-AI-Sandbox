#pragma once

#include "Scene/Scene.h"
#include "AI/AgentOrchestrator.h"

namespace nebula {

class DemoScene {
public:
    DemoScene();
    ~DemoScene();

    // Setup demo scene
    void Setup(Scene& scene);

    // Run demo tasks
    void RunDemo(AgentOrchestrator& agent, Scene& scene);

private:
    void CreatePhysicsDemo(Scene& scene);
    void CreateColorDemo(Scene& scene);
    void CreateTransformDemo(Scene& scene);
};

} // namespace nebula
