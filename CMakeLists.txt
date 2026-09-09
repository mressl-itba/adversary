cmake_minimum_required(VERSION 3.22)
project(theadversary)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default to an optimized build. Without this, single-config generators
# (Makefiles, Ninja) build with no optimization at all, which makes any
# performance measurement meaningless. Override with -DCMAKE_BUILD_TYPE=Debug
# while debugging, or use the CMake Tools variant selector in Visual Studio Code.
get_property(IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(NOT IS_MULTI_CONFIG AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        Debug Release RelWithDebInfo MinSizeRel)
endif()

include(FetchContent)

FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/6.0.tar.gz
)

FetchContent_MakeAvailable(raylib)

add_executable(game_ui
    src/game_ui.cpp
    src/ui/ui.cpp
    src/game/game.cpp
    src/game/agent.cpp
    src/starter_agent/starter_agent.cpp
    src/greedy_agent/greedy_agent.cpp
)

target_link_libraries(game_ui PRIVATE raylib)

add_executable(game_headless
    src/game_headless.cpp
    src/game/game.cpp
    src/game/agent.cpp
    src/starter_agent/starter_agent.cpp
    src/greedy_agent/greedy_agent.cpp
)
