file(GLOB_RECURSE GAME_GUI_SRCS "${CMAKE_SOURCE_DIR}/game-gui/GUI/*.cpp")
add_library(game_gui STATIC ${GAME_GUI_SRCS})

set(IMGUI_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/extern/imgui)
set(CORE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/game-core)

include_directories(${IMGUI_INCLUDE_DIR}
${CORE_INCLUDE_DIR})

target_link_libraries(game_gui glad::glad glm::glm imgui game_core)
