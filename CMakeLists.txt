cmake_minimum_required(VERSION 3.0)

project(engine)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add executable target
file(GLOB SOURCES "src/*.cpp" "src/*.c" "src/imgui/*.cpp")
add_executable(engine ${SOURCES})

# Include directories
target_include_directories(engine PRIVATE "include" "include/imgui" "libs/glm")

# Link libraries
target_link_libraries(engine PRIVATE imgui glfw GL X11 pthread Xrandr Xi dl)

# Copy shaders to build directory
file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
