
cmake_minimum_required(VERSION 3.20)

project(game2DbyPower)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED true)

enable_language(C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED true)

add_compile_definitions(
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:RELEASE>
)

# Set External Dependency Paths
set(GLM_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/glm-src)
set(GLFW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/glfw-src/include)
set(GLAD_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/glad/include)
set(IMGUI_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/imgui)
set(STB_IMAGE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external)

# set the own libraries paths for the .exe
set(CORE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/game-core)
set(GUI_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/game-gui)

#adding the include directories
include_directories(${GLM_INCLUDE_DIR} ${IMGUI_INCLUDE_DIR} 
${GLFW_INCLUDE_DIR} ${GLAD_INCLUDE_DIR} ${CORE_INCLUDE_DIR} 
${GUI_INCLUDE_DIR} ${STB_IMAGE_INCLUDE_DIR})

#adding some libraries from github and some others that come with the project
include(ExternalProject)
add_subdirectory(external)

# adding the build for the gui that is apart 
add_subdirectory(game-gui)

# adding the core own libraries of the project
add_subdirectory(game-core)

#creating the executable that have the sanbox made it by me
file(GLOB_RECURSE SANDBOX_SRC "${CMAKE_SOURCE_DIR}/sandbox/**")
add_executable(game2DbyPower ${SANDBOX_SRC})



# define macro for debug logs
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DRELEASE")




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
add_dependencies(game2DbyPower glm glad glfw 
imgui stb_image game_core game_gui)

#linking all dependecies 
target_link_libraries(game2DbyPower glm imgui glad glfw stb_image 
game_core game_gui)

include_directories(./sandbox)



