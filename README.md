# Nebula AI Sandbox

一个基于 C++20 的轻量级沙盒引擎 + 可视化编辑器 + AI Agent 驱动的智能开发平台。

## 项目简介

Nebula AI Sandbox 是一个使用 C++20 开发的智能沙盒引擎，支持自然语言控制的可视化开发环境。用户可以用一句话让 AI 创建场景、改参数、修 bug、生成关卡、跑测试。

系统分层：

```
用户自然语言输入
        ↓
AI Chat / Agent 面板
        ↓
Agent Orchestrator（任务规划器）
        ↓
Tool Router（工具路由器）
        ↓
C++ Engine APIs / Editor APIs / Build APIs / Log APIs
        ↓
Scene / ECS / Render / Physics / Script / Assets
```

## 核心功能

### 引擎能力
- **可视化场景编辑** — ImGui 编辑器（Hierarchy / Inspector / Viewport / Console）
- **实时渲染** — OpenGL 2D 渲染，精灵/方块/相机
- **ECS 架构** — 基于 entt 的实体/组件/系统
- **物理系统** — Box2D 驱动的重力、碰撞、刚体交互
- **资源管理** — 纹理/Shader/字体/音频统一管理，支持热重载
- **Lua 脚本** — 脚本驱动行为，支持热重载
- **场景持久化** — JSON 格式保存/加载场景

### AI Agent 能力
- **自然语言控制** — 用自然语言指令操控引擎
- **任务规划** — Agent 自动分解复杂任务为可执行步骤
- **工具调用** — 通过统一 Tool API 操作场景、资源、编译、脚本
- **自动修复** — 读取日志、定位错误、自动修复
- **场景生成** — AI 自动生成 demo 场景和关卡

## 技术栈

| 类别 | 技术 |
|------|------|
| 语言 | C++20 |
| 构建 | CMake |
| 图形 | OpenGL + GLFW / SDL2 |
| UI | Dear ImGui |
| 数学 | glm |
| 日志 | spdlog |
| JSON | nlohmann/json |
| ECS | entt |
| 物理 | Box2D |
| 脚本 | Lua |
| AI | OpenAI API / Anthropic API / 本地大模型 |

## 项目结构

```
NebulaAISandbox/
├── CMakeLists.txt
├── assets/                 # 资源文件
├── docs/                   # 文档
├── external/               # 第三方库
├── prompts/                # AI 提示词模板
├── scripts/                # Lua 脚本
├── src/
│   ├── Core/               # 应用框架、日志、事件、配置
│   ├── Platform/           # 窗口、输入、文件系统
│   ├── Render/             # Shader、纹理、渲染器、相机
│   ├── Scene/              # 场景、序列化、场景管理
│   ├── ECS/                # 实体、组件、系统
│   ├── Physics/            # 刚体、碰撞体、物理世界
│   ├── Scripting/          # Lua 绑定、脚本管理
│   ├── Assets/             # 资源加载、缓存、热重载
│   ├── Editor/             # 编辑器面板 + AI Panel
│   ├── AI/                 # Agent、LLM、Prompt、Memory
│   ├── Tools/              # 工具系统（暴露引擎能力给 AI）
│   └── Utils/              # 工具类
├── tests/                  # 测试
└── README.md
```

## 工具系统

工具系统是 AI 操控引擎的核心桥梁。引擎能力通过统一 Tool API 暴露给 Agent：

| 类别 | 工具 |
|------|------|
| 场景 | list_entities, create_entity, delete_entity, set_component, save_scene, load_scene |
| 资源 | load_texture, reload_shader, list_assets |
| 运行 | start_simulation, pause_simulation, stop_simulation, read_runtime_logs |
| 代码 | read_file, write_file, compile_project, run_tests |
| 脚本 | reload_script, create_script, bind_script_to_entity |

工具返回统一 JSON 格式：

```json
{
  "success": true,
  "message": "Entity created",
  "data": { "entity_id": 1024, "name": "Player" }
}
```

## AI 模块

| 模块 | 职责 |
|------|------|
| AgentOrchestrator | 任务分解、工具调度、状态管理 |
| LLMProvider | 调用模型 API、管理消息历史、统一封装多模型 |
| PromptManager | 管理系统提示词、角色设定、任务上下文 |
| ToolRouter | 解析模型输出、校验参数、调用对应工具、汇总结果 |
| MemoryManager | 任务短期记忆、操作记录、场景上下文摘要 |

## 开发阶段

### 阶段 1：引擎骨架
- CMake 工程搭建，Application 框架，Window/Event/Input
- **完成标准：** 启动显示窗口，正常退出，控制台有日志输出

### 阶段 2：基础渲染
- OpenGL 初始化，绘制基础图形，纹理、相机、坐标系统
- **完成标准：** 窗口中能显示方块、图片或简单场景

### 阶段 3：ECS + Scene
- Entity/Component/System，Scene，SceneSerializer
- **完成标准：** 场景管理多个对象，支持保存和加载

### 阶段 4：编辑器
- ImGui 编辑器（Hierarchy / Inspector / Console / Viewport）
- **完成标准：** 通过 UI 查看和修改场景对象

### 阶段 5：物理和脚本
- Box2D 集成，Lua 脚本绑定，热重载
- **完成标准：** 对象能掉落、碰撞、弹跳，脚本控制行为

### 阶段 6：AI 工具接入
- LLM 接入，Tool API 封装，AI Panel
- **完成标准：** 能用自然语言控制引擎

### 阶段 7：Agent 化
- Agent 自动规划、多步执行、自动修复
- **完成标准：** 项目具备完整 Agent 特征

### 阶段 8：最终 Demo
- 生成完整可演示场景，截图/视频/README
- **完成标准：** 可用于作品集和展示

## 版本路线图

| 版本 | 内容 |
|------|------|
| v0.1 | 能启动、开窗口、输出日志 |
| v0.2 | 基础图形渲染、输入响应 |
| v0.3 | ECS 框架、场景对象管理 |
| v0.4 | ImGui 编辑器、Inspector / Hierarchy |
| v0.5 | 场景保存/加载、资源管理 |
| v0.6 | 物理系统、Lua 脚本 |
| v0.7 | AI 工具接入、AI Panel |
| v0.8 | Agent 自动规划、多步执行 |
| v1.0 | 完整 Demo、文档、截图、演示视频 |

## MVP 范围

**MVP 必须包含：** 窗口、渲染、输入、场景、ECS、编辑器、保存/加载、AI 面板、基础工具调用

**MVP 暂不包含：** 高级自动修复 Agent、多模型切换、联网功能、复杂 3D

## 非目标范围

- 完整 AAA 级 3D 引擎 / Vulkan 全家桶
- 多人联机同步
- 复杂大地图流式加载
- 高级程序化世界生成
- 复杂动画编辑器
- 商业级资产管线
- 全自动无约束 Agent 自主改代码

## 编码规范

- **类名：** PascalCase
- **函数名：** PascalCase
- **变量名：** camelCase
- **常量：** kCamelCase 或 UPPER_SNAKE_CASE
- **原则：** 单一职责、模块隔离、AI 层与引擎层分离

## 展示效果

**场景生成：** "创建一个 2D 平台关卡，放一个玩家、三个障碍物和一个终点。" → AI 自动生成

**参数调优：** "把所有敌人的速度调快 30%，重力设成 12。" → AI 自动修改

**自动修 bug：** "当前编译失败，帮我分析并修复。" → AI 读日志、找文件、改代码、重新编译

**Demo 搭建：** "帮我搭一个物理沙盒 demo，能掉落方块、碰撞、弹跳。" → AI 自动创建

## 许可证

待定
