set(IMGUI_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/extern/imgui/stb)
set(STB_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/extern)


file(GLOB_RECURSE IMGUI_BINDINGS_SRC
    "${CMAKE_SOURCE_DIR}/extern/imgui/bindings/*.cpp"
)
add_library(imgui_bindings STATIC ${IMGUI_BINDINGS_SRC})
target_link_libraries(imgui_bindings imgui::imgui)


file(GLOB_RECURSE IMGUI_STDLIB_SRC
    "${CMAKE_SOURCE_DIR}/extern/imgui/stdlib/*.cpp"
)
add_library(imgui_stdlib STATIC ${IMGUI_STDLIB_SRC})
target_link_libraries(imgui_stdlib imgui::imgui)



file(GLOB_RECURSE STB_IMAGE_SRC "${CMAKE_SOURCE_DIR}/extern/stb_image/*.cpp")
add_library(stb_image STATIC ${STB_IMAGE_SRC})

include_directories(${IMGUI_INCLUDE_DIR} ${STB_INCLUDE_DIR})






