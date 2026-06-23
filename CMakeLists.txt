cmake_minimum_required(VERSION 3.20)

project(PhysicsEngine)

set(CMAKE_CXX_STANDARD 20)

add_executable(
    PhysicsEngine
    main.cpp
)

target_link_libraries(
    PhysicsEngine
    sfml-graphics
    sfml-window
    sfml-system
)
