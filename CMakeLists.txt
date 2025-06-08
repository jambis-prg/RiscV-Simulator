cmake_minimum_required(VERSION 3.10.0)
project(RiscVSimulator VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

# Para Debug
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

# Para Release
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

set(OpenGL_GL_PREFERENCE LEGACY)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(RiscVSimulator 
src/main.cpp 
src/application.cpp
src/glfw_custom_allocator.cpp
src/imgui_layer.cpp
src/logger.cpp

vendor/imgui/backends/imgui_impl_glfw.cpp 
vendor/imgui/backends/imgui_impl_opengl3.cpp 
vendor/imgui/imgui.cpp
vendor/imgui/imgui_widgets.cpp
vendor/imgui/imgui_draw.cpp
vendor/imgui/imgui_tables.cpp
)

target_include_directories(RiscVSimulator PRIVATE src vendor/imgui/backends vendor/imgui)
target_link_libraries(RiscVSimulator PRIVATE glfw OpenGL::GL)