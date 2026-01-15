find_package(OpenGL REQUIRED)

add_library("macrochip-lib" STATIC
    include/ecs_core.h

    src/ecs_core.c
)

target_include_directories("macrochip-lib" PUBLIC
    include
)
target_link_libraries("macrochip-lib" PUBLIC
    OpenGL::GL
    submodules
)
