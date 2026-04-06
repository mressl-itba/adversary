cmake_minimum_required(VERSION 3.22)
project(spacesim)

set(CMAKE_CXX_STANDARD 20)

find_package(raylib CONFIG REQUIRED)

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
