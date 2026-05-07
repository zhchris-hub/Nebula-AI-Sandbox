cmake_minimum_required(VERSION 3.20)
project(NebulaAISandbox VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ── Dependencies via FetchContent ──────────────────────────────────────
include(FetchContent)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)
FetchContent_MakeAvailable(glfw)

# spdlog
FetchContent_Declare(spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.14.1
)
FetchContent_MakeAvailable(spdlog)

# glm
FetchContent_Declare(glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
)
FetchContent_MakeAvailable(glm)

# nlohmann/json
FetchContent_Declare(json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
)
FetchContent_MakeAvailable(json)

# glad (OpenGL loader)
FetchContent_Declare(glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG        v0.1.36
)
FetchContent_MakeAvailable(glad)

# ── Engine library ─────────────────────────────────────────────────────
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
    "src/Core/*.cpp"
    "src/Platform/*.cpp"
    "src/Render/*.cpp"
    "src/Scene/*.cpp"
    "src/ECS/*.cpp"
    "src/Physics/*.cpp"
    "src/Scripting/*.cpp"
    "src/Assets/*.cpp"
    "src/Editor/*.cpp"
    "src/AI/*.cpp"
    "src/Tools/*.cpp"
    "src/Utils/*.cpp"
)

add_library(Engine STATIC ${ENGINE_SOURCES})
target_include_directories(Engine PUBLIC src)
target_link_libraries(Engine PUBLIC glfw spdlog::spdlog glm::glm nlohmann_json::nlohmann_json glad)

# ── Main executable ────────────────────────────────────────────────────
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE Engine)

# Copy assets to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

# ── Tests ──────────────────────────────────────────────────────────────
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "tests/*.cpp")
    if(TEST_SOURCES)
        add_executable(EngineTests ${TEST_SOURCES})
        target_link_libraries(EngineTests PRIVATE Engine)
        add_test(NAME EngineTests COMMAND EngineTests)
    endif()
endif()
