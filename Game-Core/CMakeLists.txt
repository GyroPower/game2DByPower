file(GLOB_RECURSE GAME_CORE_SRCS "${CMAKE_SOURCE_DIR}/game-core/**")
add_library(game_core STATIC ${GAME_CORE_SRCS})

set(GLM_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/glm-src)
set(GLFW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/glfw-src/include)
set(GLAD_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/glad/include)
set(IMGUI_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/imgui)
set(CORE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/game-core)
set(STB_IMAGE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external)

include_directories(${GLM_INCLUDE_DIR} ${IMGUI_INCLUDE_DIR} 
${GLFW_INCLUDE_DIR} ${GLAD_INCLUDE_DIR} ${CORE_INCLUDE_DIR} 
${STB_IMAGE_INCLUDE_DIR})

target_link_libraries(game_core glad glm imgui stb_image)