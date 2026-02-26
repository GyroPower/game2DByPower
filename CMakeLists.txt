
cmake_minimum_required(VERSION 3.10...3.20)

set(CMAKE_POLICY_VERSION_MINIMUM 3.10)

project(game2DbyPower)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

enable_language(C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED true)

add_compile_definitions(
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:RELEASE>
)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(glad REQUIRED)
find_package(imgui REQUIRED)

add_subdirectory(extern)

# adding the core own libraries of the project
add_subdirectory(game-core)

# adding the build for the gui that is apart
add_subdirectory(game-gui)



#creating the executable that have the sanbox made it by me
file(GLOB_RECURSE SANDBOX_SRC "${CMAKE_SOURCE_DIR}/sandbox/*.cpp")
add_executable(game2DbyPower ${SANDBOX_SRC})



# define macro for debug logs
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DRELEASE")

# Set External Dependency Paths

set(IMGUI_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/extern/imgui/stdlib)
set(IMGUI_INCLUDE_BINDINGS_DIR "${CMAKE_SOURCE_DIR}/extern/imgui/")
set(STB_IMAGE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/extern)

# set the own libraries paths for the .exe
set(CORE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/game-core)
set(GUI_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/game-gui)

#adding the include directories
target_include_directories(game2DbyPower PUBLIC  ${IMGUI_INCLUDE_DIR}
 ${CORE_INCLUDE_DIR} ${IMGUI_INCLUDE_BINDINGS_DIR}
${GUI_INCLUDE_DIR} ${STB_IMAGE_INCLUDE_DIR})



# set the shaders and resources in the binary directory
set(SHADER_DIR ${CMAKE_SOURCE_DIR}/shaders)
set(TEXTURE_DIR ${CMAKE_SOURCE_DIR}/resources)

set(OUTPUT_SHADER_DIR ${CMAKE_BINARY_DIR}/shaders)
set(OUTPUT_TEXTURE_DIR ${CMAKE_BINARY_DIR}/resources)

add_custom_target(
    copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${SHADER_DIR} ${OUTPUT_SHADER_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${TEXTURE_DIR} ${OUTPUT_TEXTURE_DIR}
)
add_dependencies(game2DbyPower copy_assets)


# Dependencies should be built first
add_dependencies(game2DbyPower
 imgui_stdlib imgui_bindings stb_image game_core game_gui)

#linking all dependecies
target_link_libraries(game2DbyPower glm::glm imgui_bindings imgui_stdlib glad::glad glfw stb_image
game_core game_gui)

include_directories(./sandbox)



