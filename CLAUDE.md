# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Nebula AI Sandbox — a C++20 sandbox engine with ImGui editor and AI Agent integration. Users control the engine via natural language; the AI decomposes tasks, calls engine Tool APIs, and operates the scene/rendering/physics/scripting systems.

## Build Commands

```bash
# Configure (first time, or after CMakeLists.txt changes)
cmake -B build -S .

# Build (Debug)
cmake --build build

# Build (Release)
cmake --build build --config Release

# Run tests (after building with BUILD_TESTS=ON)
cd build && ctest --output-on-failure
```

Build requires CMake 3.20+ and a C++20 compiler (MSVC 2022). Dependencies are auto-fetched via FetchContent.

## Architecture

Two-layer design: **Engine Layer** (C++ runtime) and **AI Layer** (LLM-driven orchestration).

**AI Pipeline:**
```
User natural language → AIPanel → AgentOrchestrator → ToolRouter → C++ Engine APIs
```

**Implemented Modules:**
- `src/Core/` — Application singleton, spdlog logging, event system
- `src/Platform/` — GLFW window (OpenGL 3.3 Core, 1280x720)
- `src/Render/` — Renderer, Camera, Shader
- `src/Scene/` — ECS with entt, components (Tag, Transform, Sprite, Shape, RigidBody)
- `src/Physics/` — Box2D wrapper (PhysicsWorld)
- `src/Scripting/` — Lua/sol2 scripting (LuaScript)
- `src/Editor/` — ImGui editor (Hierarchy, Inspector, Console, AI Chat panels)
- `src/AI/` — AIPanel, AgentOrchestrator, DemoScene
- `src/Tools/` — ITool interface, ToolRouter, SceneTool, RuntimeTool, ScriptingTool

**Tool System** — bridge between AI and engine. Each tool returns:
```json
{ "success": true, "message": "...", "data": { ... } }
```

**Registered Tools:**
- `scene` — Entity CRUD, transform/color manipulation
- `runtime` — Physics control, gravity, body management
- `scripting` — Lua script load/execute/run_code

## Key Patterns

- **Entry point:** `CreateApplication()` returns `Application*`. See `src/main.cpp`.
- **Event system:** `Event` base with `EventType` enum and `EventCategory` bitmask.
- **Logging:** Always use `NEBULA_*` macros from `Log.h`.
- **ECS:** Components are plain structs. Scene manages entity lifecycle.
- **Physics:** 30px = 1 meter scale factor for Box2D.
- **Editor:** Forward-declare EditorLayer in header, include only in .cpp.

## Coding Conventions

- Classes: PascalCase, Functions: PascalCase, Variables: camelCase
- Namespace: `nebula`
- Language: C++20

## Dependencies

GLFW 3.4, spdlog v1.14.1, glm 1.0.1, nlohmann/json v3.11.3, entt v3.13.2, glad v0.1.36, ImGui v1.90.4, Box2D v2.4.1, Lua 5.4.7, sol2 v3.3.0
