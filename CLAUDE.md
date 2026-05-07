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

# Build with tests disabled
cmake -B build -S . -DBUILD_TESTS=OFF
cmake --build build

# Run tests (after building with BUILD_TESTS=ON)
cd build && ctest --output-on-failure
```

Build requires CMake 3.20+ and a C++20 compiler. Dependencies (GLFW, spdlog, glm, nlohmann/json) are auto-fetched via FetchContent — no manual installation needed.

## Architecture

Two-layer design: **Engine Layer** (C++ runtime) and **AI Layer** (LLM-driven orchestration).

**AI Pipeline:**
```
User natural language → AI Panel → AgentOrchestrator → ToolRouter → C++ Engine APIs
```

**Implemented modules:**
- `src/Core/` — Application singleton, spdlog logging (`NEBULA_TRACE/INFO/WARN/ERROR/CRITICAL` macros), event system with typed events and category bitmasks
- `src/Platform/` — GLFW window (OpenGL 3.3 Core, 1280x720 default, VSync on), dispatches input/window events through callback

**Planned modules (directories exist, empty):** Render, Scene, ECS, Physics, Scripting, Assets, Editor, AI, Tools, Utils.

**Tool System** — the bridge between AI and engine. Each tool returns:
```json
{ "success": true, "message": "...", "data": { ... } }
```

Tool categories: Scene (CRUD entities, save/load), Assets (load textures, reload shaders), Runtime (simulation control, logs), Code (read/write files, compile, test), Scripting (Lua script management).

## Key Patterns

- **Entry point:** Client code defines `CreateApplication()` returning an `Application*`. See `src/main.cpp` for the pattern.
- **Event system:** `Event` base class with `EventType` enum and `EventCategory` bitmask. Concrete events (KeyPressed, MouseMoved, WindowClose, etc.) carry typed data. `EventCallbackFn = std::function<void(Event&)>`.
- **Logging:** Always use the `NEBULA_*` macros from `Log.h`, never raw spdlog.
- **Engine library:** All `src/` subdirectories compile into a single static `Engine` library. The main executable only contains `src/main.cpp`.

## Coding Conventions

- **Classes:** PascalCase
- **Functions:** PascalCase
- **Variables:** camelCase
- **Constants:** kCamelCase or UPPER_SNAKE_CASE
- **Namespace:** `nebula`
- **Principles:** Single responsibility, module isolation, AI layer separated from engine layer
- **Language:** C++20

## Dependencies

**Currently fetched (in CMakeLists.txt):** GLFW 3.4, spdlog v1.14.1, glm 1.0.1, nlohmann/json v3.11.3

**Planned (not yet added):** entt (ECS), Box2D (physics), Lua (scripting), Dear ImGui (editor UI), OpenGL loader (glad)
