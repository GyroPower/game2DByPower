file(GLOB_RECURSE GAME_GUI_SRCS "${CMAKE_SOURCE_DIR}/game-gui/GUI/**")
add_library(game_gui STATIC ${GAME_GUI_SRCS})

set(GLM_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/glm-src)
set(GLFW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/glfw-src/include)
set(GLAD_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/glad/include)
set(IMGUI_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/imgui)
set(CORE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/game-core)

include_directories(${GLM_INCLUDE_DIR} ${IMGUI_INCLUDE_DIR} 
${GLFW_INCLUDE_DIR} ${GLAD_INCLUDE_DIR} ${CORE_INCLUDE_DIR})

target_link_libraries(game_gui glad glm imgui game_core)