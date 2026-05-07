#include "AI/DemoScene.h"
#include "Core/Log.h"

namespace nebula {

DemoScene::DemoScene() {
    NEBULA_INFO("DemoScene created");
}

DemoScene::~DemoScene() {
    NEBULA_INFO("DemoScene destroyed");
}

void DemoScene::Setup(Scene& scene) {
    NEBULA_INFO("Setting up demo scene");

    // Create ground
    auto ground = scene.CreateEntity("Ground");
    scene.AddTransformComponent(ground, {0.0f, -300.0f}, {1200.0f, 20.0f}, 0.0f);
    scene.AddSpriteComponent(ground, {0.5f, 0.5f, 0.5f, 1.0f});
    scene.AddShapeComponent(ground, ShapeType::Quad);
    scene.AddRigidBodyComponent(ground, true);

    // Create falling objects
    auto redBox = scene.CreateEntity("Red Box");
    scene.AddTransformComponent(redBox, {-200.0f, 200.0f}, {80.0f, 80.0f}, 0.0f);
    scene.AddSpriteComponent(redBox, {1.0f, 0.2f, 0.2f, 1.0f});
    scene.AddShapeComponent(redBox, ShapeType::Quad);
    scene.AddRigidBodyComponent(redBox, false, 1.0f, 0.3f, 0.6f);

    auto greenBox = scene.CreateEntity("Green Box");
    scene.AddTransformComponent(greenBox, {0.0f, 300.0f}, {60.0f, 60.0f}, 0.3f);
    scene.AddSpriteComponent(greenBox, {0.2f, 1.0f, 0.2f, 1.0f});
    scene.AddShapeComponent(greenBox, ShapeType::Quad);
    scene.AddRigidBodyComponent(greenBox, false, 1.0f, 0.3f, 0.4f);

    auto blueTriangle = scene.CreateEntity("Blue Triangle");
    scene.AddTransformComponent(blueTriangle, {200.0f, 250.0f}, {100.0f, 100.0f}, 0.0f);
    scene.AddSpriteComponent(blueTriangle, {0.2f, 0.2f, 1.0f, 1.0f});
    scene.AddShapeComponent(blueTriangle, ShapeType::Triangle);
    scene.AddRigidBodyComponent(blueTriangle, false, 1.0f, 0.3f, 0.5f);

    NEBULA_INFO("Demo scene setup complete");
}

void DemoScene::RunDemo(AgentOrchestrator& agent, Scene& scene) {
    NEBULA_INFO("Running demo tasks");

    // Demo 1: Get scene info
    NEBULA_INFO("Demo 1: Getting scene info");
    auto task1 = agent.ExecuteTask("Get scene info");
    NEBULA_INFO("Task 1 result: {0}", task1.Result);

    // Demo 2: Create new entity
    NEBULA_INFO("Demo 2: Creating new entity");
    auto task2 = agent.ExecuteTask("Create a new entity");
    NEBULA_INFO("Task 2 result: {0}", task2.Result);

    // Demo 3: Physics info
    NEBULA_INFO("Demo 3: Getting physics info");
    auto task3 = agent.ExecuteTask("Get physics info");
    NEBULA_INFO("Task 3 result: {0}", task3.Result);

    NEBULA_INFO("Demo tasks complete");
}

void DemoScene::CreatePhysicsDemo(Scene& scene) {
    // Stack of boxes
    for (int i = 0; i < 5; i++) {
        auto box = scene.CreateEntity("Box_" + std::to_string(i));
        float x = -100.0f + (i % 3) * 80.0f;
        float y = 100.0f + (i / 3) * 80.0f;
        scene.AddTransformComponent(box, {x, y}, {60.0f, 60.0f}, 0.0f);
        scene.AddSpriteComponent(box, {0.8f, 0.6f, 0.2f, 1.0f});
        scene.AddShapeComponent(box, ShapeType::Quad);
        scene.AddRigidBodyComponent(box, false, 1.0f, 0.3f, 0.3f);
    }
}

void DemoScene::CreateColorDemo(Scene& scene) {
    // Gradient of colors
    for (int i = 0; i < 5; i++) {
        auto entity = scene.CreateEntity("Color_" + std::to_string(i));
        float x = -300.0f + i * 150.0f;
        float r = static_cast<float>(i) / 4.0f;
        float g = 1.0f - r;
        scene.AddTransformComponent(entity, {x, 0.0f}, {100.0f, 100.0f}, 0.0f);
        scene.AddSpriteComponent(entity, {r, g, 0.5f, 1.0f});
        scene.AddShapeComponent(entity, ShapeType::Quad);
    }
}

void DemoScene::CreateTransformDemo(Scene& scene) {
    // Rotated and scaled objects
    for (int i = 0; i < 4; i++) {
        auto entity = scene.CreateEntity("Transform_" + std::to_string(i));
        float angle = i * 0.5f;
        float scale = 50.0f + i * 30.0f;
        scene.AddTransformComponent(entity, {-200.0f + i * 150.0f, 0.0f}, {scale, scale}, angle);
        scene.AddSpriteComponent(entity, {0.5f, 0.5f, 1.0f, 1.0f});
        scene.AddShapeComponent(entity, (i % 2 == 0) ? ShapeType::Quad : ShapeType::Triangle);
    }
}

} // namespace nebula
