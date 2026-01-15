add_subdirectory(submodules)
add_subdirectory(glad)

add_library(submodules INTERFACE)
target_link_libraries(submodules INTERFACE
    glfw
    glad
)

