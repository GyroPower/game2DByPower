include(FetchContent)

set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/external)

FetchContent_Declare(
    glm 
    GIT_REPOSITORY https://github.com/g-truc/glm.git 
    GIT_TAG 0.9.9.8

)

# FetchContent_Declare(
#     imgui
#     GIT_REPOSITORY https://github.com/ocornut/imgui.git 
#     GIT_TAG v1.89.3 
# )

FetchContent_Declare(
    glfw 
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

FetchContent_MakeAvailable(glm glfw)




# Set External Dependency Paths
set(GLM_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/glm-src)
set(GLFW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/glfw-src/include)
set(GLAD_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/glad/include)
set(IMGUI_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/imgui)
set(STB_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external)

# manually add imgui
file(GLOB_RECURSE IMGUI_SRCS "${CMAKE_SOURCE_DIR}/external/imgui/**")
add_library(imgui STATIC ${IMGUI_SRCS})

# manually add glad
file(GLOB_RECURSE GLAD_SRC "${CMAKE_SOURCE_DIR}/external/glad/**")
add_library(glad STATIC ${GLAD_SRC})

file(GLOB_RECURSE STB_IMAGE_SRC "${CMAKE_SOURCE_DIR}/external/stb_image/**")
add_library(stb_image STATIC ${STB_IMAGE_SRC})

include_directories(${GLM_INCLUDE_DIR} ${IMGUI_INCLUDE_DIR} 
${GLFW_INCLUDE_DIR} ${GLAD_INCLUDE_DIR} ${STB_INCLUDE_DIR})






