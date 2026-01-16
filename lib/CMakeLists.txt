find_package(OpenGL REQUIRED)

add_library("macrochip-lib" STATIC
    include/ecs_core.h
    include/ecs_components.h
    include/mesh.h
    include/mesh_primitives.h

    src/ecs_core.c
    src/mesh_primitives.c
)

target_include_directories("macrochip-lib" PUBLIC
    include
)
target_link_libraries("macrochip-lib" PUBLIC
    OpenGL::GL
    submodules
)
