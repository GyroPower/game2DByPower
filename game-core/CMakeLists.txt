file(GLOB_RECURSE GAME_CORE_SRCS "${CMAKE_SOURCE_DIR}/game-core/**")
add_library(game_core STATIC ${GAME_CORE_SRCS})

set(IMGUI_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/extern/imgui)
set(IMGUI_INCLUDE_BINDINGS_DIR "${CMAKE_SOURCE_DIR}/extern/imgui/stdlib")
set(CORE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/game-core)
set(STB_IMAGE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/extern)

include_directories(${IMGUI_INCLUDE_DIR}
${IMGUI_INCLUDE_BINDINGS_DIR}
${CORE_INCLUDE_DIR}
${STB_IMAGE_INCLUDE_DIR})

target_link_libraries(game_core glad::glad glm::glm imgui::imgui stb_image)
