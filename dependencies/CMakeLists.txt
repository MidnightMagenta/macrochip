add_subdirectory(submodules)

add_library(submodules INTERFACE)
target_link_libraries(submodules INTERFACE
    glfw
    cglm
)

target_include_directories(submodules INTERFACE 
    include
)