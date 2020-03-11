cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
cmake_policy(VERSION 3.15)

set(NAME RT)
project(${NAME} C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Disable if it bugs you
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")

# Additional cmake scripts
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
include(${CMAKE_SOURCE_DIR}/cmake/shaders.cmake)

# External libraries are here
# add_subdirectory(external)
find_package(glfw3 3.2 REQUIRED)
add_compile_definitions(GLFW_INCLUDE_VULKAN)
find_package(Vulkan REQUIRED)
find_package(Threads REQUIRED)
add_subdirectory(VulkanMemoryAllocator)

# Common Internal Code
add_subdirectory(Core)

# Individual Internal Projects
add_subdirectory(RT1)
